#include <app.h>
// Define all motor pins below (A is red wire, B is black wire)
#define LEFT_FRONT_A 32  // IN1
#define LEFT_FRONT_B 33  // IN2
#define LEFT_BACK_A 25   // IN3
#define LEFT_BACK_B 26   // IN4
#define RIGHT_FRONT_A 27 // IN5
#define RIGHT_FRONT_B 14 // IN6
#define RIGHT_BACK_A 12  // IN7
#define RIGHT_BACK_B 13  // IN8
#define VIN 34

//uint32_t g_vIn = analogRead(VIN);
uint32_t g_vIn = 255;

// Define Motor array where:
// first 4 index is A, last 4 is B
// Even indexes are left motors, Odd indexes are right motors
uint8_t MOTORS[8] = {
  LEFT_FRONT_A,
  RIGHT_FRONT_A,
  LEFT_BACK_A,
  RIGHT_BACK_A,
  LEFT_FRONT_B,
  RIGHT_FRONT_B,
  LEFT_BACK_B,
  RIGHT_BACK_B
};

// Set motor pins here
void SetMotorPins(){
  for (uint8_t i=0; i<8;i++){
    ledcAttachChannel(MOTORS[i], 12000, 8, i);
  }
}

// redo this part later
bool MoveForward() {
  if (RemoteXY.forward == 1){
    Serial.println("<Robot moving forward>");
    for (uint8_t i=0;i<4;i++){
      uint32_t val = g_vIn;
      if (i % 2 == 0){
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.left));
      } else {
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.right));
      }
      ledcWrite(i, val);
      
      // For Debugging
      Serial.println("Motor[" + String(i) + "] value is: " + String(val));
      RemoteXY_delay(125);
      // Delete between these 2 comments
    }
    return true;  
  }
  return false;
}

bool MoveBackward() {
  if (RemoteXY.backward == 1){
    Serial.println("<Robot moving backward>");
    for (uint8_t i=4;i<8;i++){
      uint32_t val = g_vIn;
      if (i % 2 == 0){
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.left));
      } else {
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.right));
      }
      ledcWrite(i, val);

      // For Debugging
      Serial.println("Motor[" + String(i) + "] value is: " + String(val));
      RemoteXY_delay(125);
      // Delete between these 2 comments
    }  
    return true;
  }
  return false;
}

bool Stop() {
  Serial.println("<Robot is not moving>");
  for (uint8_t i=0;i<8;i++){
    ledcWrite(i, 0);
  }
  return true;
}

bool HandleMovement(){
  return MoveForward() || MoveBackward() || Stop();
}
