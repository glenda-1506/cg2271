#include "Init.c"  

int main(){
  InitLed();
  InitSwitch();
	InitTimers();
	InitMotor();
	initPWM();
	while(1){
		playSong();
	}
}
