#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <math.h>
#include "StaticWebs.h"
#include <esp_now.h>

// ESTRUCTURA PINES INDIVIDUALES
#define IN1a 12
#define IN2a 14
#define IN3a 27
#define IN4a 26

#define IN1b 32
#define IN2b 15
#define IN3b 2
#define IN4b 4

#define IN1c 18
#define IN2c 19
//#define IN3c 3
//#define IN4c 17 


// Pines de cada motor, se utilizan  para mandar la señal PWM
//Motor sup
#define aENA 13

//Motor Inf
#define aENB 25

//Motor Norte
#define bENA 33

//Motor sur
#define bENB 16

//Motor este
#define cENA 23
String solenoidNames[] = {"Superior", "Inferior", "Norte", "Sur", "Este", "Oeste"};

//Motor oeste
//#define cENB 5

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTimeGyro = 0;  
unsigned long lastTime = 0;     
unsigned long lastTimeAcc = 0;
unsigned long lastTimeMixed = 0;
unsigned long gyroDelay = 10;
unsigned long accelerometerDelay = 200;
unsigned long mixedReadingDelay = 10;

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;

//Gyroscope sensor deviation
float gyroXerror = 0.07; 
float gyroYerror = 0.1;
float gyroZerror = 0.1; 


//****************************************************A
AsyncWebServer server(80);

Adafruit_MPU6050 mpu;

/*
const char* ssid = "hotspotesp32";       
const char* password = "haptiball";
*/

const char* ssid = "MIWIFI_xHdm";       
const char* password = "aQrrvrMn";

sensors_event_t a, g, temp;

