#include <Wire.h> //include the wire library for i2c communication
#include <ESP8266WiFi.h> //include the esp8266 wifi library

const char* ssid = "mastenn"; //ssid of the wifi network
const char* password = "123456789"; //password of the wifi network
const char* host = "192.168.65.148"; //ip address of the server to send data
String sensorData; //string to store the sensor data

void setup() {
  Serial.begin(9600); //initialize serial communication at 9600 baud rate
  Wire.begin(); //start i2c as master
  WiFi.begin(ssid, password); //begin wifi connection
  
  while (WiFi.status() != WL_CONNECTED) { //wait for wifi connection
    delay(500); //delay 500 ms
    Serial.print("."); //print dots to indicate waiting
  }
  
  Serial.println("\nWiFi connected"); //notify when wifi is connected
  Serial.print("IP Address: "); //print the local ip address
  Serial.println(WiFi.localIP());
  Serial.println("Master Device Ready"); //indicate the master device is ready
}

void loop() {
  sensorData = ""; //reset sensorData string
  Serial.println("Requesting data from Slave..."); 
  Wire.requestFrom(8, 128); //request 128 bytes from slave device at address 8
  
  while (Wire.available()) { //while data is available from slave
    char c = Wire.read(); //read a character from the buffer
    if (c >= 32 && c <= 126) { //filter to readable ascii range
      sensorData += c; //append character if it's printable
    }
  }

  if (!sensorData.isEmpty()) { //check if any sensor data was received
    Serial.print("Received sensor data: "); 
    Serial.println(sensorData); //print the received data

    WiFiClient client; //create a wifi client
    if (client.connect(host, 80)) { //connect to server at given host and port
      Serial.println("Connected to server, sending data..."); 
      client.println("GET /data?" + sensorData + " HTTP/1.1"); //send GET request with sensor data
      client.println("Host: " + String(host)); //send the host header
      client.println("Connection: close"); //close the connection after completion
      client.println(); //end of headers

      while (client.connected()) { //while connected to the server
        while (client.available()) { //while there's data from the server
          String line = client.readStringUntil('\n'); //read line by line
          Serial.println(line); //print each line received from the server
        }
      }
      client.stop(); //stop the client
    } else {
      Serial.println("Connection to server failed"); //print if connection failed
    }
  } else {
    Serial.println("No data received from slave"); //print if no data was received
  }

  delay(5000); //wait 5 seconds before next request
}
