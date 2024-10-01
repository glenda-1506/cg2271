#include "LED.c"   
#include "Motor.c"

int main(){
  InitGPIO();
  InitSwitch();
	InitBackLedTimer();
	while(1){
		HandleFrontLeds();
	}
}
