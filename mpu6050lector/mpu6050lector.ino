
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WebServer.h>
#include "html.h"
#include <math.h>


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

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");                
 
  delay(1000);  

}


void loop() {

  mpu.getEvent(&a, &g, &temp);

  
  //ACELERACION 
  Print out the values  
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  
  
  //INCLINACION SEGÚN VELOCIDAD ANG

  
  float accel_ang_x=atan(a.acceleration.x/sqrt(pow(a.acceleration.y,2) + pow(a.acceleration.z,2)))*(180.0/3.14);
  float accel_ang_y=atan(a.acceleration.y/sqrt(pow(a.acceleration.x,2) + pow(a.acceleration.z,2)))*(180.0/3.14);
  Serial.print("Inclinacion en X: ");
  Serial.println(accel_ang_x); 
  Serial.print("Inclinacion en Y: ");
  Serial.println(accel_ang_y);

//ROTACION (velocidad angular en radianes/s)
/
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
*/

//ANGULOS DE ROTACION (comentar angulo de rotacion)

  long tiempo_prev, dt;
  float girosc_ang_x, girosc_ang_y;
  float girosc_ang_x_prev, girosc_ang_y_prev;

  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  
  girosc_ang_x = (g.gyro.x/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (g.gyro.y/131)*dt/1000.0 + girosc_ang_y_prev;
  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;  
  Serial.print("Rotacion angular en X:  ");
  Serial.println(girosc_ang_x); 
  Serial.print("Rotacion angular en Y: ");
  Serial.println(girosc_ang_y);
*/
//ANGULO DE ROTACION CON GIROSCOPIO Y FILTRO COMPLEMENTO (comentar angulos de rotacion)
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

  Serial.print("Rotacion en X:  ");
  Serial.println(ang_x* (180/M_PI)); 
  Serial.print("Rotacion en Y: ");
  Serial.println(ang_y* (180/M_PI));


  Serial.println("");
  
  server.handleClient();
  delay(1000);
}