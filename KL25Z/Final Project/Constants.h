#ifndef CONSTANTS_H
#define CONSTANTS_H

// RTOS Stuff 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "MKL25Z4.h"    

//RTOX constructs
osSemaphoreId_t serialFlag;
osSemaphoreId_t motorFlag;
osSemaphoreId_t runningFlag;
osSemaphoreId_t endFlag;


// Define Serial Ports
#define SERIAL_RX 23 // PTE23

// Define all Led Pins <Any led pins from F6 onwards is port C>
#define F1 7   // PTC7
#define F2 0   // PTC0
#define F3 3   // PTC3
#define F4 4   // PTC4
#define F5 5   // PTC5
#define F6 6   // PTC6
#define F7 10  // PTC10
#define F8 11  // PTC11
#define F9 12  // PTC12
#define F10 13 // PTC13
#define B1 3   // PTB3

// Define Motor Constants
#define TIMER_PRESCALER 5
#define MOTOR_MOD_VALUE 30000 // assuming a 50kHz PWM Frequency
#define MAX_MOTOR_SPEED 100 // range 0 - 100
#define TURN_RADIUS 0.5 // range 0 - 1 (bigger value == larger turn radius)

// Indexes for MOTOR_WIRE Array
#define LEFT_RED     0
#define RIGHT_BLACK  1 
#define LEFT_BLACK   2 
#define RIGHT_RED    3

typedef struct {
	int pin;
	volatile uint32_t *channelStatusControl;
	volatile uint32_t *channelValue;
} MotorWire;

const MotorWire MOTOR_WIRE[4] = {
	{1, &TPM0_C0SC, &TPM0_C0V}, // PTC1 - LEFT_RED
	{2, &TPM0_C1SC, &TPM0_C1V}, // PTC2 - RIGHT_BLACK
	{8, &TPM0_C4SC, &TPM0_C4V}, // PTC8 - LEFT_BLACK
	{9, &TPM0_C5SC, &TPM0_C5V}  // PTC9 - RIGHT_RED
};

const uint8_t GEAR_SPEED[4] = {30,60,75,100};

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

// Define Buzzer Constants
#define PTB0_Pin 0 
#define BPM 240 
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
#define BEAT_DURATION (30000.0/BPM) //duration for an eighth note 
 
// Define all other constants
#define MASK(x) (1 << (x))
#define SW_POS 6 // PTD6
#define MAX_DELAY 0x80000

#endif // CONSTANTS_H

