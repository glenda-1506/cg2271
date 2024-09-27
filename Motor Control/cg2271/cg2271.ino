#include <motor_control.h>

const char* ssid     = NETWORK_NAME;
const char* password = NETWORK_PASSWORD;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  RemoteXY_Init (); 

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Trying to connect to: ");
    Serial.println(NETWORK_NAME);
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  SetMotorPins();
}

void loop() {
  RemoteXY_Handler ();
  HandleControl();
  HandleMovement();
}
