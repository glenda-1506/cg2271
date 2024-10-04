#include "MKL25Z4.h" 
 
#define PTB0_Pin 0 
#define PTB1_Pin 1 
#define BPM 120 
#define NOTE_C1  32.70 
#define NOTE_CS1 34.65 
#define NOTE_D1  36.71 
#define NOTE_DS1 38.89 
#define NOTE_E1  41.20 
#define NOTE_F1  43.65 
#define NOTE_FS1 46.25 
#define NOTE_G1  49.00 
#define NOTE_GS1 51.91 
#define NOTE_A1  55.00 
#define NOTE_AS1 58.27 
#define NOTE_B1  61.74 
#define NOTE_C2  65.41 
#define NOTE_CS2 69.30 
#define NOTE_D2  73.42 
#define NOTE_DS2 77.78 
#define NOTE_E2  82.41 
#define NOTE_F2  87.31 
#define NOTE_FS2 92.50 
#define NOTE_G2  98.00 
#define NOTE_GS2 103.83 
#define NOTE_A2  110.00 
#define NOTE_AS2 116.54 
#define NOTE_B2  123.47 
#define NOTE_C3  130.81 
#define NOTE_CS3 138.59 
#define NOTE_D3  146.83 
#define NOTE_DS3 155.56 
#define NOTE_E3  164.81 
#define NOTE_F3  174.61 
#define NOTE_FS3 185.00 
#define NOTE_G3  196.00 
#define NOTE_GS3 207.65 
#define NOTE_A3  220.00 
#define NOTE_AS3 233.08 
#define NOTE_B3  246.94 
#define NOTE_C4  261.63 
#define NOTE_CS4 277.18 
#define NOTE_D4  293.66 
#define NOTE_DS4 311.13 
#define NOTE_E4  329.63 
#define NOTE_F4  349.23 
#define NOTE_FS4 369.99 
#define NOTE_G4  392.00 
#define NOTE_GS4 415.30 
#define NOTE_A4  440.00 
#define NOTE_AS4 466.16 
#define NOTE_B4  493.88 
#define NOTE_C5  523.25 
#define NOTE_CS5 554.37 
#define NOTE_D5  587.33 
#define NOTE_DS5 622.25 
#define NOTE_E5  659.25 
#define NOTE_F5  698.46 
#define NOTE_FS5 739.99 
#define NOTE_G5  783.99 
#define NOTE_GS5 830.61 
#define NOTE_A5  880.00 
#define NOTE_AS5 932.33 
#define NOTE_B5  987.77 
#define NOTE_C6  1046.50 
#define NOTE_CS6 1108.73 
#define NOTE_D6  1174.66 
#define NOTE_DS6 1244.51 
#define NOTE_E6  1318.15 
#define NOTE_F6  1396.91 
#define NOTE_FS6 1479.98 
#define NOTE_G6  1567.98 
#define NOTE_GS6 1661.22 
#define NOTE_A6  1760.00 
#define NOTE_AS6 1864.66 
#define NOTE_B6  1975.53 
#define NOTE_C7  2093.00 
#define NOTE_CS7 2217.46 
#define NOTE_D7  2349.32 
#define NOTE_DS7 2489.02 
#define NOTE_E7  2637.02 
#define NOTE_F7  2793.83 
#define NOTE_FS7 2959.69 
#define NOTE_G7  3135.96 
#define NOTE_GS7 3322.44 
#define NOTE_A7  3520.00 
#define NOTE_AS7 3729.31 
#define NOTE_B7  3951.07 
#define NOTE_C8  4186.01 
#define REST      0 
 
#define BEAT_DURATION (60000.0/BPM) //duration for an eighth note 
 
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


static void delay_ms(volatile uint32_t milliseconds) {  
  while(milliseconds != 0) {  
    volatile uint32_t cycles = 375; // 1 ms delay with prescaler of 128
    while(cycles != 0) {
      __asm("NOP"); 
      cycles--; 
    }
    milliseconds--; 
  } 
}


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
    delay_ms(duration);
} 
 
 
void playSong(){ 
    for (int i = 0; i < sizeof(song)/sizeof(Note); i++) { 
        setFrequency(song[i].frequency); 
        setDuration(song[i].beats); 
        TPM1_C0V = 0; // Stop the sound between notes 
        delay_ms(100);
    } 
} 
 
 
/* intiPWM() */ 
void initPWM(void) { 
    // Enable clock for Port B 
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; 
   
    // Configure PTB0 as TPM0_CH0 (Alt function 4 for PWM) 
    PORTB -> PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;  // Clear MUX field for PTB0 
    PORTB -> PCR[PTB0_Pin] |= PORT_PCR_MUX(4);     // Set PTB0 to TPM0_CH0 
   
    // Enable clock for TPM0 module (Timer/PWM Module 0)
    SIM -> SCGC6 |= SIM_SCGC6_TPM0_MASK; 
   
    // Select TPM clock source as MCGFLLCLK or MCGPLLCLK/2 
    SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;  // Clear clock source for TPM 
    SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);     // Set clock source to MCGFLLCLK 
   
    // Initialize TPM0 for PWM 
    TPM0 -> MOD = 0;  // Set modulation register to 0 to prevent initial buzzing 
    TPM0_C0V = 0;     // Set initial duty cycle for channel 0 to 0 
   
    // Set TPM0 timer to up-counting mode with a prescaler of 128 
    TPM0 -> SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear clock mode and prescaler 
    TPM0 -> SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set clock mode to '01' (up-counting) and prescaler to 128 
   
    // Ensure the timer is in up-counting PWM mode 
    TPM0 -> SC &= ~(TPM_SC_CPWMS_MASK);  // Ensure up-counting mode is selected 
   
    // Configure TPM0_CH0 for edge-aligned PWM (ELSxB = 1, MSxB = 1) 
    TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));  // Clear channel control 
    TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Set edge-aligned PWM with high-true pulses (ELSxB = 1, MSxB = 1) 
} 

 
 /*
int main() { 
    initPWM(); 
    playSong(); 
}*/