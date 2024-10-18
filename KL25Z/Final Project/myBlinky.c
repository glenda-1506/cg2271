#include "LED.c"
#include "Serial.c"
#include "Motor.c"
#include "buzzer.c"
 
/*----------------------------------------------------------------------------
 * Create mutex for shared delay
 *---------------------------------------------------------------------------*/
osMutexId_t motorControlMutex;

void InitRTOX() {
    serialFlag = osSemaphoreNew(1, 0, NULL);
    // motorControlMutex = osMutexNew(NULL);
}

/*----------------------------------------------------------------------------
 * Buzzer Control
 *---------------------------------------------------------------------------*/
void buzzer_thread (void *argument) {
  // ...
  for (;;) {
		playSong();
	}
}

/*----------------------------------------------------------------------------
 * Serial Data Control
 *---------------------------------------------------------------------------*/
void serial_thread (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(serialFlag, osWaitForever);
		ConfigureRemoteXY();
		osSemaphoreRelease(serialFlag);
		osDelay(100);
	}
}

/*----------------------------------------------------------------------------
 * Motor Control
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
  // ...
  for (;;) {
		osSemaphoreAcquire(serialFlag, osWaitForever);
		ConfigureRemoteXY();
		HandleMovement();
		osSemaphoreRelease(serialFlag);
		osDelay(100);
	}
}

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	//InitLed();
	InitBuzzer();
	InitSerial(9600);
	InitMotor();
	InitRTOX();
  // ...
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
  //osThreadNew(buzzer_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
