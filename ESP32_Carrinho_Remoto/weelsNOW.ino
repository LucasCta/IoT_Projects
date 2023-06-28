#include <espnow.h>
#include <ESP8266WiFi.h>

void setup() {
  
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  Serial.print("Mac Address Slave: ");
  Serial.println(WiFi.macAddress());
  
  if (esp_now_init() != 0)
    Serial.println("Erro Iniciação do ESP-NOW!!!");
  else
    Serial.println("ESP-NOW OK!!!");

  esp_now_register_recv_cb(OnDataRecv);
}


void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) {
  
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1],mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  
  Serial.print("Tamanho da Mensagem : ");
  Serial.println(len);
  
  Serial.print("Mac do Master : ");
  Serial.println(macStr);

  char dir;
  memcpy(&dir, incomingData, sizeof(char));
  Serial.print("Direcao Recebida: ")
  Serial.println(dir);
}

void loop() {
  
}
