
#include "app.h"
#include "cc_registers.h"

/*
 * writeRFSettings --
 * Setup TX registers. Values are obtained from TI's SmartRF04 development kit.
 */

void writeRFSettings (void) {

  TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG2,   0x0B); /* GDO2 output pin config    */
  TI_CC_SPIWriteReg(TI_CCxxx0_IOCFG0,   0x06); /* GDO0 output pin config    */
  TI_CC_SPIWriteReg(TI_CCxxx0_PKTLEN,   0xFF); /* Packet length             */
  TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL1, 0x05); /* Packet automation control */
  TI_CC_SPIWriteReg(TI_CCxxx0_PKTCTRL0, 0x05); /* Packet automation control */
  TI_CC_SPIWriteReg(TI_CCxxx0_ADDR,     0x01); /* Device address            */
  TI_CC_SPIWriteReg(TI_CCxxx0_CHANNR,   0x00); /* Channel number            */

  TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL1,  0x06); /* Freq synthesizer control */
  TI_CC_SPIWriteReg(TI_CCxxx0_FSCTRL0,  0x00); /* Freq synthesizer control */

  TI_CC_SPIWriteReg(TI_CCxxx0_FREQ2,    0x22); /* Freq control word, high byte */
  TI_CC_SPIWriteReg(TI_CCxxx0_FREQ1,    0x9F); /* Freq control word, mid byte  */
  TI_CC_SPIWriteReg(TI_CCxxx0_FREQ0,    0x82); /* Freq control word, low byte  */


  TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG4,  0xF5); /* Modem configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG3,  0x83); /* Modem configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG2,  0x12); /* Modem configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG1,  0x22); /* Modem configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_MDMCFG0,  0xF8); /* Modem configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_DEVIATN,  0x15); /* Modem dev (when FSK mod en) */

  TI_CC_SPIWriteReg(TI_CCxxx0_MCSM2,   0x07); /* MainRadio Cntrl State Machine */
  TI_CC_SPIWriteReg(TI_CCxxx0_MCSM1,   0x30); /* MainRadio Cntrl State Machine */
  TI_CC_SPIWriteReg(TI_CCxxx0_MCSM0,   0x18); /* MainRadio Cntrl State Machine */
  
  TI_CC_SPIWriteReg(TI_CCxxx0_FOCCFG,   0x16); /* Freq Offset Compens. Config */
  TI_CC_SPIWriteReg(TI_CCxxx0_BSCFG,    0x6C); /*  Bit synchronization config */
  
  TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL2, 0x03); /* AGC control */
  TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL1, 0x40); /* AGC control */
  TI_CC_SPIWriteReg(TI_CCxxx0_AGCCTRL0, 0x91); /* AGC control */

  TI_CC_SPIWriteReg(TI_CCxxx0_FREND1,   0xB6); /* Front end RX configuration */
  TI_CC_SPIWriteReg(TI_CCxxx0_FREND0,   0x10); /* Front end RX configuration */
  
  TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL3,   0xE9); /* Frequency synthesizer cal */
  TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL2,   0x2A); /* Frequency synthesizer cal */
  TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL1,   0x00); /* Frequency synthesizer cal */
  TI_CC_SPIWriteReg(TI_CCxxx0_FSCAL0,   0x1F); /* Frequency synthesizer cal */
  TI_CC_SPIWriteReg(TI_CCxxx0_FSTEST,   0x59); /* Frequency synthesizer cal */
  
  TI_CC_SPIWriteReg(TI_CCxxx0_TEST2,    0x88); /* Various test settings */
  TI_CC_SPIWriteReg(TI_CCxxx0_TEST1,    0x31); /* Various test settings */
  TI_CC_SPIWriteReg(TI_CCxxx0_TEST0,    0x0B); /* Various test settings */
}

/* PATABLE (0 dBm output power) */
extern char paTable[] = {0x60};
extern char paTableLen = 1;

/* 
 * RFSendPacket --
 * Transmit packet (63 bytes). 
 *
 * To use this function, GD00 must be configured to be asserted when sync word 
 * is sent and de-asserted at the end of the packet, which is accomplished by
 * setting the IOCFG0 register to 0x06, per the CCxxxx datasheet.  GDO0 goes high at
 * packet start and returns low when complete.  The function polls GDO0 to
 * ensure packet completion before returning. [slaa325]
 */

void RFSendPacket (char *txBuffer, char size) {
  
  TI_CC_SPIWriteBurstReg(TI_CCxxx0_TXFIFO, txBuffer, size); /* Write TX data */
  
  TI_CC_SPIStrobe(TI_CCxxx0_STX);   /* Change state to TX, initiating data transfer */
  
  while (!(TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN));
  
  /* Wait till the packet transmission has been confirmed. */
  while (TI_CC_GDO0_PxIN&TI_CC_GDO0_PIN);
  
  /* Wait GDO0 to clear -> end of pkt. After pkt TX, this flag is set.
   * The flag must be cleared out before exiting the ISR. 
   */
  TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;      
  
}

