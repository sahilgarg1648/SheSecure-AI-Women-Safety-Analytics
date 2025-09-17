# Women Safety Analytics: Smart Women Safety Monitoring System

## Overview
The Women Safety Analytics system is a comprehensive solution designed to enhance women's safety in real-time. Developed from January 2023 to April 2023, this system integrates IoT sensors, advanced analytics, and real-time threat detection capabilities to ensure the protection of women in various environments. The system is capable of identifying potential dangers and provides alerts to enhance safety measures.

## Key Features
- **IoT Sensor Integration**: The system utilizes IoT sensors to collect real-time data from the environment to detect potential threats.
- **Threat Detection**: The system can identify up to 95% of potential dangers using advanced analytics and sensors.
- **Gender Detection**: The system employs gender recognition technology to ensure that safety measures are gender-specific.
- **Emotion Recognition**: Real-time emotion recognition helps detect distress signals or emotional state changes, alerting authorities or caregivers.
- **Real-Time Monitoring**: Continuous monitoring of the user's surroundings, with the ability to alert both the user and emergency contacts in case of detected threats.
- **Alerts and Notifications**: The system sends instant alerts in the event of potential threats, ensuring timely responses.

## File Structure
The following files are included in this repository:

- **diagram.json**: A JSON file containing the system's architecture and flow diagram for visualization.
- **mlmodel.h5**: The trained machine learning model used for emotion recognition and threat detection.
- **sketch.ino**: The Arduino sketch used for interfacing with IoT sensors and handling the data collection.
- **wokwi-project.txt**: A file containing configuration details for the Wokwi simulation platform, used to simulate the IoT device and sensors.

## Installation
1. Clone this repository to your local machine.
2. Ensure you have Python 3.x installed.
3. Install the required Python packages for running the machine learning model:
   ```bash
   pip install -r requirements.txt
