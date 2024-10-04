#include "LED.c"
#include "Serial.c"
#include "Motor.c"

void app_main (void *argument) {
	
  for (;;) {
		osSemaphoreAcquire(flag, osWaitForever);

		HandleMotorControl(serialData); 
}

void InitTimers(){
	InitBackLedTimer();
	InitSerial(60096);
	InitMotor()

	osKernelInitialize();

	flag = osSemaphoreNew(1, 0, NULL);

	osThreadNew(app_main, NULL, &main_thread_attr);
	osThreadNew(motor_control, NULL, NULL);
	osKernelStart();


}
