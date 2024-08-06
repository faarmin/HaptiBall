#include <Arduino.h>

#define ENA 27
#define IN1 14
#define IN2 12

#define ENB 13
#define IN3 15
#define IN4 26

void setup() {
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, LOW); // Asegura que IN2 empieza en LOW

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, LOW); // Asegura que IN2 empieza en LOW

  Serial.println("Solenoide listo");
  Serial.println("Introduce 5 numeros enteros separados por un espacio: solenoidID(1-2) reps waitTime fuerza(1-254) duracion(1-1000)\nPor ejemplo: 1 3 50 255 30");
}

void loop() {
  if (Serial.available()) {
    int solenoidID = Serial.parseInt();
    int reps = Serial.parseInt();
    int waitTime = Serial.parseInt();
    int fuerza = Serial.parseInt();
    int tiempo = Serial.parseInt();
    Serial.print("Solenoid: ");
    Serial.print(solenoidID);
    Serial.print(" | reps: ");
    Serial.print(reps);
    Serial.print(" | waitTime: ");
    Serial.print(waitTime);
    Serial.print(" | Fuerza: ");
    Serial.print(fuerza);
    Serial.print(" | Tiempo: ");
    Serial.println(tiempo);
    // Limitar la fuerza a un rango seguro (0-255)
    fuerza = constrain(fuerza, 0, 255);

    int actualEN;
    int actualIN_first;
    int actualIN_second;
    if (solenoidID==1){
      actualEN = ENA;
      actualIN_first = IN1;
      actualIN_second = IN2;
    }else{
      actualEN = ENB;
      actualIN_first = IN3;
      actualIN_second = IN4;
    }

    for (int i = 1; i <= reps; i++) {
      Serial.println(i);
       // Controlar la dirección y fuerza del solenoide
      digitalWrite(actualIN_first, HIGH);
      digitalWrite(actualIN_second, LOW); 
      analogWrite(actualEN, fuerza);

      // Mantener el solenoide activado durante el tiempo especificado
      delay(tiempo);

      // Desactivar el solenoide
      digitalWrite(actualIN_first, LOW);
      digitalWrite(actualIN_second, LOW);

      Serial.println("Solenoide desactivado");
      delay(waitTime);

    }
     
   
    
  }
}
