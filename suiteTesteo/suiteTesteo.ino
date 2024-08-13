
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <math.h>
#include "StaticWebs.h"


// Pines de control del solenoide 1 Top
#define aENA 27
#define aIN1 12
#define aIN2 14

// Pines de control del solenoide 2 Inf
#define aENB 13
#define aIN3 25
#define aIN4 26

#define MOTOR_T 12 //Top
#define MOTOR_I 13  //Inf
#define MOTOR_N 14  //Norte
#define MOTOR_S 15 //Sur
#define MOTOR_E 16//Este
#define MOTOR_O 17//Oeste

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


const char* ssid = "hotspotesp32";       
const char* password = "haptiball";
/*
const char* ssid = "MIWIFI_xHdm";       
const char* password = "aQrrvrMn";
*/
  

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
    Serial.println("."); 
    } 
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());      

  pinMode(aENA, OUTPUT);
  pinMode(aIN1, OUTPUT);
  pinMode(aIN2, LOW); // Asegura que IN2 empieza en LOW, para evitar que el solenoide se active al inicio

  pinMode(aENB, OUTPUT);
  pinMode(aIN3, OUTPUT);
  pinMode(aIN4, LOW); // Asegura que IN2 empieza en LOW, para evitar que el solenoide se active al inicio


  //motor superior accionado
  server.on("/Sup", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            int valor = number.toInt();
            //Calculamos la intensidad dado el valor
            int power = calcularPotencia(valor);
            Serial.println("Solenoide superior activado con potencia: " + String(power));
            digitalWrite(aIN1, HIGH);
            digitalWrite(aIN2, LOW);
            analogWrite(aENA, power);
            delay(150);
            // Desactivar el solenoide
            // ambos pines en LOW significa que el solenoide está desactivado
            digitalWrite(aIN1, LOW);
            digitalWrite(aIN2, LOW);

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
            int valor = number.toInt();
            //Calculamos la intensidad dado el valor
            int power = calcularPotencia(valor);
            Serial.println("Solenoide inferior activado con potencia: " + String(power));
            digitalWrite(aIN3, HIGH);
            digitalWrite(aIN4, LOW);
            analogWrite(aENB, power);
            delay(150);
            // Desactivar el solenoide
            // ambos pines en LOW significa que el solenoide está desactivado
            digitalWrite(aIN3, LOW);
            digitalWrite(aIN4, LOW);
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
            int valor = number.toInt();
            int power = calcularPotencia(valor);
            Serial.println("Solenoide norte activado con potencia: " + String(power));
            //Incluir pines correspondientes
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
            int valor = number.toInt();
            int power = calcularPotencia(valor);
            Serial.println("Solenoide sur activado con potencia: " + String(power));        
            //Incluir pines correspondientes
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mEste", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            String number = request->getParam("number")->value();
            int valor = number.toInt();
            int power = calcularPotencia(valor);
            Serial.println("Solenoide este activado con potencia: " + String(power));
            //Incluir pines correspondientes
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mOeste", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("number")) {
          String number = request->getParam("number")->value();
          int valor = number.toInt();
          int power = calcularPotencia(valor);
          Serial.println("Solenoide oeste activado con potencia: " + String(power));
          //Incluir pines correspondientes
          request->send(200, "text/plain", "OK");
    }
    else{
      request->send(200, "text/plain", "ERROR: No number recieved");

    }
});

//LAUNCH
server.on("/launch", HTTP_GET, [](AsyncWebServerRequest *request){
  int values[6] = {0}; 
  String valorParam;

  if (request->hasParam("values")) {
    valorParam = request->getParam("values")->value(); 

    // Dividimos la cadena en los valores individuales
    int lastIndex = 0;
    for (int i = 0; i < 6; i++) {
      int commaIndex = valorParam.indexOf(',', lastIndex);
      if (commaIndex == -1) { // Si es el último valor (sin coma al final)
        commaIndex = valorParam.length();
      }
      String valueString = valorParam.substring(lastIndex, commaIndex);
      values[i] = calcularPotencia(valueString.toInt()); // Convertimos y calculamos

      lastIndex = commaIndex + 1; // Movemos al siguiente valor
    }
      //ACTIVAR MOTORES NORMALMENTE
      //activateMotors(values);
      Serial.println("***** LAUNCH*****");
      for (int i = 0; i < 6; i++) {
        Serial.println("Solenoide " + String(i) + " activado con el valor " + String(values[i]));
     
    }

    request->send(200, "text/plain", "OK");
  } else {
    request->send(400, "text/plain", "ERROR: No values received");
  }
});


