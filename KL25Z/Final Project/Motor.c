#include "Constants.h"

typedef struct {
	int pin;
	volatile uint32_t *channelStatusControl;
	volatile uint32_t *channelValue;
} Motor;

Motor motor[4] = {
	{MOTOR_LEFT_RED, &TPM0_C0SC, &TPM0_C0V},
	{MOTOR_RIGHT_RED, &TPM0_C1SC, &TPM0_C1V},
	{MOTOR_LEFT_BLACK, &TPM0_C4SC, &TPM0_C4V},
	{MOTOR_RIGHT_BLACK, &TPM0_C5SC, &TPM0_C5V}
};

void setMotorSpeed(Motor *motor, int speed){
	*(motor->channelValue) = (uint32_t)((speed / 100.0) * MOTOR_MOD_VALUE);
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
  TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
  TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
	
	for (int i = 0; i < 4; i++){
		*(motor[i].channelStatusControl) &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
		*(motor[i].channelStatusControl) |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
		setMotorSpeed(&motor[i],0);
	}
	setMotorSpeed(&motor[1],100);
	TPM0->MOD = MOTOR_MOD_VALUE;
}

