#include <Wire.h> //include the wire library for i2c communication
#include <DHT.h> //include the dht library for temperature and humidity sensor

#define DHTPIN 7 //define the pin where the dht sensor is connected
#define DHTTYPE DHT11 //define the type of dht sensor
DHT dht(DHTPIN, DHTTYPE); //initialize the dht sensor

const int dry = 595; //calibrated value for dry soil
const int wet = 239; //calibrated value for wet soil
int photoPin = A1; //pin for the light sensor

unsigned long lastSensorRead = 0; //stores the last time sensors were read
const long sensorReadInterval = 3000; //interval to read sensors in milliseconds

int humidity = 0, temperature = 0; //variables to store humidity and temperature values
int soilMoisture = 0, lightLevel = 0; //variables to store soil moisture and light level values

void setup() {
  Serial.begin(9600); //begin serial communication at 9600 baud rate
  dht.begin(); //start the dht sensor
  pinMode(photoPin, INPUT); //set photopin as input

  Wire.begin(8); //start i2c as slave on address 8
  Wire.onRequest(requestEvent); //register the event for i2c request
  Serial.println("Slave Device Ready"); //print device ready message
}

void loop() {
  unsigned long currentMillis = millis(); //get current time
  if (currentMillis - lastSensorRead >= sensorReadInterval) { //check if it's time to read sensors
    lastSensorRead = currentMillis; //update last sensor read time

    humidity = dht.readHumidity(); //read humidity
    temperature = dht.readTemperature(); //read temperature
    soilMoisture = map(analogRead(A0), wet, dry, 100, 0); //read and scale soil moisture
    lightLevel = map(analogRead(photoPin), 300, 0, 100, 0); //read and scale light level

    Serial.print("Humidity: "); 
    Serial.print(humidity); 
    Serial.println("%, ");

    Serial.print("temperature: "); 
    Serial.print(temperature); 
    Serial.println("°C, ");

    Serial.print("Soil Moisture: "); 
    Serial.print(soilMoisture); 
    Serial.print("%, ");

    Serial.print("Light Level: "); 
    Serial.print(lightLevel); 
    Serial.println("%");

    Serial.println(" ");  
  }
}

void requestEvent() { //handle i2c request
    String sensorData; //using string for easier concatenation
    if (isnan(humidity) || isnan(temperature)) { //check if sensor values are not a number
        sensorData = "Temp=?&Hum=?&Soil=" + String(soilMoisture) + "&Light=" + String(lightLevel); //handle missing temperature and humidity
    } else {
        sensorData = "Temp=" + String((int)temperature) + "&Hum=" + String((int)humidity) + 
                     "&Soil=" + String(soilMoisture) + "&Light=" + String(lightLevel); //concatenate valid sensor values into a string
    }
    Wire.write(sensorData.c_str(), sensorData.length()); //send data over i2c
}


