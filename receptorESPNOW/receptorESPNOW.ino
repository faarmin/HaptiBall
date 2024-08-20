#include <esp_now.h>
#include <WiFi.h>
#define COMMAND_ACTIVATE 1
#define COMMAND_DEACTIVATE 2

typedef struct struct_message {
    uint8_t command;  
    uint8_t pin;      
} struct_message;

struct_message incomingData;


void OnDataRecv(const esp_now_recv_info* info, const uint8_t *data, int len) {
    memcpy(&incomingData, data, sizeof(incomingData));

    Serial.print("Comando recibido: ");
    Serial.print("Comando: ");
    Serial.print(incomingData.command);
    Serial.print(" - Pin: ");
    Serial.println(incomingData.pin);

    // Configura los pines como salida
    //pinMode(incomingData.pin, OUTPUT);

    // Activa o desactiva el pin según el comando
    if (incomingData.command == COMMAND_ACTIVATE) {
        //digitalWrite(incomingData.pin, HIGH);
        Serial.print("Pin ");
        Serial.print(incomingData.pin);
        Serial.println(" activado.");
    } else if (incomingData.command == COMMAND_DEACTIVATE) {
        //digitalWrite(incomingData.pin, LOW);
        Serial.print("Pin ");
        Serial.print(incomingData.pin);
        Serial.println(" desactivado.");
    }
}

  void setup() {
    Serial.begin(115200);

    WiFi.mode(WIFI_AP_STA);  
    WiFi.disconnect(true);   

    delay(100);
    String macAddress = WiFi.macAddress();

    WiFi.channel(6); 

    // Imprime la dirección MAC
    Serial.print("Dirección MAC: ");
    Serial.println(macAddress);
    Serial.print("Canal Wi-Fi actual: ");
    Serial.println(WiFi.channel());

    
    // Inicializa ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error al inicializar ESP-NOW");
        return;
    }else{
      Serial.println("ESP-NOW inicializado");
    }

    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
}
