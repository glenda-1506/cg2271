#include "Constants.h"

int g_frontLeds[10] = {F1,F2,F3,F4,F5,F6,F7,F8,F9,F10};
volatile int g_currentFrontLed = 0;
volatile int g_backLedCycle = 25; // amount of overflow for 1 second period [(clk freq / (PS * (1 / 0.5))] / 3750
volatile int g_overflowCounter = 0;
volatile int g_backLedIsOn = 1;
volatile int g_ledOnTicker = 0;

void InitFrontLed(void) {
	// Enable Clock to PORTB & PORTC
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);
	
	// Configure MUX settings to make all pins required GPIO
	// Set Data Direction Registers for all pins
	for (int i = 0; i < 10; i++){
		PORTC->PCR[g_frontLeds[i]] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[g_frontLeds[i]] |= PORT_PCR_MUX(1);
		PTC->PDDR |= MASK(g_frontLeds[i]);
	}
}

void OnFrontLed(int led){
	PTC->PSOR |= MASK(led);
}

void OffFrontLed(int led) {
	PTC->PCOR |= MASK(led);
}

void OnAllFrontLeds(){
	for (int i = 0; i < 10; i++){
		OnFrontLed(g_frontLeds[i]);
	}
}

void OffAllFrontLeds(){
	for (int i = 0; i < 10; i++){
		OffFrontLed(g_frontLeds[i]);
	}
}

void InitLed(){
	
	InitFrontLed();
	
	// Configure Mode 3 for PWM pin Operation
	SIM->SCGC5 |= (SIM_SCGC5_PORTB_MASK);
	PORTB->PCR[B1] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[B1] |= PORT_PCR_MUX(3);
	
	// Enable clock gating for Timer 2
	SIM->SCGC6 |= SIM_SCGC6_TPM2_MASK;
	
	// Select Clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Select MOD value
	TPM2->MOD = 3750;
	TPM2_C1V = TPM2->MOD;
	
	// Select CMOD and Prescalars
	TPM2 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM2 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7) | TPM_SC_TOIE_MASK); // CMOD = 01, Prescalar = 128, Enable the overflow interrupt
  TPM2 -> SC &= ~(TPM_SC_CPWMS_MASK);
	
	// TPM2 CH1 is PTB3
	TPM2_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_MSB_MASK));
	TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Enable IRQ for TPM2
	NVIC_SetPriority(TPM2_IRQn, 3);
  NVIC_ClearPendingIRQ(TPM2_IRQn);
  NVIC_EnableIRQ(TPM2_IRQn);
}

void HandleFrontLeds(){
	if (!(g_controls.stop || g_controls.complete)){
		if (g_ledOnTicker == 0){
			OffAllFrontLeds();
			OnFrontLed(g_frontLeds[g_currentFrontLed]);
			g_ledOnTicker = 10;
		} else {
			g_ledOnTicker--;
		}
	} else {
		OnAllFrontLeds();
		g_currentFrontLed = -1;
	}
	g_currentFrontLed = (g_currentFrontLed + 1) % 10;
}

void TPM2_IRQHandler(){
	NVIC_ClearPendingIRQ(TPM2_IRQn); // clear pending interrupts
	g_backLedCycle = (g_controls.stop || g_controls.complete) ? 25 : 50;
	g_overflowCounter++;
	HandleFrontLeds();
	
	if (g_overflowCounter >= g_backLedCycle){
		TPM2_C1V = g_backLedIsOn ? 3750 : 0;
		g_overflowCounter = 0;
		g_backLedIsOn = !g_backLedIsOn;
	}
	
	TPM2->SC |= TPM_SC_TOF_MASK; // clear overflow flag
}