//Custom launch
server.on("/customLaunch", HTTP_GET, [](AsyncWebServerRequest *request){
  int values[6] = {0}; 
  String valorParam;
  int actTime;
  int waitTime;
  int repsInput;
    if (request->hasParam("actTime")) {
    Serial.println("actTime parameter received");
    Serial.println(request->getParam("actTime")->value()); 
    actTime = request->getParam("actTime")->value().toInt(); 
  }

  if (request->hasParam("waitTime")) {
    Serial.println("waitTime parameter received");
    Serial.println(request->getParam("waitTime")->value()); 
    waitTime = request->getParam("waitTime")->value().toInt(); 
  }

  if (request->hasParam("repsInput")) {
    Serial.println("repsInput parameter received");
    Serial.println(request->getParam("repsInput")->value()); 
    repsInput = request->getParam("repsInput")->value().toInt(); 
  }
  if (request->hasParam("values")) {
    valorParam = request->getParam("values")->value(); 
    Serial.println(request->getParam("values")->value()); 
    // Dividimos la cadena en los valores individuales
    int lastIndex = 0;
    for (int i = 0; i < 6; i++) {
      int commaIndex = valorParam.indexOf(',', lastIndex);
      if (commaIndex == -1) { // Si es el último valor (sin coma al final)
        commaIndex = valorParam.length();
      }
      String valueString = valorParam.substring(lastIndex, commaIndex);
      values[i] = calcularPotencia(valueString.toInt()); // Convertimos y calculamos

      lastIndex = commaIndex + 1; // Movemos al siguiente valor
    }

    // ACTIVAR MOTORES CON CUSTOM HIT
    //activateMotorsCustomHit(values,repsInput,waitTime,actTime);
    for (int r = 0; r < repsInput; r++) {
      Serial.println("*****CUSTOM LAUNCH*****");
      for (int i = 0; i < 6; i++) {
        Serial.println("Solenoide " + String(i) + " activado con el valor " + String(values[i]));
        delay(actTime);
        Serial.println("timeActivated:"+String(actTime));
    }
    delay(waitTime);
    Serial.println("delay:"+String(waitTime));
    

    }
    request->send(200, "text/plain", "OK");
  } else {
    request->send(400, "text/plain", "ERROR: No values received");
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
    request->send_P(200, "text/html", "<!DOCTYPE <html></html>");
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

int calcularPotencia(int valor){
  if (valor==0){
    return 0;
  }
  else if (valor==10){
    return 255;
  }
  else{
    int res;
    return res = round(11.11 * (valor - 1) + 155);
    
  }

}

int calcularLedcFadeDuration(int valor){
  if (valor==0){
    return 0;
  }
  else if (valor==10){
    return 500;
  }
  else{
    int res;
    return res = 2000-((1500*(valor-1)/9));
  }

}

void activateMotors(int values[]){

   // Motor 1
   /*
  if (values[0] > 0) {
    digitalWrite(aIN1, HIGH);
    digitalWrite(aIN2, LOW);
    analogWrite(aENB1, values[0]);
  } else {
    digitalWrite(aIN1, LOW);
    digitalWrite(aIN2, LOW);
  }

  // Motor 2
  if (values[1] > 0) {
    digitalWrite(aIN3, HIGH);
    digitalWrite(aIN4, LOW);
    analogWrite(aENB2, values[1]);
  } else {
    digitalWrite(aIN3, LOW);
    digitalWrite(aIN4, LOW);
  }

  // Motor 3
  if (values[2] > 0) {
    digitalWrite(aIN5, HIGH);
    digitalWrite(aIN6, LOW);
    analogWrite(aENB3, values[2]);
  } else {
    digitalWrite(aIN5, LOW);
    digitalWrite(aIN6, LOW);
  }

  // Motor 4
  if (values[3] > 0) {
    digitalWrite(aIN7, HIGH);
    digitalWrite(aIN8, LOW);
    analogWrite(aENB4, values[3]);
  } else {
    digitalWrite(aIN7, LOW);
    digitalWrite(aIN8, LOW);
  }

  // Motor 5
  if (values[4] > 0) {
    digitalWrite(aIN9, HIGH);
    digitalWrite(aIN10, LOW);
    analogWrite(aENB5, values[4]);
  } else {
    digitalWrite(aIN9, LOW);
    digitalWrite(aIN10, LOW);
  }

  // Motor 6
  if (values[5] > 0) {
    digitalWrite(aIN11, HIGH);
    digitalWrite(aIN12, LOW);
    analogWrite(aENB6, values[5]);
  } else {
    digitalWrite(aIN11, LOW);
    digitalWrite(aIN12, LOW);
  }

  // Esperar 150 ms
  delay(150);

  // Desactivar todos los motores
  digitalWrite(aIN1, LOW);
  digitalWrite(aIN2, LOW);
  digitalWrite(aIN3, LOW);
  digitalWrite(aIN4, LOW);
  digitalWrite(aIN5, LOW);
  digitalWrite(aIN6, LOW);
  digitalWrite(aIN7, LOW);
  digitalWrite(aIN8, LOW);
  digitalWrite(aIN9, LOW);
  digitalWrite(aIN10, LOW);
  digitalWrite(aIN11, LOW);
  digitalWrite(aIN12, LOW);
}
*/
}

void activateMotorsCustomHit(int values[],int waitTime,int actTime,int reps){
   /*for (int r = 0; r < reps; r++) {
 
    // Motor 1
    if (values[0] > 0) {
      digitalWrite(aIN1, HIGH);
      digitalWrite(aIN2, LOW);
      analogWrite(aENB1, values[0]);
    } else {
      digitalWrite(aIN1, LOW);
      digitalWrite(aIN2, LOW);
    }

    // Motor 2
    if (values[1] > 0) {
      digitalWrite(aIN3, HIGH);
      digitalWrite(aIN4, LOW);
      analogWrite(aENB2, values[1]);
    } else {
      digitalWrite(aIN3, LOW);
      digitalWrite(aIN4, LOW);
    }

    // Motor 3
    if (values[2] > 0) {
      digitalWrite(aIN5, HIGH);
      digitalWrite(aIN6, LOW);
      analogWrite(aENB3, values[2]);
    } else {
      digitalWrite(aIN5, LOW);
      digitalWrite(aIN6, LOW);
    }

    // Motor 4
    if (values[3] > 0) {
      digitalWrite(aIN7, HIGH);
      digitalWrite(aIN8, LOW);
      analogWrite(aENB4, values[3]);
    } else {
      digitalWrite(aIN7, LOW);
      digitalWrite(aIN8, LOW);
    }

    // Motor 5
    if (values[4] > 0) {
      digitalWrite(aIN9, HIGH);
      digitalWrite(aIN10, LOW);
      analogWrite(aENB5, values[4]);
    } else {
      digitalWrite(aIN9, LOW);
      digitalWrite(aIN10, LOW);
    }

    // Motor 6
    if (values[5] > 0) {
      digitalWrite(aIN11, HIGH);
      digitalWrite(aIN12, LOW);
      analogWrite(aENB6, values[5]);
    } else {
      digitalWrite(aIN11, LOW);
      digitalWrite(aIN12, LOW);
    }

    // Esperar el tiempo de activación
    delay(actTime);

    // Desactivar todos los motores
    digitalWrite(aIN1, LOW);
    digitalWrite(aIN2, LOW);
    digitalWrite(aIN3, LOW);
    digitalWrite(aIN4, LOW);
    digitalWrite(aIN5, LOW);
    digitalWrite(aIN6, LOW);
    digitalWrite(aIN7, LOW);
    digitalWrite(aIN8, LOW);
    digitalWrite(aIN9, LOW);
    digitalWrite(aIN10, LOW);
    digitalWrite(aIN11, LOW);
    digitalWrite(aIN12, LOW);

    // Esperar el tiempo entre repeticiones
    delay(waitTime);
  }*/
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