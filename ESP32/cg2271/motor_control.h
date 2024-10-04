#include <app.h>

//uint32_t g_vIn = analogRead(VIN);
uint32_t g_vIn = 255;

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
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) 
            * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.left));
      } else {
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) 
            * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.right));
      }
      ledcWrite(MOTORS[i], val);
      
      // For Debugging
      Serial.println("Motor[" + String(i) + "] value is: " + String(val));
      RemoteXY_delay(125);
      // Delete between these 2 comments
      
      Serial.write((uint8_t*)&val, sizeof(val));

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
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) 
            * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.left));
      } else {
        val = (uint32_t)(val * ((float)RemoteXY.forwardSpeed / 100) 
            * (1 - ((float)(RemoteXY.turnSpeed) / 100) * RemoteXY.right));
      }
      ledcWrite(MOTORS[i], val);

      // For Debugging
      Serial.println("Motor[" + String(i) + "] value is: " + String(val));
      RemoteXY_delay(125);
      // Delete between these 2 comments

      Serial.write((uint8_t*)&val, sizeof(val));

    }  
    return true;
  }
  return false;
}

bool Stop() {
  Serial.println("<Robot is not moving>");
  for (uint8_t i=0;i<8;i++){
    ledcWrite(MOTORS[i], 0);
  }
  return true;
}

bool HandleMovement(){
  return MoveForward() || MoveBackward() || Stop();
}
