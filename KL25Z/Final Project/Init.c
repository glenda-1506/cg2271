#include "LED.c"
#include "Serial.c"
#include "Motor.c"
#include "buzzer.c"

void InitTimers(){
	InitBackLedTimer();
	InitSerial(60096);
}
