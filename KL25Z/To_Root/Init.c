#include "LED.c"
#include "Serial.c"

void InitTimers(){
	InitBackLedTimer();
	InitSerial(60096);
}
