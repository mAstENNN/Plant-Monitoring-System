# Plant-Monitoring-System

# Abstract:

Smart Plant Monitoring System helps with taking care of indoor plants as well as outdoor plants. It uses Arduino boards with sensors to measure light, soil moisture, temperature, and humidity around each plant or a general area of soil if using it outdoors. This information is sent to a main Arduino unit and then to an easy-to-use web app. This lets users watch and compare the health of their plants in real time. The goal is to make taking care of plants easier by maintaining the best growing conditions and meeting the needs of plant lovers effectively.

## Project Ideas:

Overall Description of Project Idea
The Smart Plant Monitoring System uses an arduino with a different suite of sensors to improve indoor and outdoor plant care. It connects two or more Arduino boards, each linked to a plant, to gather data from different sensors and then send that data to a Central Hub / Server for processing and display the data to the user. This will offer a detailed understanding of the health and requirements of each plant.

## Initial Project Design stating how Multiple Arduinos will be used

3 Arduino boards connected to 3 plants along with their respective sensors. These individual Arduino boards are both connected to another arduino which is the central system for receiving and computation of information by the sensors. The central unit will also redirect to the front end of an application which displays the day to the end user.

## Expected Plan for Use and Communication between the multiple Arduinos

Communication between the individual Arduino boards and the central unit will be facilitated through WiFi (ESP8266 Serial Wifi Module). This setup allows for the easy addition of more plants to the system and ensures data is consistently and reliably transmitted to the user's interface.

## Initial Project Design stating Expected Inputs/Outputs

Inputs:
Light sensors x3
Soil Moisture sensors x3
Heat and Humidity sensor (DHT11) x3
ESP 8266 NodeMCU CP2102 ESP-12E x4
Potentiometer x3

Outputs:
Real-time data streaming to a web app interface
ESP 8266 NodeMCU CP2102 ESP-12E x4
LCD x3

# Our project introduces an approach to plant care by integrating a scalable network of Arduino-based sensors and a central processing unit. This design simplifies the monitoring of multiple plants. Each plant gets its own set of sensors that check things like soil moisture, temperature, humidity, and light. These sensors send information to a main Server powered by ESP8266, which acts like the brain of the system. The main board looks at the data and send it to the web app that will display the data in real-time for the user to see
