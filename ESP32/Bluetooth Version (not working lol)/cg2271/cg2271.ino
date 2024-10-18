#include <app.h>
#include "BluetoothSerial.h"
BluetoothSerial ESP_BT;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600,SERIAL_8N1,16,17);

  ESP_BT.begin(REMOTEXY_BLUETOOTH_NAME);
  Serial.println("Bluetooth started. Waiting for a device to connect...");
  
  while (!ESP_BT.hasClient()) {
    Serial.println("Trying to connect to a Bluetooth device...");
    delay(1000); 
  }

  Serial.println("Bluetooth device connected!");
  RemoteXY_Init(); 
}

void loop() {
  RemoteXY_Handler();
  HandleControl(); 
}