void setup(void) {

  Serial.begin(115200);
  while (!Serial)
    delay(10);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");


  //Configuración WIFI
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
  
  //Setup motores
  pinMode(aENA, OUTPUT);
  pinMode(aENB, OUTPUT);
  pinMode(bENA, OUTPUT);
  pinMode(bENB, OUTPUT);
  pinMode(cENA, OUTPUT);


  pinMode(IN1a, OUTPUT);
  pinMode(IN2a, LOW); 
  pinMode(IN3a, OUTPUT);
  pinMode(IN4a, LOW); 

  pinMode(IN1b, OUTPUT);
  pinMode(IN2b, LOW); 
  pinMode(IN3b, OUTPUT);
  pinMode(IN4b, LOW); 

  pinMode(IN1c, OUTPUT);
  pinMode(IN2c, LOW); 


  //motor superior accionado
  server.on("/Sup", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
        int actTime;
        int waitTime;
        int repsInput;
        String number = request->getParam("number")->value();
        int valor = number.toInt();
        //Calculamos la intensidad dado el valor
        int power = calcularPotencia(valor);

        if (request->hasParam("actTime")) {
          actTime = request->getParam("actTime")->value().toInt(); 
          waitTime = request->getParam("waitTime")->value().toInt(); 
          repsInput = request->getParam("repsInput")->value().toInt(); 
          Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
        }
        else{
          repsInput = 1;
          actTime = 150;
          waitTime = 0;
        }
        for (int r = 0; r < repsInput; r++) {
          Serial.println(String(r));
          Serial.println("Solenoide superior activado con potencia: " + String(power));
          digitalWrite(IN1a, HIGH);
          digitalWrite(IN2a, LOW);
          analogWrite(aENA, power);
          delay(actTime);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN1a, LOW);
          digitalWrite(IN2a, LOW);
          delay(waitTime);
          
        }
        request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });
  
  //motor inferior izquierdo accionado
  server.on("/Inf", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
        int actTime;
        int waitTime;
        int repsInput;
        String number = request->getParam("number")->value();
        int valor = number.toInt();
        //Calculamos la intensidad dado el valor
        int power = calcularPotencia(valor);
        if (request->hasParam("actTime")) {
          actTime = request->getParam("actTime")->value().toInt(); 
          waitTime = request->getParam("waitTime")->value().toInt(); 
          repsInput = request->getParam("repsInput")->value().toInt(); 
          Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
        }
        else{
          repsInput = 1;
          actTime = 150;
          waitTime = 0;
        }
         for (int r = 0; r < repsInput; r++) {
          Serial.println("Solenoide inferior activado con potencia: " + String(power));
          digitalWrite(IN3a, HIGH);
          digitalWrite(IN4a, LOW);
          analogWrite(aENB, power);
          delay(actTime);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN3a, LOW);
          digitalWrite(IN4a, LOW);
          delay(waitTime);
        }
        request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  //motor inferior derecho accionado
  server.on("/mNorte", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            int actTime;
            int waitTime;
            int repsInput;
            String number = request->getParam("number")->value();
            int valor = number.toInt();
            int power = calcularPotencia(valor);
            if (request->hasParam("actTime")) {
              actTime = request->getParam("actTime")->value().toInt(); 
              waitTime = request->getParam("waitTime")->value().toInt(); 
              repsInput = request->getParam("repsInput")->value().toInt(); 
              Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
            }
            else{
              repsInput = 1;
              actTime = 150;
              waitTime = 0;
            }
            for (int r = 0; r < repsInput; r++) {
              Serial.println("Solenoide norte activado con potencia: " + String(power));
              digitalWrite(IN1b, HIGH);
              digitalWrite(IN2b, LOW);
              analogWrite(bENA, power);
              delay(actTime);
              // Desactivar el solenoide
              // ambos pines en LOW significa que el solenoide está desactivado
              digitalWrite(IN1b, LOW);
              digitalWrite(IN2b, LOW);
              delay(waitTime);
            }
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });
    //motor inferior derecho accionado
  server.on("/mSur", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            int actTime;
            int waitTime;
            int repsInput;
            String number = request->getParam("number")->value();
            int valor = number.toInt();
            int power = calcularPotencia(valor);

            if (request->hasParam("actTime")) {
              actTime = request->getParam("actTime")->value().toInt(); 
              waitTime = request->getParam("waitTime")->value().toInt(); 
              repsInput = request->getParam("repsInput")->value().toInt(); 
              Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
            }
            else{
              repsInput = 1;
              actTime = 150;
              waitTime = 0;
            }
            for (int r = 0; r < repsInput; r++) {
              Serial.println("Solenoide sur activado con potencia: " + String(power));        
              //Incluir pines correspondientes
              digitalWrite(IN3b, HIGH);
              digitalWrite(IN4b, LOW);
              analogWrite(bENB, power);
              delay(actTime);
              // Desactivar el solenoide
              // ambos pines en LOW significa que el solenoide está desactivado
              digitalWrite(IN3b, LOW);
              digitalWrite(IN4b, LOW);
              delay(waitTime);
            }
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mEste", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("number")) {
            int actTime;
            int waitTime;
            int repsInput;
            String number = request->getParam("number")->value();
            int valor = number.toInt();
            int power = calcularPotencia(valor);

            if (request->hasParam("actTime")) {
              actTime = request->getParam("actTime")->value().toInt(); 
              waitTime = request->getParam("waitTime")->value().toInt(); 
              repsInput = request->getParam("repsInput")->value().toInt(); 
              Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
            }
            else{
              repsInput = 1;
              actTime = 150;
              waitTime = 0;
            }
            for (int r = 0; r < repsInput; r++) {
              Serial.println("Solenoide este activado con potencia: " + String(power));
              //Incluir pines correspondientes
              digitalWrite(IN1c, HIGH);
              digitalWrite(IN2c, LOW);
              analogWrite(cENA, power);
              delay(actTime);
              // Desactivar el solenoide
              // ambos pines en LOW significa que el solenoide está desactivado
              digitalWrite(IN1c, LOW);
              digitalWrite(IN2c, LOW);
              delay(waitTime);
            }
            request->send(200, "text/plain", "OK");
      }
      else{
        request->send(200, "text/plain", "ERROR: No number recieved");

      }
  });

  server.on("/mOeste", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("number")) {
          int actTime;
          int waitTime;
          int repsInput;
          String number = request->getParam("number")->value();
          int valor = number.toInt();
          int power = calcularPotencia(valor);
          if (request->hasParam("actTime")) {
            actTime = request->getParam("actTime")->value().toInt(); 
            waitTime = request->getParam("waitTime")->value().toInt(); 
            repsInput = request->getParam("repsInput")->value().toInt(); 
            Serial.println("Valores recibidos: ActTime: "+request->getParam("actTime")->value()+ "WaitTime: "+ request->getParam("waitTime")->value()+ "Reps: "+request->getParam("repsInput")->value());
          }
          else{
            repsInput = 1;
            actTime = 150;
            waitTime = 0;
          }
          for (int r = 0; r < repsInput; r++) {
            Serial.println("Solenoide oeste activado con potencia: " + String(power));
            //Incluir pines correspondientes
            /*
            digitalWrite(IN3c, HIGH);
            digitalWrite(IN4c, LOW);
            analogWrite(cENB, power);
            delay(actTime);
            // Desactivar el solenoide
            // ambos pines en LOW significa que el solenoide está desactivado
            digitalWrite(IN3c, LOW);
            digitalWrite(IN4c, LOW);*/
            delay(waitTime);
          }
          
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
      activateMotors(values);
      Serial.println("***** LAUNCH*****");
      for (int i = 0; i < 6; i++) {
        Serial.println("Solenoide "  + solenoidNames[i] + " activado con el valor " + String(values[i]));
     
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

    // ACTIVAR MOTORES CON CUSTOM HIT repsInput waitTime actTime
    activateMotorsCustomHit(values,waitTime,actTime,repsInput);
    for (int r = 0; r < repsInput; r++) {
      Serial.println("*****CUSTOM LAUNCH*****");
      for (int i = 0; i < 6; i++) {
        Serial.println("Solenoide "  + solenoidNames[i] + " activado con el valor " + String(values[i]));
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


//Lectura  con dt
String getGyroscope() {
    mpu.getEvent(&a, &g, &temp);

    // Obtener el tiempo actual y calcular dt
    unsigned long currentTime = millis();
    float dt = (currentTime - lastTimeGyro) / 1000.0;  // Convertir a segundos
    lastTimeGyro = currentTime;

    // Integrar la velocidad angular en función de dt
    float gyroX_temp = g.gyro.x;
    if (abs(gyroX_temp) > gyroXerror) {
        gyroX += gyroX_temp * dt;
    }

    float gyroY_temp = g.gyro.y;
    if (abs(gyroY_temp) > gyroYerror) {
        gyroY += gyroY_temp * dt;
    }

    float gyroZ_temp = g.gyro.z;
    if (abs(gyroZ_temp) > gyroZerror) {
        gyroZ += gyroZ_temp * dt;
    }

    // Almacenar las lecturas en el objeto readings
    readings["gyroX"] = String(gyroX);
    readings["gyroY"] = String(gyroY);
    readings["gyroZ"] = String(gyroZ);

    // Convertir readings a un JSON y retornarlo
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

//values=[sup,inf,norte,sur,este,oeste];
void activateMotors(int values[]){

   // Motor 1 sup
  if (values[0] > 0) {
    digitalWrite(IN1a, HIGH);
    digitalWrite(IN2a, LOW);
    analogWrite(aENA, values[0]);
  } 

  // Motor 2 inf
  if (values[1] > 0) {
    digitalWrite(IN3a, HIGH);
    digitalWrite(IN4a, LOW);
    analogWrite(aENB, values[1]);
  } 
  // Motor 3 norte
  if (values[2] > 0) {
    digitalWrite(IN1b, HIGH);
    digitalWrite(IN2b, LOW);
    analogWrite(bENA, values[2]);
  } 

  // Motor 4 sur
  if (values[3] > 0) {
    digitalWrite(IN3b, HIGH);
    digitalWrite(IN4b, LOW);
    analogWrite(bENB, values[3]);
  } 

  // Motor 5 este
  if (values[4] > 0) {
    digitalWrite(IN1c, HIGH);
    digitalWrite(IN2c, LOW);
    analogWrite(cENA, values[4]);
  } 

  // Motor 6 oeste
  if (values[5] > 0) {
    Serial.println("LAUNCH OESTE POTENCIA: ["+ String(values[5]) +"]"); 
  } 

  // Esperar 150 ms
  delay(150);

  // Desactivar todos los motores
  digitalWrite(IN1a, LOW);
  digitalWrite(IN2a, LOW);
  digitalWrite(IN3a, LOW);
  digitalWrite(IN4a, LOW);

  digitalWrite(IN1b, LOW);
  digitalWrite(IN2b, LOW);
  digitalWrite(IN3b, LOW);
  digitalWrite(IN4b, LOW);

  digitalWrite(IN1c, LOW);
  digitalWrite(IN2c, LOW);

}



void activateMotorsCustomHit(int values[],int waitTime,int actTime,int reps){
  Serial.println("ACTIVATED-> Reps: "+String(reps)+ "WaitTime: "+ String(waitTime)+ "ActTime: "+String(actTime)); 
   for (int r = 0; r < reps; r++) {
 
    // Motor 1 sup
    if (values[0] > 0) {
      digitalWrite(IN1a, HIGH);
      digitalWrite(IN2a, LOW);
      analogWrite(aENA, values[0]);

      delay(actTime);

      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, LOW);

    } 

    // Motor 2 inf
    if (values[1] > 0) {
      digitalWrite(IN3a, HIGH);
      digitalWrite(IN4a, LOW);
      analogWrite(aENB, values[1]);

      delay(actTime);

      digitalWrite(IN1a, LOW);
      digitalWrite(IN2a, LOW);
    } 
    // Motor 3 norte
    if (values[2] > 0) {
      digitalWrite(IN1b, HIGH);
      digitalWrite(IN2b, LOW);
      analogWrite(bENA, values[2]);

      delay(actTime);

      digitalWrite(IN1b, LOW);
      digitalWrite(IN2b, LOW);
    } 

    // Motor 4 sur
    if (values[3] > 0) {
      digitalWrite(IN3b, HIGH);
      digitalWrite(IN4b, LOW);
      analogWrite(bENB, values[3]);

      delay(actTime);


      digitalWrite(IN3b, LOW);
      digitalWrite(IN4b, LOW);
    } 

    // Motor 5 este
    if (values[4] > 0) {
      digitalWrite(IN1c, HIGH);
      digitalWrite(IN2c, LOW);
      analogWrite(cENA, values[4]);

      delay(actTime);

      digitalWrite(IN1c, LOW);
      digitalWrite(IN2c, LOW);
    } 

    // Motor 6 oeste
    if (values[5] > 0) {
       Serial.println("LAUNCH OESTE POTENCIA: ["+ String(values[5]) +"]"); 

      delay(actTime);
    } 


    // Esperar el tiempo entre repeticiones
    delay(waitTime);
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
    lastTimeAcc = millis();
  }
  if ((millis() - lastTimeMixed) > mixedReadingDelay) {
    // Send Events to the Web Server with the Sensor Readings
    events.send(getAngularRotation().c_str(),"combined_reading",millis());
    lastTimeMixed = millis();
  }

  

}