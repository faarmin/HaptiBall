/* TEST PARA ESTUDIOS DE USUARIO: SE ACTIVAN SOLENOIDES SEGUN LA DIRECCION DESEADA DE MANERA ALEATORIA
    SE IMPRIME POR PANTALLA LA DIRECCION PARA POSTERIOR RECOPILACION DE LOS DATOS
*/

// ESTRUCTURA PINES INDIVIDUALES
#define IN1a 12
#define IN2a 14
#define IN3a 27
#define IN4a 26

#define IN1b 32
#define IN2b 15
#define IN3b 2
#define IN4b 4

//Motor sup
#define aENA 13

//Motor Inf
#define aENB 25

//Motor Norte
#define bENA 33

//Motor sur
#define bENB 16



void haciaAlante (){
  Serial.println("haciaAlante activado");
  digitalWrite(IN1b, HIGH);
  digitalWrite(IN2b, LOW);
  analogWrite(bENA, 255);
  delay(150);
  digitalWrite(IN1b, LOW);
  digitalWrite(IN2b, LOW);
  delay(1000);
}

void haciaAtras (){
  Serial.println("haciaAtras activado");
  digitalWrite(IN3b, HIGH);
  digitalWrite(IN4b, LOW);
  analogWrite(bENB, 255);
  delay(150);
  digitalWrite(IN3b, LOW);
  digitalWrite(IN4b, LOW);
  delay(1000);
}

void haciaArriba (){
  Serial.println("haciaArriba activado");
  digitalWrite(IN1a, HIGH);
  digitalWrite(IN2a, LOW);
  analogWrite(aENA, 255);
  delay(150);
  digitalWrite(IN1a, LOW);
  digitalWrite(IN2a, LOW);
  delay(1000);
}

void haciaAbajo (){
  Serial.println("haciaAbajo activado");
  digitalWrite(IN3a, HIGH);
  digitalWrite(IN4a, LOW);
  analogWrite(aENB, 255);
  delay(150);
  digitalWrite(IN3a, LOW);
  digitalWrite(IN4a, LOW);
  delay(1000);
}


typedef void (*MotorFunction)();
MotorFunction funciones[] = {haciaAbajo, haciaArriba, haciaAtras, haciaAlante};
MotorFunction combinaciones[12];

void setup() {
  Serial.begin(115200);
  //Setup motores
  pinMode(aENA, OUTPUT);
  pinMode(aENB, OUTPUT);
  pinMode(bENA, OUTPUT);
  pinMode(bENB, OUTPUT);
 
  pinMode(IN1a, OUTPUT);
  pinMode(IN2a, LOW); 
  pinMode(IN3a, OUTPUT);
  pinMode(IN4a, LOW); 

  pinMode(IN1b, OUTPUT);
  pinMode(IN2b, LOW); 
  pinMode(IN3b, OUTPUT);
  pinMode(IN4b, LOW); 



  randomSeed(analogRead(A0));
  //Llenar el array de combinaciones con 3 repeticiones de cada función
  int index = 0;
  for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 3; j++) {
          combinaciones[index++] = funciones[i];
      }
  }

  //Aleatorizar el array de combinaciones usando Fisher-Yates
  for (int i = 11; i > 0; i--) {
      int j = random(0, i + 1);
      MotorFunction temp = combinaciones[i];
      combinaciones[i] = combinaciones[j];
      combinaciones[j] = temp;
  }
}

void loop() {
  delay(5000);
  Serial.println("**************TEST BEGIN*********************");
   for (int i = 0; i < 12; i++) {
        combinaciones[i]();  
        delay(1000);
    }

  Serial.println("**************TEST END*********************");
  while (true) {
    }
}

