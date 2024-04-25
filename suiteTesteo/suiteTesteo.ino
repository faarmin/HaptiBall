
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "html.h"
#include <Arduino_JSON.h>
#include <math.h>

#define MOTOR_A 12 /
#define MOTOR_B 13 
#define MOTOR_C 14 

// Crear origen de eventos
AsyncEventSource events("/events");

AsyncWebServer server(80);
Adafruit_MPU6050 mpu;

//HOTSPOT 
const char* ssid = "-------";       
const char* password = "--------";

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

//Error
float gyroXerror = 0.07;
float gyroYerror = 0.1;
float gyroZerror = 0.01;

// Variables de tiempo
unsigned long lastTime = 0;  
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long accelerometerDelay = 200;

sensors_event_t a, g, temp;
JSONVar readings;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  Serial.println("Adafruit MPU6050 test!");

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

//wifi protocol
  WiFi.mode(WIFI_STA);                  
  WiFi.begin(ssid, password);
   
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                     
  while(WiFi.waitForConnectResult() != WL_CONNECTED){
    Serial.print(".");
    } 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());      
 
  //server.on("/", MainPage);             
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String _html_page = html_page;              
    request->send(200, "text/html", _html_page);  
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    gyroY=0;
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  //motor superior accionado
  server.on("/Sup", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(MOTOR_A, HIGH); // Enciende el motor solenoide
      delay(1000); // Espera 1 segundo antes de repetir el ciclo
      digitalWrite(MOTOR_A, LOW); // Apaga el motor solenoide
    request->send(200, "text/plain", "OK");
  });

  //motor inferior izquierdo accionado
  server.on("/InfIz", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(MOTOR_B, HIGH); 
      delay(1000); 
      digitalWrite(MOTOR_B, LOW); 
    request->send(200, "text/plain", "OK");
  });

  //motor inferior derecho accionado
  server.on("/InfDer", HTTP_GET, [](AsyncWebServerRequest *request){
      digitalWrite(MOTOR_C, HIGH); 
      delay(1000); 
      digitalWrite(MOTOR_C, LOW); 
    
    request->send(200, "text/plain", "OK");
  });


  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
}
//***********ROTACION ANGULAR - RADIANES Y GRADOS
String getAngularRotation(){
  mpu.getEvent(&a, &g, &temp);

  long tiempo_prev;
  float dt;
  float ang_x, ang_y;
  float ang_x_prev, ang_y_prev;

  dt = (millis()-tiempo_prev)/1000.0;
  tiempo_prev=millis();
  
  //inclinacion según velocidad angular
  float accel_ang_x=atan(a.acceleration.x/sqrt(pow(a.acceleration.y,2) + pow(a.acceleration.z,2)))*(180.0/3.14);
  float accel_ang_y=atan(a.acceleration.y/sqrt(pow(a.acceleration.x,2) + pow(a.acceleration.z,2)))*(180.0/3.14);

  //Calcular angulo de rotación con giroscopio y filtro complemento  
  ang_x = 0.98*(ang_x_prev+(g.gyro.x/131)*dt) + 0.02*accel_ang_x;
  ang_y = 0.98*(ang_y_prev+(g.gyro.y/131)*dt) + 0.02*accel_ang_y;
  

  ang_x_prev=ang_x;
  ang_y_prev=ang_y;


  readings["angXrad"] = String(ang_x);
  readings["angYrad"] = String(ang_y);
  readings["angX"] = String(ang_x* (180/M_PI));
  readings["angY"] = String(ang_y* (180/M_PI));

  //Serial.print("Rotacion en X:  ");
  //Serial.println(ang_x* (180/M_PI)); 
  //Serial.print("Rotacion en Y: ");
  //Serial.println(ang_y* (180/M_PI));
  

  String jsonString = JSON.stringify(readings);
  return jsonString;


}

String getGyroscope(){
  mpu.getEvent(&a, &g, &temp);
  float gyroX_temp = g.gyro.x;
  if(abs(gyroX_temp) > gyroXerror)  {
    gyroX += gyroX_temp/50.00;
  }
  float gyroY_temp = g.gyro.y;
  if(abs(gyroY_temp) > gyroYerror) {
    gyroY += gyroY_temp/200.00;
  }
  float gyroZ_temp = g.gyro.z;
  if(abs(gyroZ_temp) > gyroZerror) {
    gyroZ += gyroZ_temp/90.00;
  }

  readings["gyroX"] = String(gyroX);
  readings["gyroY"] = String(gyroY);
  readings["gyroZ"] = String(gyroZ);

  String jsonString = JSON.stringify(readings);
  return jsonString;

}


String getAccelerometer() {
  mpu.getEvent(&a, &g, &temp);
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString = JSON.stringify (readings);
  return accString;
}



void loop() {
   mpu.getEvent(&a, &g, &temp);
  if ((millis() - lastTime) > gyroDelay) {
    // manda evento
    events.send(getGyroscope().c_str(),"gyro_readings",millis());
    lastTime = millis();
  }
  if ((millis() - lastTimeAcc) > accelerometerDelay) {
    events.send(getAccelerometer().c_str(),"accelerometer_readings",millis());
    events.send(getAngularRotation().c_str(),"combined_reading",millis());
    lastTimeAcc = millis();
  }


}
