#include "LED.c"
#include "Serial.c"
#include "Motor.c"
#include "buzzer.c"

int main(){
  InitLed();
  InitSwitch();
	InitBuzzer();
	InitSerial(9600);
	InitMotor();
	while(1){
		//playSong();
		HandleMovement();
	}
}
