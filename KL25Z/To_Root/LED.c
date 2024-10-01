#include "MKL25Z4.h"
#include "Constants.h"

int g_frontLeds[10] = {F1,F2,F3,F4,F5,F6,F7,F8,F9,F10};
volatile int g_isMoving = 0;
volatile int g_currentFrontLed = 0;
volatile int g_backLedCycle = 25; // amount of overflow for 1 second period
volatile int g_overflowCounter = 0;
volatile int g_backLedIsOn = 1;

void InitLed(void) {
	// Enable Clock to PORTD & PORTC
  SIM->SCGC5 |= (SIM_SCGC5_PORTD_MASK);
	SIM->SCGC5 |= (SIM_SCGC5_PORTC_MASK);
	
	// Configure MUX settings to make all pins required GPIO
	// Set Data Direction Registers for all pins
	for (int i = 0; i < 5; i++){
		PORTD->PCR[g_frontLeds[i]] &= ~PORT_PCR_MUX_MASK;
		PORTD->PCR[g_frontLeds[i]] |= PORT_PCR_MUX(1);
		PTD->PDDR |= MASK(g_frontLeds[i]);
	}
	
	for (int i = 5; i < 10; i++){
		PORTC->PCR[g_frontLeds[i]] &= ~PORT_PCR_MUX_MASK;
		PORTC->PCR[g_frontLeds[i]] |= PORT_PCR_MUX(1);
		PTC->PDDR |= MASK(g_frontLeds[i]);
	}
}

void InitSwitch(void){
    // enable clock for PortD
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;

    /* Select GPIO and enable pull-up resistors and interrupts 
    on falling edges of pin connected to switch    
    */
    PORTD->PCR[SW_POS] |= (PORT_PCR_MUX(1) |
                           PORT_PCR_PS_MASK |
                           PORT_PCR_PE_MASK |
                           PORT_PCR_IRQC(0x0a));

    // Set PORT D Switch bit to input
    PTD->PDDR &= ~MASK(SW_POS);

    // Enable the Interrupts
    NVIC_SetPriority(PORTD_IRQn, 2);
    NVIC_ClearPendingIRQ(PORTD_IRQn);
    NVIC_EnableIRQ(PORTD_IRQn);
}

void PORTD_IRQHandler(){
    // Clear Pending IRQ
    NVIC_ClearPendingIRQ(PORTD_IRQn);

    // Update some variables / flag
    g_isMoving = !g_isMoving;
		g_isMoving ? (g_backLedCycle = 50):(g_backLedCycle = 25);

    // Clear INT Flag
    PORTD->ISFR |= MASK(SW_POS);
}

static void delay(volatile uint32_t nof) { 
  while(nof!=0) { 
    __asm("NOP");
    nof--;
  }
}

void OnAllBackLeds(){
	PTC->PSOR |= MASK(B1);
}

void OffAllBackLeds(){
	PTC->PCOR |= MASK(B1);
}
void OnFrontLed(int led, int index){
	if (index < 5){
		PTD->PSOR |= MASK(led); // All pins are Port D
	} 
	else {
		PTC->PSOR |= MASK(led); // All pins are Port C
	}
}

void OffFrontLed(int led, int index) {
	if (index < 5){
		PTD->PCOR |= MASK(led); // All pins are Port D
	} 
	else {
		PTC->PCOR |= MASK(led); // All pins are Port C
	}
}

void OnAllFrontLeds(){
	for (int i = 0; i < 10; i++){
		OnFrontLed(g_frontLeds[i],i);
	}
}

void OffAllFrontLeds(){
	for (int i = 0; i < 10; i++){
		OffFrontLed(g_frontLeds[i],i);
	}
}

void HandleFrontLeds(){
	if (g_isMoving){
		OffAllFrontLeds();
		OnFrontLed(g_frontLeds[g_currentFrontLed], g_currentFrontLed);
		delay(MAX_DELAY*0.2);
		OffFrontLed(g_frontLeds[g_currentFrontLed], g_currentFrontLed);
	} else {
		OnAllFrontLeds();
		g_currentFrontLed = -1;
	}
	g_currentFrontLed = (g_currentFrontLed + 1) % 10;
}

void InitBackLedTimer(){
	// Configure Mode 4 for PWM pin Operation
	PORTC->PCR[B1] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[B1] |= PORT_PCR_MUX(4);
	
	// Enable clock gating for Timer 0
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	// Select Clock for TPM module
	SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	// Select MOD value
	TPM0->MOD = 3750;
	TPM0_C1V = TPM0->MOD;
	
	// Select CMOD and Prescalars
	TPM0 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
  TPM0 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7) | TPM_SC_TOIE_MASK); // CMOD = 01, Prescalar = 128, Enable the overflow interrupt
  TPM0 -> SC &= ~(TPM_SC_CPWMS_MASK);
	
	// TPM0 CH1 is PTC2
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSA_MASK) | (TPM_CnSC_MSB_MASK));
	TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
	
	// Enable IRQ for TPM0
	NVIC_SetPriority(TPM0_IRQn, 3);
  NVIC_ClearPendingIRQ(TPM0_IRQn);
  NVIC_EnableIRQ(TPM0_IRQn);
}

void TPM0_IRQHandler(){
	NVIC_ClearPendingIRQ(TPM0_IRQn); // clear pending interrupts
	g_overflowCounter++;
	if (g_overflowCounter >= g_backLedCycle){
		if (g_backLedIsOn){
			TPM0_C1V = 0;
		} else {
			TPM0_C1V = 3750;
		}
		g_overflowCounter = 0;
		g_backLedIsOn = !g_backLedIsOn;
	}
	TPM0->SC |= TPM_SC_TOF_MASK; // clear overflow flag
}
