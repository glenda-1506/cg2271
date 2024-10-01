#include "MKL25Z4.h"  
#include "Constants.h"

void UART2_IRQHandler(void) {
  NVIC_ClearPendingIRQ(UART2_IRQn);
	
  if (UART2->S1 & UART_S1_RDRF_MASK) {
    uint32_t serialData = UART2->D;
  }
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

  NVIC_SetPriority(UART2_IRQn, 128);
  NVIC_ClearPendingIRQ(UART2_IRQn);
  NVIC_EnableIRQ(UART2_IRQn);
	
  UART2->C2 |= UART_C2_RIE_MASK; 
}
