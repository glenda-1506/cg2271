#include "MKL25Z4.h"     
#include "Constants.h"

// Function declarations for motor movement
void move_left_forward(uint8_t speed);
void move_left_reverse(uint8_t speed);
void move_right_forward(uint8_t speed);
void move_right_reverse(uint8_t speed);

// Initialize the motor control
void InitMotor() {
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
    SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;

    // Set motor pin muxing for PWM
    PORTB->PCR[MOTOR_FRONT_LEFT] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[MOTOR_FRONT_LEFT] |= PORT_PCR_MUX(3);

    PORTB->PCR[MOTOR_FRONT_RIGHT] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[MOTOR_FRONT_RIGHT] |= PORT_PCR_MUX(3);

    PORTB->PCR[MOTOR_BACK_LEFT] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[MOTOR_BACK_LEFT] |= PORT_PCR_MUX(3);

    PORTB->PCR[MOTOR_BACK_RIGHT] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[MOTOR_BACK_RIGHT] |= PORT_PCR_MUX(3);

    // Timer setup
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);

    // Timer configuration for TPM1
    TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
    TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
    TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

    // Set up channels for motor control
    TPM1_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    
    TPM1_C1SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM1_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

    // Timer configuration for TPM2
    TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));
    TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(TIMER_PRESCALER));
    TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

    // Set up channels for motor control
    TPM2_C0SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));

    TPM2_C1SC &= ~(TPM_CnSC_ELSB_MASK | TPM_CnSC_ELSA_MASK | TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);
    TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    
    // Set the period for the timers
    TPM1->MOD = 1000;
    TPM2->MOD = 1000;
}

// Define gear control values (assuming these are speed settings or similar)
uint8_t g_gearControl[4] = {25, 50, 75, 100}; // Updated to 0 for Gear 0, values for others
uint32_t g_vIn = 255;

// Function to handle motor control based on received data
void HandleMotorControl(uint8_t serialData) {
    // Extract motor control from the lower 4 bits (bits 0 to 3)
    motorControls = serialData & 0b00001111; // Keep the lower 4 bits
    checkfb = motorControls & 0b00000011;
    checklr = (motorControls >> 2) & 0b00000011;

    // Extract gear control from the upper 4 bits (bits 4 to 7)
    gearControls = (serialData >> 4) & 0b00001111; // Shift right by 4 and keep the lower 4 bits

    // Check gear control and set the corresponding gear speed
    uint8_t forwardGear = gearControls & 0b00000011; // Bits 4-5 for forward gear
    uint8_t turnGear = (gearControls >> 2) & 0b00000011; // Bits 6-7 for turn gear

    uint8_t forwardSpeed = g_gearControl[forwardGear];
    uint8_t turnSpeed = g_gearControl[turnGear];
    
    if (checkfb == 1) { // Move Forward
        MoveForward(forwardSpeed, turnSpeed, checklr);
    } else if (checkfb == 2) { // Move Backward
        MoveBackward(forwardSpeed, turnSpeed, checklr);
    } else if (motorControls == 0b0000) { // Stop
        // Stop all motors
        for (uint8_t i = 0; i < 4; i++) {
            //ledcWrite(MOTORS[i], 0);
        }
    } else if (motorControls == 0b1111){
      // Play Music
    }
}

bool MoveForward(uint8_t forwardSpeed, uint8_t turnSpeed, uint8_t checklr) {
        for (uint8_t i = 0; i < 4; i++) {
            uint32_t val = g_vIn; 
            if (i % 2 == 0) { 
                val = (uint32_t)(val * ((float)forwardSpeed / 100) *
                    (1 - ((float)turnSpeed / 100) * checklr & 0b00000001));
            } else { 
                val = (uint32_t)(val * ((float)forwardSpeed / 100) *
                    (1 - ((float)turnSpeed / 100) * (checklr >> 1) & 0b00000001));
            }
            //ledcWrite(MOTORS[i], val);
        }
        return true;
    }
    return false;

bool MoveBackward(uint8_t forwardSpeed, uint8_t turnSpeed) {
      for (uint8_t i = 4; i < 8; i++) {
          uint32_t val = g_vIn; 
          if (i % 2 == 0) { 
              val = (uint32_t)(val * ((float)forwardSpeed / 100) *
                  (1 - ((float)turnSpeed / 100) * checklr & 0b00000001));
          } else { 
              val = (uint32_t)(val * ((float)forwardSpeed / 100) *
                  (1 - ((float)turnSpeed / 100) * checklr >> 1) & 0b00000001));
          }
          //ledcWrite(MOTORS[i], val);
      }
      return true;
  }
  return false;

// Main function
int main(void) {
    InitSerial(9600); // Initialize serial communication with baud rate
    InitMotor(); // Initialize the motor control

    while (1) {
        // Main loop can perform other tasks if necessary
    }
}
