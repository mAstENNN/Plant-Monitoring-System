#include <ESP8266WiFi.h> //include wifi module library
#include <ESP8266WebServer.h> //include web server library for esp8266

// change according to your network
const char* ssid = "mastenn"; //wifi ssid
const char* password = "123456789"; //wifi password

ESP8266WebServer server(80); //create web server on port 80

String sensorData = "No data received yet"; //default message for sensor data
int nextClientID = 1; //initial client id

struct ClientInfo {
  String ip; //client ip address
  int id; //client identifier
};

ClientInfo clients[10]; //array to hold up to 10 clients
int clientCount = 0; //current number of connected clients

void setup() {
  Serial.begin(9600); //start serial communication at 9600 baud rate
  WiFi.begin(ssid, password); //start wifi with ssid and password

  while (WiFi.status() != WL_CONNECTED) { //wait until connected to wifi
    delay(500); //delay 500 ms
    Serial.print("."); //print dots while waiting for connection
  }

  Serial.println("\nConnected to WiFi"); //indicate wifi connection success
  Serial.print("IP address: "); //prompt for IP address
  Serial.println(WiFi.localIP()); //print the assigned IP address

  server.on("/", handleRoot); //define root directory handler         
  server.on("/data", handleData); //define data endpoint handler     
  server.begin(); //start server
  Serial.println("HTTP server started"); //indicate server start
}

int getClientId(String ip) { //retrieve or assign client id based on ip
  for (int i = 0; i < clientCount; i++) { //search through current clients
    if (clients[i].ip == ip) { //if client exists
      return clients[i].id; //return existing id
    }
  }
  
  if (clientCount < sizeof(clients) / sizeof(clients[0])) { //check for available space
    clients[clientCount].ip = ip; //set client ip
    clients[clientCount].id = nextClientID; //set new client id
    clientCount++; //increment client count
    return nextClientID++; //return and increment client id
  } else {
    Serial.println("Client array full!"); //error message if no space
    return -1; //return error
  }
}

void handleRoot() { //handle requests to root url
  String clientIP = server.client().remoteIP().toString(); //get client ip
  int clientId = getClientId(clientIP); //get or create client id
  
  String html = "<!DOCTYPE html><html><head></head><body>"; //start html
  html += "<h1>Sensor Data:</h1>"; //add header
  html += "<p id='sensorData'>" + sensorData + "</p>"; //add sensor data paragraph
  html += "<script>"; //start script
  html += "setInterval(function() {"; //set interval for live updates
  html += "fetch('/data').then(response => response.text()).then(data => {"; //fetch data from server
  html += "document.getElementById('sensorData').innerHTML = data.replace(/<br>/g, '<br/>');"; //update html
  html += "});"; //end fetch
  html += "}, 5000);"; //repeat every 5000 ms
  html += "</script>"; //end script
  html += "</body></html>"; //end html
  server.send(200, "text/html", html); //send html response

  Serial.print("Client connected from IP: "); //log client ip
  Serial.print(clientIP); //print client ip
  Serial.print(" as Client "); //log client number
  Serial.println(clientId); //print client id
}

void handleData() { //handle data requests
  String clientIP = server.client().remoteIP().toString(); //get client ip
  int clientId = getClientId(clientIP); //get or create client id
  
  if (server.args() > 0) { //if there are arguments
    sensorData = "Client ID: " + String(clientId) + "<br>"; //start sensor data string
    for (uint8_t i = 0; i < server.args(); i++) { //loop through arguments
      String argName = server.argName(i); //get argument name
      String argValue = server.arg(i); //get argument value
      sensorData += argName + ": " + argValue + "<br>"; //append data

      Serial.print("Data from Client "); //log data receipt
      Serial.print(clientId); //log client id
      Serial.print(" - "); //log separator
      Serial.print(argName); //log argument name
      Serial.print(": "); //log separator
      Serial.println(argValue); //log value
    }
  }
  server.send(200, "text/plain", "Data updated"); //send response
  Serial.println("Sensor data updated and sent to client"); //log update
}

void loop() {
  server.handleClient(); //handle incoming client requests
}
