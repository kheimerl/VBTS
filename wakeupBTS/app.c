/*
 * wakeupBTS main application.
 * (TI's MSP430 with CC1150 Transmitter)
 */

#include <signal.h>
#include "app.h"

extern char paTable[];
extern char paTableLen;

char txBuffer[4]; 

int main (void){

  WDTCTL = WDTPW + WDTHOLD;     /* Stop Watchdog Timeout. */
  
  /* 5ms delay to compensate for time to startup between 
   * MSP430 and CC1100/2500 [slaa325]
   */
  __delay_cycles(5000);
  
  TI_CC_SPISetup();             /* Initialize SPI port */
  TI_CC_PowerupResetCCxxxx();   /* Reset CCxxxx */
  writeRFSettings();            /* Write to RF configuration regs. */

  TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, 
			 paTable, paTableLen);

  /* Setup PINs and enable interrupts */

  P1DIR |= (LED1 + LED2) ; /* Declare output P1.0 and P1.1 
			    * (Leds on EzMSP430 board) 
			    */
  P1DIR &= ~(BUTTON); /* Declare input P1.2 (Button) */
  P1IE  |= (BUTTON);  /* Enable interrupts on P1.2 */

  /* Enter LMP3 and enable interrupts. */
  __bis_SR_register(LPM3_bits + GIE); 

  /* Never executed!. */
  return 1;
}

/*
 * Interrupt service routine when button is pressed.
 */

interrupt (PORT1_VECTOR) Port1_ISR(void) {

  /* Make sure switch was indeed pressed. */

  if ( P1IFG ){

    /* Build packet */
    txBuffer[0] = 2;                 /* Packet length */
    txBuffer[1] = 0x01;              /* Packet address */
    txBuffer[2] = 
      (~TI_CC_SW_PxIFG << 1) & 0x02; /* Load switch inputs */
    RFSendPacket(txBuffer, 3);       /* Send value over RF */
    __delay_cycles(5000);            /* Switch debounce */
  }

  TI_CC_SW_PxIFG &= ~(TI_CC_SW1);    /* Clr flag that caused int */

  /* Toggle Leds and Reset interrupt. */
  P1IFG &= ~(BUTTON);  
  P1OUT ^= (LED1 + LED2); 
}
