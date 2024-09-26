#include <app.h>
#include <env.h>
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
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
}

void loop() {
  RemoteXY_Handler ();
  HandleControl();
}
