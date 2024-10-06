#include "Constants.h"

typedef struct {
	int pin;
	volatile uint32_t *channelStatusControl;
	volatile uint32_t *channelValue;
} Motor;

typedef struct {
	uint8_t forward;
	uint8_t backward;
	uint8_t left;
	uint8_t right;
	uint8_t forwardSpeed;
	uint8_t turnSpeed;
	uint8_t forwardGear;
	uint8_t turnGear;
	uint8_t stop;
	uint8_t complete;
} RemoteXY;

volatile RemoteXY g_controls;

Motor motor[4] = {
	{MOTOR_LEFT_RED, &TPM0_C0SC, &TPM0_C0V},
	{MOTOR_RIGHT_RED, &TPM0_C1SC, &TPM0_C1V},
	{MOTOR_LEFT_BLACK, &TPM0_C4SC, &TPM0_C4V},
	{MOTOR_RIGHT_BLACK, &TPM0_C5SC, &TPM0_C5V}
};

void SetMotorSpeed(Motor *motor, int speed){
	*(motor->channelValue) = (uint32_t)((speed / 100.0) * MOTOR_MOD_VALUE);
}

uint8_t ConfigureGearSpeed(uint8_t gear){
	switch (gear){
		case 0:
			return 25;
			// fallthrough
		
		case 1:
			return 50;
			// fallthrough
		
		case 2:
			return 75;
			// fallthrough
		
		case 3:
			return 100;
			// fallthrough
		
		default:
			return 0;
		// fallthrough
	}
}

void ConfigureRemoteXY(){
	uint8_t motorControls = serialData & 0b1111; // keep lower 4 bits
	uint8_t checkFB = motorControls & 0b11;
	uint8_t checkLR = (motorControls >> 2) & 0b11;
	uint8_t gearControls = serialData >> 4;
	
	// set all control variables
	g_controls.forward = (checkFB == 1) ? 1:0;
	g_controls.backward = (checkFB == 2) ? 1:0;
	g_controls.left = (checkLR == 1) ? 1:0;
	g_controls.right = (checkLR == 2)? 1:0;
	g_controls.forwardGear = gearControls & 0b11;
	g_controls.turnGear = (gearControls >> 2) & 0b11;
	g_controls.forwardSpeed = ConfigureGearSpeed(g_controls.forwardGear);
	g_controls.turnSpeed = ConfigureGearSpeed(g_controls.turnGear);
	g_controls.stop = (motorControls == 0) ? 1:0;
	g_controls.complete = (motorControls == 0b1111) ? 1:0;
}

uint8_t MoveForward(){
	if (g_controls.forward == 1){
		int leftMotorSpeed = (int)(MAX_MOTOR_SPEED * ((float)g_controls.forwardSpeed / 100)
											 *	(1 - ((float)(g_controls.turnSpeed) / 100) * g_controls.left));
		int rightMotorSpeed = (int)(MAX_MOTOR_SPEED * ((float)g_controls.forwardSpeed / 100)
												*	(1 - ((float)(g_controls.turnSpeed) / 100) * g_controls.right));
		SetMotorSpeed(&motor[0], leftMotorSpeed);
		SetMotorSpeed(&motor[1], rightMotorSpeed);
		SetMotorSpeed(&motor[2], 0);
		SetMotorSpeed(&motor[3], 0);
		return 1;
	}
	return 0;
}

uint8_t MoveBackward(){
	if (g_controls.backward == 1){
		int leftMotorSpeed = (int)(MAX_MOTOR_SPEED * ((float)g_controls.forwardSpeed / 100)
											 *	(1 - ((float)(g_controls.turnSpeed) / 100) * g_controls.left));
		int rightMotorSpeed = (int)(MAX_MOTOR_SPEED * ((float)g_controls.forwardSpeed / 100)
												*	(1 - ((float)(g_controls.turnSpeed) / 100) * g_controls.right));
		SetMotorSpeed(&motor[2], leftMotorSpeed);
		SetMotorSpeed(&motor[3], rightMotorSpeed);
		SetMotorSpeed(&motor[0], 0);
		SetMotorSpeed(&motor[1], 0);
		return 1;
	}
	return 0;
}

uint8_t Stop(){
	for (uint8_t i = 0; i < 4; i++){
		SetMotorSpeed(&motor[i], 0);
	}
	return 1;
}

uint8_t HandleMovement(){
	ConfigureRemoteXY();
	return MoveForward() || MoveBackward() || Stop();
}

void InitMotor() {
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	for (int i = 0; i < 4; i++){
		PORTC->PCR[motor[i].pin] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[motor[i].pin] |= (i<2) ? PORT_PCR_MUX(4) : PORT_PCR_MUX(3);
	}
	
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER) | TPM_SC_TOIE_MASK);
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	for (int i = 0; i < 4; i++){
		*(motor[i].channelStatusControl) &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
		*(motor[i].channelStatusControl) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
		Stop();
	}	
	TPM0->MOD = MOTOR_MOD_VALUE;
	
	// Enable IRQ for TPM0
	NVIC_SetPriority(TPM0_IRQn, 2);
  NVIC_ClearPendingIRQ(TPM0_IRQn);
  NVIC_EnableIRQ(TPM0_IRQn);
}

void TPM0_IRQHandler(){
	NVIC_ClearPendingIRQ(TPM0_IRQn);
	if (TPM0->SC & TPM_SC_TOF_MASK) {
		if (serialReady){
			HandleMovement();
			serialReady = 0;
		}
	}
	TPM0->SC |= TPM_SC_TOF_MASK;
}

