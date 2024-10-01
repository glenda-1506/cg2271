#include "LED.c"
#include "Serial.c"
#include "Motor.c"

void InitTimers(){
	InitBackLedTimer();
	InitSerial(60096);
	//InitMotor();
}
