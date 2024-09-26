/*
   -- ESP Motor Control --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 3.1.13 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.14.08 or later version;
     - for iOS 1.11.2 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__WIFI

#include <WiFi.h>
#include <env.h>
#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID NETWORK_NAME
#define REMOTEXY_WIFI_PASSWORD NETWORK_PASSWORD
#define REMOTEXY_SERVER_PORT REMOTEXY_PORT
#define REMOTEXY_ACCESS_PASSWORD REMOTEXY_PASSWORD

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 217 bytes
  { 255,10,0,0,0,210,0,18,0,0,0,27,1,200,84,1,1,15,0,1,
  28,52,23,23,8,24,31,226,150,189,0,1,28,22,23,23,8,24,31,226,
  150,179,0,129,64,7,74,11,192,3,77,111,116,111,114,32,67,111,110,116,
  114,111,108,115,0,1,123,57,23,23,8,24,31,226,151,129,0,1,158,57,
  23,23,8,24,31,226,150,183,0,4,66,38,57,6,128,1,26,129,80,33,
  27,4,0,16,70,111,114,119,97,114,100,32,83,112,101,101,100,0,4,66,
  52,57,6,128,1,26,129,84,48,21,4,0,16,84,117,114,110,32,83,112,
  101,101,100,0,3,126,26,39,11,132,1,24,3,126,43,40,11,132,1,24,
  129,135,21,24,4,0,16,70,111,114,119,97,114,100,32,71,101,97,114,0,
  129,138,38,18,4,0,16,84,117,114,110,32,71,101,97,114,0,1,171,42,
  9,9,0,1,31,83,0,1,171,29,9,9,0,12,31,67,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t backward; // =1 if button pressed, else =0
  uint8_t forward; // =1 if button pressed, else =0
  uint8_t left; // =1 if button pressed, else =0
  uint8_t right; // =1 if button pressed, else =0
  int8_t forwardSpeed; // from 0 to 100
  int8_t turnSpeed; // from 0 to 100
  uint8_t forwardGear; // from 0 to 3 inclusive
  uint8_t turnGear; // from 0 to 3 inclusive
  uint8_t stop; // =1 if button pressed, else =0
  uint8_t complete; // =1 if button pressed, else =0

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;   
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
uint8_t g_gearControl[4] = {25,50,75,100};
bool isMoving = false;
int currentForwardGear = -1;
int currentTurnGear = -1;

void HandleControl(){
  if (RemoteXY.forward == 1){
    Serial.println("Robot moving forward");
    isMoving = true;
  }

  if (RemoteXY.backward == 1){
    Serial.println("Robot moving backward");
    isMoving = true;
  }

  if (isMoving && RemoteXY.forward == 0 && RemoteXY.backward == 0){
    RemoteXY.stop = 1;
    isMoving = false;
  }

  if (RemoteXY.stop == 1){
    // Do something here to signal that robot has stopped (an interrupt to KL25Z to trigger LEDs)
    Serial.println("Robot is not moving");
    RemoteXY_delay(100); 
    RemoteXY.stop = 0; // this line is needed!
  }

  if (RemoteXY.complete == 1){
    Serial.println("Race is complete");
    RemoteXY_delay(100); 
  }
  
  for (int i = 0; i < 4; i++){
    if (currentForwardGear != i && RemoteXY.forwardGear == i){
      RemoteXY.forwardSpeed = g_gearControl[i];
      currentForwardGear = i;
    }
  }

  for (int i = 0; i < 4; i++){
    if (currentTurnGear != i && RemoteXY.turnGear == i){
      RemoteXY.turnSpeed = g_gearControl[i];
      currentTurnGear = i;
    }
  }
}


/* DO NOT DELETE THE CODE BELOW

void setup() 
{
  RemoteXY_Init (); 
  
  
  // TODO you setup code
  
}


void loop() 
{ 
  RemoteXY_Handler ();
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}
*/