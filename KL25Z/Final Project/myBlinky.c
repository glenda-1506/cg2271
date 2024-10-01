#include "Init.c"  

int main(){
  InitLed();
  InitSwitch();
	InitTimers();
	InitMotor();
	while(1){
		HandleFrontLeds();
	}
}
