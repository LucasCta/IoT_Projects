#include <WiFi.h>
#include <esp_now.h>

int jx = 32;
int jy = 33; 
int jb = 35;

uint8_t macSlave[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

esp_now_peer_info_t peerInfo;

void setup() {
  
  Serial.begin(115200);
  
  pinMode(jb, INPUT_PULLUP);
  
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
  
  if (esp_now_init() != ESP_OK)
    Serial.println("Erro Iniciação do ESP-NOW!!!");
  else
    Serial.println("ESP-NOW OK!!!");
 
  peerInfo.channel = 1;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, macSlave, sizeof(macSlave));
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
    Serial.println("Erro na montagem do peer!!!");
  else
    Serial.println("Montagem OK!!!");

}

void send(String dir){
  esp_err_t result;
  result = esp_now_send(macSlave, (uint8_t *) &dir, sizeof(dir));
  if (result == ESP_OK)
    Serial.println("Envio Sucesso - ");
  else
    Serial.println("Envio Fracasso - ");
}

void loop() {
  
  short x = analogRead(jx);
  short y = analogRead(jy);
  
  x = map(x,0,4096,0,1024);
  y = map(y,0,4096,0,1024);

  if (x > 900) x = 2;
  else if (x < 30) x = 0;
  else x = 1;
  
  if (y > 900) y = 2;
  else if (y < 30) y = 0;
  else y = 1;
  
  switch (x*10 + y){
    case 0:
      send("LEFT-UP");
      break;
    case 1:
      send("UP");
      break;
    case 12:
      send("RIGHT-UP");
      break;
    case 11:
      send("CENTER");
      break;
    default:
      send("REVERSE");
      break;
  }
  
}
