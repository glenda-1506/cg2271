#include "LED.c"
#include "Serial.c"
#include "Motor.c"
#include "buzzer.c"

int main(){
  InitLed();
  InitSwitch();
	InitBuzzer();
	while(1){
		playSong();
	}
}
