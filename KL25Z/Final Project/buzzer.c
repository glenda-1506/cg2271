#include "Constants.h"    

/* 
music will be played in sixteenth notes (semiquaver) 
Note duration (in beats): 
semibreve 16 
minim 8 
crotchet 4 
quaver 2
semiquaver 1 
*/ 
 
typedef struct { 
	float frequency;   // Frequency of the note 
	int beats; // Number of beats for the Duration of the note 
} Note; 


Note song[] = { 
    {NOTE_E4, 4},
    {NOTE_B4, 2}, {NOTE_GS4, 2}, {NOTE_GS4, 4}, {NOTE_FS4, 2}, {NOTE_E4, 2}, {NOTE_E4, 2}, {NOTE_A4, 4},
    {NOTE_GS4, 2}, {NOTE_GS4, 2}, {NOTE_FS4, 2}, {NOTE_FS4, 2}, {NOTE_E4, 4},{NOTE_E4, 2},
    {NOTE_B4, 2}, {NOTE_GS4, 2}, {NOTE_GS4, 4}, {NOTE_FS4, 2}, {NOTE_E4, 2}, {NOTE_E4, 2},
    {NOTE_CS4, 4}, {NOTE_B3, 6}, {REST, 4}, {NOTE_E4, 2}, {NOTE_E4, 1}, {NOTE_E4, 1},
		{NOTE_B4, 2}, {NOTE_GS4, 2}, {NOTE_GS4, 2}, {NOTE_FS4, 2}, {NOTE_FS4, 2}, {NOTE_E4, 2}, {NOTE_E4, 2}, {NOTE_A4, 4},
		{NOTE_GS4, 2}, {NOTE_GS4, 2}, {NOTE_FS4, 2}, {NOTE_FS4, 2}, {NOTE_E4, 2}, {NOTE_E4, 2}, {NOTE_B4, 2}, 
		{NOTE_GS4, 2}, {NOTE_GS4, 2}, {NOTE_FS4, 4}, {NOTE_E4, 2}, {NOTE_E4, 2}, {NOTE_F4, 4}, 
		{NOTE_CS4, 2}
}; 
 
 
void setFrequency(float frequency) { 
	uint16_t modValue; 
  uint32_t tpmClock; 
     
  tpmClock = (48000000 / 128); // TPM clock frequency after prescaler 
     
  modValue = (uint16_t)(tpmClock / frequency); 
  TPM1->MOD = modValue; 
  TPM1_C0V = modValue / 32; // 50% duty cycle 
} 
 
 
void setDuration(float beats) { 
	int duration = beats * BEAT_DURATION; 
  delay(duration);
} 

void playSong(){ 
  for (int i = 0; i < sizeof(song)/sizeof(Note); i++) { 
		setFrequency(song[i].frequency); 
    setDuration(song[i].beats); 
    TPM1_C0V = 0; // Stop the sound between notes 
    delay(100);
  } 
} 

void InitBuzzer() { 
	// Enable clock for Port B 
  SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; 
   
  // Configure PTB0 as TPM1_CH0 (Alt function 3 for PWM) 
  PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;  // Clear MUX field for PTB0 
  PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(3);     // Set PTB0 to TPM1_CH0 
   
  // Enable clock for TPM1 module (Timer/PWM Module 1)
  SIM -> SCGC6 |= SIM_SCGC6_TPM1_MASK; 
   
  // Select TPM clock source as MCGFLLCLK or MCGPLLCLK/2 
  SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;  // Clear clock source for TPM 
  SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);     // Set clock source to MCGFLLCLK 
   
  // Initialize TPM1 for PWM 
  TPM1 -> MOD = 0;  // Set modulation register to 0 to prevent initial buzzing 
  TPM1_C0V = 0;     // Set initial duty cycle for channel 0 to 0 
   
  // Set TPM1 timer to up-counting mode with a prescaler of 128 
  TPM1 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear clock mode and prescaler 
  TPM1 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set clock mode to '01' (up-counting) and prescaler to 128 
   
  // Ensure the timer is in up-counting PWM mode 
  TPM1 -> SC &= ~(TPM_SC_CPWMS_MASK);  // Ensure up-counting mode is selected 
   
  // Configure TPM1_CH0 for edge-aligned PWM (ELSxB = 1, MSxB = 1) 
  TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));  // Clear channel control 
  TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Set edge-aligned PWM with high-true pulses (ELSxB = 1, MSxB = 1) 
} 
