
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <math.h>
#include "StaticWebs.h"

//Solenoides
#define MOTOR_A 12 
#define MOTOR_B 13 
#define MOTOR_C 14 

const int intensidadMOTOR_A = 0;
const int pwmFrequencyMOTOR_A = 5000; // Frecuencia PWM en Hz
const int pwmResolutionMOTOR_A = 8; // Resolución PWM en bits (0-255)

const int intensidadMOTOR_B= 0;
const int pwmFrequencyMOTOR_B = 5000; // Frecuencia PWM en Hz
const int pwmResolutionMOTOR_B = 8; // Resolución PWM en bits (0-255)

const int intensidadMOTOR_C= 0;
const int pwmFrequencyMOTOR_C = 5000; // Frecuencia PWM en Hz
const int pwmResolutionMOTOR_C = 8; // Resolución PWM en bits (0-255)

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long accelerometerDelay = 200;

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

//Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.1;
float gyroZerror = 0.01;


//****************************************************A
AsyncWebServer server(80);

Adafruit_MPU6050 mpu;

const char* ssid = "MIWIFI_xHdm";       
const char* password = "aQrrvrMn";

sensors_event_t a, g, temp;

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


//SETUP DE MOTORES
/*
  pinMode(MOTOR_A, OUTPUT);
  pinMode(MOTOR_B, OUTPUT);
  pinMode(MOTOR_C, OUTPUT);
*/
  // Configuración del canal PWM
  /*
  ledcSetup(intensidadMOTOR_A, pwmFrequency, pwmResolution);
  ledcSetup(intensidadMOTOR_B, pwmFrequency, pwmResolution);
  ledcSetup(intensidadMOTOR_C, pwmFrequency, pwmResolution);

  // Asignación del canal PWM al pin del solenoide
  ledcAttachPin(MOTOR_A, intensidadMOTOR_A);
  ledcAttachPin(MOTOR_B, intensidadMOTOR_B);
  ledcAttachPin(MOTOR_C, intensidadMOTOR_C);
*/
  //motor superior accionado
  server.on("/Sup", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            Serial.println("Solenoide activado con potencia: " + number);
            int intensity = number.toInt();
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });
  
  //motor inferior izquierdo accionado
  server.on("/Inf", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            Serial.println("Solenoide activado con potencia: " + number);
            int intensity = number.toInt();
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  //motor inferior derecho accionado
  server.on("/mNorte", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            Serial.println("Solenoide activado con potencia: " + number);
            int intensity = number.toInt();
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });
    //motor inferior derecho accionado
  server.on("/mSur", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            Serial.println("Solenoide activado con potencia: " + number);
            int intensity = number.toInt();
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mEste", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            Serial.println("Solenoide activado con potencia: " + number);
            int intensity = number.toInt();
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mOeste", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("number")) {
          String number = request->getParam("number")->value();
          Serial.println("Solenoide activado con potencia: " + number);
          int intensity = number.toInt();
          request->send(200, "text/plain", "OK");
    }
    else{
      request->send(200, "text/plain", "ERROR: No number recieved");

    }
});

  //LAUNCH
  server.on("/launch", HTTP_GET, [](AsyncWebServerRequest *request){
    int values[6] = {0};
      if (request->hasParam("values")) {
        for (int i = 0; i < 6; i++) {
              String paramName = "v" + String(i);
              if (request->hasParam(paramName)) {
                values[i] = request->getParam(paramName)->value().toInt();
              }
            }
           // Imprimir los valores recibidos - LOS MANDAMOS A LOS MOTORES CORRESPONDIENTES
        for (int i = 0; i < 6; i++) {
          Serial.print("v");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(values[i]);
        }
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", STATIC_WEB_MAIN);
  });

  server.on("/styles.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", STATIC_WEB_CSS);
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", STATIC_WEB_SCRIPT);
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", "<!DOCTYPE html><html />");
  });

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX = 0;
    gyroY = 0;
    gyroZ = 0;
    request->send(200, "text/plain", "Gyroscope reset successful");
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
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString = JSON.stringify (readings);
  return accString;
}

void activarSolenoide(int intensity, String solenoide) {
    // Lógica para activar el solenoide basado en el número recibido
    Serial.println("Activando solenoide: " + String(solenoide));
    switch (intensity) {
        case 'Sup':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        case 'mNorte':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        case 'mSur':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        case 'mEste':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        case 'mOeste':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        case 'Inf':
              // Configurar el ciclo de trabajo PWM
              ledcWrite(intensidadMOTOR_A, intensity);
              // Mantener el solenoide encendido por un corto periodo
              delay(100); // Ajusta este valor según tus necesidades
              // Apagar el solenoide
              ledcWrite(intensidadMOTOR_A, 0);
              // Aquí va el código para controlar el solenoide
            break;
        default:
            Serial.println("Número de solenoide inválido");
            break;
    }
}

void loop() {

   mpu.getEvent(&a, &g, &temp);

  if ((millis() - lastTime) > gyroDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getGyroscope().c_str(),"gyro_readings",millis());
    lastTime = millis();
  }
  if ((millis() - lastTimeAcc) > accelerometerDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getAccelerometer().c_str(),"accelerometer_readings",millis());
    events.send(getAngularRotation().c_str(),"combined_reading",millis());
    lastTimeAcc = millis();
  }

}