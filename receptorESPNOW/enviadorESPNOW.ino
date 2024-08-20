#include <esp_now.h>
#include <WiFi.h>

// Dirección MAC del esclavo (Reemplaza con la MAC de tu esclavo)
uint8_t slaveAddress[] = {0x84, 0xCC, 0xA8, 0x48, 0x7F, 0xF8};  

#define COMMAND_ACTIVATE 1
#define COMMAND_DEACTIVATE 2

typedef struct struct_message {
    uint8_t command;  
    uint8_t pin;      
} struct_message;

struct_message myData;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
   

    // Inicializa ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error al inicializar ESP-NOW");
        return;
    }

    // Añade el esclavo como peer
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, slaveAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Error al añadir el peer");
        return;
    }
}

void loop() {
  
    myData.command = COMMAND_ACTIVATE;
    myData.pin = 13;
    esp_now_send(slaveAddress, (uint8_t *)&myData, sizeof(myData));
    Serial.println("Enviando comando para activar el pin 13");
    delay(2000);  
}

