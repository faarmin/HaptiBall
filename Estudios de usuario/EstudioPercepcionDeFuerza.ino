/* TEST PARA ESTUDIOS DE USUARIO: SE ACTIVAN SOLENOIDES SEGUN LOS ESTIMULOS DEFINIDOS EN LA MEMORIA DE MANERA ALEATORIA
    SE IMPRIME POR PANTALLA EL ESTIMULO Y LA POTENCIA UTILIZADOS PARA LA POSTERIOR RECOPILACION DE LOS DATOS
*/

// ESTRUCTURA PINES INDIVIDUALES
#define IN1a 12
#define IN2a 14
#define IN3a 27
#define IN4a 26

//Motor sup
#define aENA 13

//Motor Inf
#define aENB 25


void Estimulo1 (int power){
//Un hit breve 
          
          digitalWrite(IN1a, HIGH);
          digitalWrite(IN2a, LOW);
          analogWrite(aENA, power);
          delay(50);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN1a, LOW);
          digitalWrite(IN2a, LOW);
          delay(1000);
          Serial.println("Estimulo1 activado con potencia: " + String(power));
}

void Estimulo2 (int power){
//Un hit largo 
          
          digitalWrite(IN1a, HIGH);
          digitalWrite(IN2a, LOW);
          analogWrite(aENA, power);
          delay(500);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN1a, LOW);
          digitalWrite(IN2a, LOW);
          delay(1000);
          Serial.println("Estimulo2 activado con potencia: " + String(power));
}

void Estimulo3 (int power){
//2 hits breves en la misma dirección 

for (int r = 0; r < 2; r++) {
          digitalWrite(IN1a, HIGH);
          digitalWrite(IN2a, LOW);
          analogWrite(aENA, power);
          delay(30);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN1a, LOW);
          digitalWrite(IN2a, LOW);
          delay(150);
          
        }
        delay(1000);
        Serial.println("Estimulo3 activado con potencia: " + String(power));
}

void Estimulo4 (int power){
//3 hits breves en la misma dirección.

  for (int r = 0; r < 3; r++) {
          digitalWrite(IN1a, HIGH);
          digitalWrite(IN2a, LOW);
          analogWrite(aENA, power);
          delay(30);
          // Desactivar el solenoide
          // ambos pines en LOW significa que el solenoide está desactivado
          digitalWrite(IN1a, LOW);
          digitalWrite(IN2a, LOW);
          delay(150);
          
        }
        delay(1000);
        Serial.println("Estimulo4 activado con potencia: " + String(power));
}

void Estimulo5 (int power){
//Un hit suave en en una dirección y un hit fuerte en la opuesta.
 
  int lowPower = power*0.66;
  int highPower = power*1.33;
  if (highPower > 255){
    highPower = 255;
  }
  if (lowPower < 155){
    lowPower = 155;
  }

//hitsuave
  digitalWrite(IN3a, HIGH);
  digitalWrite(IN4a, LOW);
  analogWrite(aENB, lowPower);
  delay(150);
  // Desactivar el solenoide
  // ambos pines en LOW significa que el solenoide está desactivado
  digitalWrite(IN3a, LOW);
  digitalWrite(IN4a, LOW);
  delay(150);
//Hit fuerte
  digitalWrite(IN1a, HIGH);
  digitalWrite(IN2a, LOW);
  analogWrite(aENA, power);
  delay(150);
  // Desactivar el solenoide
  // ambos pines en LOW significa que el solenoide está desactivado
  digitalWrite(IN1a, LOW);
  digitalWrite(IN2a, LOW);
  delay(1000);
  Serial.println("Estimulo5 activado con potencia: " + String(power));
}

typedef void (*MotorFunction)(int);
MotorFunction funciones[] = {Estimulo1, Estimulo2, Estimulo3, Estimulo4, Estimulo5};

int potencias[] = {155, 199, 255};

struct Combinacion {
    MotorFunction funcion;
    int potencia;
};

Combinacion combinaciones[15]; 

void setup() {
  Serial.begin(115200);
  //Setup motores
  pinMode(aENA, OUTPUT);
  pinMode(aENB, OUTPUT);

  pinMode(IN1a, OUTPUT);
  pinMode(IN2a, LOW); 
  pinMode(IN3a, OUTPUT);
  pinMode(IN4a, LOW); 

  randomSeed(analogRead(A0));
  //Crear todas las combinaciones
  int index = 0;
  for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 3; j++) {
          combinaciones[index].funcion = funciones[i];
          combinaciones[index].potencia = potencias[j];
          index++;
      }
  }

  //Aleatorizar el array de combinaciones usando Fisher-Yates
  for (int i = 14; i > 0; i--) {
      int j = random(0, i + 1);
      Combinacion temp = combinaciones[i];
      combinaciones[i] = combinaciones[j];
      combinaciones[j] = temp;
  }

}

void loop() {
  delay(5000);
  Serial.println("**************TEST BEGIN*********************");
  for (int i = 0; i < 15; i++) {
        combinaciones[i].funcion(combinaciones[i].potencia);
        delay(1000); 
    }
  Serial.println("**************TEST END*********************");
  while (true) {
    }
}



