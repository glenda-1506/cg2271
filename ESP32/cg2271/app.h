/*
   -- ESP Motor Control --

   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
*/

#include <WiFi.h>
#include <env.h>
#include <RemoteXY.h>

#define REMOTEXY_MODE__WIFI

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID NETWORK_NAME
#define REMOTEXY_WIFI_PASSWORD NETWORK_PASSWORD
#define REMOTEXY_SERVER_PORT REMOTEXY_PORT
#define REMOTEXY_ACCESS_PASSWORD REMOTEXY_PASSWORD

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] = {
  255,10,0,0,0,210,0,18,0,0,0,27,1,200,84,1,1,15,0,1,
  28,52,23,23,8,24,31,226,150,189,0,1,28,22,23,23,8,24,31,226,
  150,179,0,129,64,7,74,11,192,3,77,111,116,111,114,32,67,111,110,116,
  114,111,108,115,0,1,123,57,23,23,8,24,31,226,151,129,0,1,158,57,
  23,23,8,24,31,226,150,183,0,4,66,38,57,6,128,1,26,129,80,33,
  27,4,0,16,70,111,114,119,97,114,100,32,83,112,101,101,100,0,4,66,
  52,57,6,128,1,26,129,84,48,21,4,0,16,84,117,114,110,32,83,112,
  101,101,100,0,3,126,26,39,11,132,1,24,3,126,43,40,11,132,1,24,
  129,135,21,24,4,0,16,70,111,114,119,97,114,100,32,71,101,97,114,0,
  129,138,38,18,4,0,16,84,117,114,110,32,71,101,97,114,0,1,171,42,
  9,9,0,1,31,83,0,1,171,29,9,9,0,12,31,67,0 
};

// this structure defines all the variables and events of your control interface 
struct {

  // input variables
  uint8_t backward; // =1 if button pressed, else =0
  uint8_t forward;  // =1 if button pressed, else =0
  uint8_t left;     // =1 if button pressed, else =0
  uint8_t right;    // =1 if button pressed, else =0
  int8_t forwardSpeed; // from 0 to 100
  int8_t turnSpeed;    // from 0 to 100
  uint8_t forwardGear; // from 0 to 3 inclusive
  uint8_t turnGear;    // from 0 to 3 inclusive
  uint8_t stop;        // =1 if button pressed, else =0
  uint8_t complete;    // =1 if button pressed, else =0

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)

uint8_t g_gearControl[4] = {25, 50, 75, 100};
bool isMoving = false;
int currentForwardGear = -1;
int currentTurnGear = -1;

void HandleControl() {
  uint8_t controlByte = 0;  

  if (RemoteXY.complete == 1) {
    controlByte |= (1 << 0);  
    controlByte |= (1 << 1);  
    controlByte |= (1 << 2);  
    controlByte |= (1 << 3);  
  } else if(RemoteXY.stop == 1) {
    controlByte &= ~(1 << 0);  
    controlByte &= ~(1 << 1);  
    controlByte &= ~(1 << 2);
    controlByte &= ~(1 << 3);
  } else {
    if (RemoteXY.forward == 1) controlByte |= (1 << 0);  
    if (RemoteXY.backward == 1) controlByte |= (1 << 1); 
    if (RemoteXY.left == 1) controlByte |= (1 << 2);  
    if (RemoteXY.right == 1) controlByte |= (1 << 3); 
  }

  controlByte |= (RemoteXY.forwardGear << 4);  

  controlByte |= (RemoteXY.turnGear << 6);     

  Serial.write(controlByte);

  Serial.print("Control Byte: ");
  Serial.println(controlByte, BIN);
}

void setup() {
  // Initialize Serial for debugging
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(REMOTEXY_WIFI_SSID, REMOTEXY_WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Initialize RemoteXY
  RemoteXY_Init();
}

void loop() {
  RemoteXY_Handler();
  HandleControl();
}