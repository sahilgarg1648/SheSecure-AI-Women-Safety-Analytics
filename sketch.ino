//hello


#include <WiFi.h>
#include <HTTPClient.h>

// WiFi credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ThingSpeak API credentials
const String api_key = "U1YYIONE9R2DAFGO";  // ThingSpeak API Key
const String channel_id = "2704756";         // ThingSpeak Channel ID
const String server = "http://api.thingspeak.com";

// Pin Definitions
#define BUTTON_PIN 15
#define BUZZER_PIN 4
#define LDR_PIN 36
#define MIC_PIN 36
#define LED_PIN 17

// Thresholds and Counters
int lightThreshold = 1500;
int soundThreshold = 2100;
int soundCount = 0;

// Timing Variables
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 10000;  // 10 seconds
unsigned long lastSoundTime = 0;
const unsigned long debounceDelay = 300;
const unsigned long detectionWindow = 5000;  // 5 seconds

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi
  connectToWiFi();
   Serial.println("Welcome to Women Safety Monitor");
}

void loop() {
  unsigned long currentTime = millis();

  // Handle SOS Button
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("SOS activated! Alerting the Authorities!!");
    triggerAlert("Woman in Danger");
  }

  // LDR Detection
  if (currentTime - lastPrintTime >= printInterval) {
    int lightValue = analogRead(LDR_PIN);
    if (lightValue > lightThreshold) {
      Serial.println("Daytime: Press SOS if you feel any threat.");
    } else {
      Serial.println("Nighttime: Press SOS if you feel any threat.");
    }
    lastPrintTime = currentTime;
  }

  // Sound Detection
  detectSound(currentTime);

  // Fetch and handle data from ThingSpeak every 30 seconds
  if (currentTime % 30000 < 100) {
    fetchDataFromThingSpeak();
  }

  delay(100);  // Avoid rapid looping
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

}

bool timeIsNight() {
  int lightValue = analogRead(LDR_PIN);  // Read LDR value
  return lightValue < lightThreshold;    // True if below threshold (night)
}

void fetchDataFromThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = server + "/channels/" + channel_id + "/feeds/last.json?api_key=" + api_key;

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("ThingSpeak Response: ");
      Serial.println(response);

      // Extract data fields from JSON
      int men = extractField(response, "\"field1\":\"");
      int women = extractField(response, "\"field2\":\"");
      int fear = extractField(response, "\"field5\":\"");
      int angry = extractField(response, "\"field4\":\"");
      int happy = extractField(response, "\"field3\":\"");
      int neutral = extractField(response, "\"field6\":\"");

      // Check conditions and trigger alerts
      if (women >= 1 && men == 0) {
        Serial.println("ALERT: Woman detected alone. Ensure safety.");
        triggerAlert("Woman detected alone. Stay alert.");
      } 
      else if (men > women) {
        Serial.println("ALERT: Potential threat detected—more men present.");
        triggerAlert("Alert: Ensure safety due to an imbalanced gender ratio.");
      }

      if (fear > 50 || angry > 50) {
        Serial.println("ALERT: Fear or anger detected! Take action immediately.");
        triggerAlert("Fear/Anger detected. Immediate intervention required.");
      } 
      else if (neutral > 50) {
        Serial.println("Neutral mood detected. Monitor for any changes.");
      }

      if (happy > 50) {
        Serial.println("All clear: Happy environment detected.");
      } 
      else if (fear < 20 && angry < 20 && neutral > 60) {
        Serial.println("Environment stable: Neutral state detected.");
      }

      if (happy < 20 && (fear > 40 || angry > 40)) {
        Serial.println("ALERT: Unstable environment—possible danger detected.");
        triggerAlert("Unstable environment detected. Take action!");
      }

      if (men >= 5 && women == 0 && timeIsNight()) {
        Serial.println("ALERT: Large group of men detected at night.");
        triggerAlert("Potential danger: Group of men detected at night.");
      }

      if (women >= 2 && happy > 70) {
        Serial.println("Positive environment: Multiple women detected and happy.");
      }

    } else {
      Serial.printf("Error fetching data from ThingSpeak. Code: %d\n", httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectToWiFi();
  }
}

int extractField(const String& response, const String& field) {
  int index = response.indexOf(field);
  if (index >= 0) {
    int start = index + field.length();
    int end = response.indexOf("\"", start);
    return response.substring(start, end).toInt();
  }
  return 0;  // Default value if extraction fails
}

void detectSound(unsigned long currentTime) {
  int soundValue = analogRead(MIC_PIN);
  if (soundValue > soundThreshold) {
    if (currentTime - lastSoundTime < detectionWindow) {
      soundCount++;
    } else {
      soundCount = 1;
    }
    lastSoundTime = currentTime;

    if (soundCount >= 3) {
      triggerAlert("DANGER! Loud sound detected multiple times.");
      soundCount = 0;
    }
  }
}

void triggerAlert(const char* message) {
  Serial.println(message);
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(3000);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}