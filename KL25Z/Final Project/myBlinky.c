#include "LED.c"
#include "Serial.c"
#include "Motor.c"
#include "buzzer.c"
 
/*----------------------------------------------------------------------------
 * Create mutex for shared delay
 *---------------------------------------------------------------------------*/
osMutexId_t motorControlMutex;

void InitRTOX() {
    serialFlag = osSemaphoreNew(1,1,NULL);
		motorFlag = osSemaphoreNew(1,0,NULL);
		runningFlag = osSemaphoreNew(1,1,NULL);
		endFlag = osSemaphoreNew(1,0,NULL);
    // motorControlMutex = osMutexNew(NULL);
}

/*----------------------------------------------------------------------------
 * Buzzer Control
 *---------------------------------------------------------------------------*/
void runningSong_thread (void *argument) {
  // ...
  for (;;) {
		for (int i = 0; i < sizeof(runningSong)/sizeof(Note); i++) { 
			osSemaphoreAcquire(runningFlag, osWaitForever);
			setFrequency(runningSong[i].frequency); 
			setDuration(runningSong[i].beats); 
			TPM1_C0V = 0; // Stop the sound between notes 
			(g_controls.complete) ? osSemaphoreRelease(endFlag) : osSemaphoreRelease(runningFlag);
			osDelay(10);
		} 
	}
}

void stopSong_thread(void *argument) {
	for (;;) {
		for (int i = 0; i < sizeof(stopSong)/sizeof(Note); i++) { 
			osSemaphoreAcquire(endFlag, osWaitForever);
			setFrequency(stopSong[i].frequency); 
			setDuration(stopSong[i].beats); 
			TPM1_C0V = 0; // Stop the sound between notes 
			(g_controls.complete) ? osSemaphoreRelease(endFlag) : osSemaphoreRelease(runningFlag);
			osDelay(10);
		} 
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
		osSemaphoreRelease(motorFlag);
		osDelay(50);
	}
}

/*----------------------------------------------------------------------------
 * Motor Control
 *---------------------------------------------------------------------------*/
void motor_thread (void *argument) {
  // ...
	
  for (;;) {
		osSemaphoreAcquire(motorFlag, osWaitForever);
		HandleMovement();
		osDelay(50);
	}
}

/*----------------------------------------------------------------------------
 * LED Control
 *---------------------------------------------------------------------------*/
void led_thread (void *argument) {
  // ...
	InitLed();
  for (;;) {
		// do something
	}
}

const osThreadAttr_t thread1_attr = {
  .priority = osPriorityHigh //Set initial thread priority to high   
};

/*----------------------------------------------------------------------------
 * Main Loop
 *---------------------------------------------------------------------------*/
int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
	InitSerial(9600);
	InitMotor();
	InitBuzzer();
	
  // ...
 
  osKernelInitialize();  // Initialize CMSIS-RTOS
	InitRTOX();               
  osThreadNew(serial_thread, NULL, NULL);
	osThreadNew(runningSong_thread, NULL, NULL);
	osThreadNew(stopSong_thread, NULL, NULL);
	osThreadNew(led_thread, NULL, NULL);
	osThreadNew(motor_thread, NULL, NULL);
  osKernelStart();                      // Start thread execution
  for (;;) {}
}

