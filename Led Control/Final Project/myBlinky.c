#include "MKL25Z4.h"

// Define all Led Pins <Any led pins from F6 onwards is port C>
#define F1 1   // PTD1
#define F2 3   // PTD3
#define F3 2   // PTD2
#define F4 0   // PTD0
#define F5 5   // PTD5
#define F6 9   // PTC9
#define F7 8   // PTC8
#define F8 17  // PTC17
#define F9 16  // PTC16
#define F10 13 // PTC13
#define L1 12  // PTC12

int g_frontLeds[10] = {F1,F2,F3,F4,F5,F6,F7,F8,F9,F10};
volatile int g_isMoving = 0;
volatile int currentFrontLed = 0;
volatile int ledState = 0; // 0 for off & 1 is for on

// Define all other constants
#define MASK(x) (1 << (x))
#define SW_POS 6 // PTD6
#define MAX_DELAY 0x80000



void InitGPIO(void) {
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

    // Clear INT Flag
    PORTD->ISFR |= MASK(SW_POS);
}

static void delay(volatile uint32_t nof) { 
  while(nof!=0) { 
    __asm("NOP");
    nof--;
  }
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

void HandleFrontLed() {
	if (g_isMoving) {
		if (ledState == 0) {
			OffAllFrontLeds();
			OnFrontLed(g_frontLeds[currentFrontLed], currentFrontLed);
      ledState = 1;
		} else {
			OffFrontLed(g_frontLeds[currentFrontLed], currentFrontLed);
      ledState = 0;
      currentFrontLed = (currentFrontLed + 1) % 10; // move to the next LED
		}
	} else {
		OnAllFrontLeds();
    currentFrontLed = 0; // reset to prep for interrupt
    ledState = 0; // reset to prep for interrupt
	}
	delay(0.1 * MAX_DELAY);
}

void HandleFrontLeds(){
	if (g_isMoving){
		OffAllFrontLeds();
		OnFrontLed(g_frontLeds[currentFrontLed], currentFrontLed);
		delay(MAX_DELAY*0.2);
		OffFrontLed(g_frontLeds[currentFrontLed], currentFrontLed);
	} else {
		OnAllFrontLeds();
		currentFrontLed = -1;
	}
	currentFrontLed = (currentFrontLed + 1) % 10; // move to the next LED
}

int main(){  //InitGPIO
  InitGPIO();
  InitSwitch();
	while(1){
		HandleFrontLeds();
	}
}