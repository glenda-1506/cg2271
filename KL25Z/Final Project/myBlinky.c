#include "LED.c"                    // Device header

int main(){
  InitGPIO();
  InitSwitch();
	InitBackLedTimer();
	while(1){
		HandleFrontLeds();
	}
}
