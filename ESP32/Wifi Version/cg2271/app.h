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
uint8_t RemoteXY_CONF[] =   // 236 bytes
  { 255,10,0,22,0,229,0,19,0,0,0,0,27,1,200,84,1,1,17,0,
  1,28,52,23,23,8,24,31,226,150,189,0,1,28,22,23,23,8,24,31,
  226,150,179,0,129,64,7,74,11,192,3,77,111,116,111,114,32,67,111,110,
  116,114,111,108,115,0,1,123,57,23,23,8,24,31,226,151,129,0,1,158,
  57,23,23,8,24,31,226,150,183,0,4,66,38,57,6,128,1,26,129,80,
  33,27,4,0,16,70,111,114,119,97,114,100,32,83,112,101,101,100,0,4,
  66,52,57,6,128,1,26,129,84,48,21,4,0,16,84,117,114,110,32,83,
  112,101,101,100,0,3,126,26,39,11,132,1,24,3,126,43,40,11,132,1,
  24,129,135,21,24,4,0,16,70,111,114,119,97,114,100,32,71,101,97,114,
  0,129,138,38,18,4,0,16,84,117,114,110,32,71,101,97,114,0,1,171,
  42,9,9,0,1,31,83,0,1,171,29,9,9,0,12,31,67,0,67,74,
  60,40,10,69,2,26,11,67,84,25,20,6,69,2,26,11 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t backward; // =1 if button pressed, else =0
  uint8_t forward; // =1 if button pressed, else =0
  uint8_t left; // =1 if button pressed, else =0
  uint8_t right; // =1 if button pressed, else =0
  int8_t forwardSpeed; // from 0 to 100
  int8_t turnSpeed; // from 0 to 100
  uint8_t forwardGear; // from 0 to 4
  uint8_t turnGear; // from 0 to 4
  uint8_t stop; // =1 if button pressed, else =0
  uint8_t complete; // =1 if button pressed, else =0

    // output variables
  char serialBinary[11]; // string UTF8 end zero
  char serialValue[11]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;     
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////
#define MASK(i) (1<<i)
int prevData = 0;
bool isComplete = false;

void HandleControl(){
  uint8_t currentData = 0;
  if (RemoteXY.complete){
    for (int i=0;i<4;i++){
      currentData |= MASK(i);
    }   
  } else if (RemoteXY.stop) {
    for (int i=0;i<4;i++){
      currentData &= ~MASK(i);
    }
  } else {
    if (RemoteXY.forward) currentData |= MASK(0);
    if (RemoteXY.backward) currentData |= MASK(1);
    if (RemoteXY.left) currentData |= MASK(2);
    if (RemoteXY.right) currentData |= MASK(3);
  }
  currentData |= (RemoteXY.forwardGear << 4); 
  currentData |= (RemoteXY.turnGear << 6); 
  if (currentData != prevData){
    Serial2.write(currentData);
    Serial.print("Control Byte: ");
    Serial.println(currentData, BIN);
    itoa(currentData, RemoteXY.serialBinary, 2);
    itoa(currentData, RemoteXY.serialValue, 10);
    prevData = currentData;
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