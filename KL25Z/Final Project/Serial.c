#include "Constants.h"

volatile uint8_t serialData = 0; 
volatile uint8_t serialReady = 0;


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
	g_controls.forwardSpeed = GEAR_SPEED[g_controls.forwardGear];
	g_controls.turnSpeed = GEAR_SPEED[g_controls.turnGear];
	g_controls.stop = (motorControls == 0) ? 1:0;
	g_controls.complete = (motorControls == 0b1111) ? 1:0;
	serialReady = 1;
}

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  if (UART2->S1 & UART_S1_RDRF_MASK) {
		if (serialReady){
			serialReady = 0;
			serialData = UART2->D;
			osSemaphoreRelease(serialFlag);
			osDelay(10);
		}
  }
}

void UART_Receive_Poll() {
	while(!(UART2->S1 & UART_S1_RDRF_MASK));
	serialData = UART2->D;
}

void InitSerial(uint32_t baud_rate) {
	uint32_t divisor;

  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  PORTE->PCR[SERIAL_RX] &= ~PORT_PCR_MUX_MASK; 
  PORTE->PCR[SERIAL_RX] |= PORT_PCR_MUX(4);

  UART2->C2 &= ~UART_C2_RE_MASK;

  uint32_t BUS_CLOCK = DEFAULT_SYSTEM_CLOCK / 2; 
  divisor = BUS_CLOCK / (16 * baud_rate);
  UART2->BDH = UART_BDH_SBR(divisor >> 8);
  UART2->BDL = UART_BDL_SBR(divisor);

  UART2->C1 = UART2->S2 = UART2->C3 = 0;
  
  UART2->C2 |= UART_C2_RE_MASK;

  NVIC_SetPriority(UART2_IRQn, 2);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);
	
  UART2->C2 |= UART_C2_RIE_MASK; 
}
