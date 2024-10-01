#include "Init.c"  

int main(){
  InitLed();
  InitSwitch();
	InitTimers();
	while(1){
		HandleFrontLeds();
	}
}
