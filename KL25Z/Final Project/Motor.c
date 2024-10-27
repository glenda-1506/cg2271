#include "Constants.h"

void SetMotorSpeed(const MotorWire *MOTOR_WIRE, int speed){
	*(MOTOR_WIRE->channelValue) = (uint32_t)((speed / 100.0) * MOTOR_MOD_VALUE);
}

int CalculateMotorSpeed(uint8_t turnState){
	float forwardFactor = (float)g_controls.forwardSpeed / 100.0;
	float turnFactor = 1.0 - TURN_RADIUS;
	float turnAdjustment = 1.0 - (1.0 - (float)g_controls.turnSpeed / 100.0) * turnState * turnFactor;
	return (int)(MAX_MOTOR_SPEED * forwardFactor * turnAdjustment);
}

uint8_t MoveForward(){
	if (g_controls.forward){
		int leftMotorSpeed = CalculateMotorSpeed(g_controls.left);
		int rightMotorSpeed = CalculateMotorSpeed(g_controls.right);
		SetMotorSpeed(&MOTOR_WIRE[LEFT_BLACK], g_controls.left ? 0 : leftMotorSpeed);
		SetMotorSpeed(&MOTOR_WIRE[LEFT_RED], g_controls.left ? leftMotorSpeed : 0);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_RED], g_controls.right ? 0 : rightMotorSpeed);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_BLACK], g_controls.right ? rightMotorSpeed : 0);
		return 1;
	}
	return 0;
}

uint8_t MoveBackward(){
	if (g_controls.backward){
		int leftMotorSpeed = CalculateMotorSpeed(g_controls.left);
		int rightMotorSpeed = CalculateMotorSpeed(g_controls.right);
		SetMotorSpeed(&MOTOR_WIRE[LEFT_RED], g_controls.left ? 0 : leftMotorSpeed); 
		SetMotorSpeed(&MOTOR_WIRE[LEFT_BLACK], g_controls.left ? leftMotorSpeed : 0);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_BLACK], g_controls.right ? 0 : rightMotorSpeed); 
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_RED], g_controls.right ? rightMotorSpeed : 0);		
		return 1;
	}
	return 0;
}

uint8_t RotateAntiClockwise(){
	if (g_controls.left) {
		int speed = (int)(MAX_MOTOR_SPEED * (float)(g_controls.turnSpeed) / 100); 
		SetMotorSpeed(&MOTOR_WIRE[LEFT_RED], speed); 
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_RED], speed);
		SetMotorSpeed(&MOTOR_WIRE[LEFT_BLACK], 0);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_BLACK], 0);
		return 1;
	}
	return 0;
}

uint8_t RotateClockwise(){
	if (g_controls.right) {
		int speed = (int)(MAX_MOTOR_SPEED * (float)(g_controls.turnSpeed) / 100); 
		SetMotorSpeed(&MOTOR_WIRE[LEFT_BLACK], speed);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_BLACK], speed);
		SetMotorSpeed(&MOTOR_WIRE[LEFT_RED], 0);
		SetMotorSpeed(&MOTOR_WIRE[RIGHT_RED], 0);
		return 1;
	}
	return 0;
}

uint8_t Stop(){
	for (uint8_t i = 0; i < 4; i++){
		SetMotorSpeed(&MOTOR_WIRE[i], 0);
	}
	return 1;
}

uint8_t HandleMovement(){
	return MoveForward() || MoveBackward() || RotateAntiClockwise() || RotateClockwise() || Stop();
}

void InitMotor() {
  SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
  SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
	for (int i = 0; i < 4; i++){
		PORTC->PCR[MOTOR_WIRE[i].pin] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[MOTOR_WIRE[i].pin] |= (i<2) ? PORT_PCR_MUX(4) : PORT_PCR_MUX(3);
	}
	
  SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
  SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
  TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER) | TPM_SC_TOIE_MASK);
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	for (int i = 0; i < 4; i++){
		*(MOTOR_WIRE[i].channelStatusControl) &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
		*(MOTOR_WIRE[i].channelStatusControl) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
		Stop();
	}	
	
	TPM0->MOD = MOTOR_MOD_VALUE;
}
