
#include "cc_registers.h"
#include "spi.h"
#include "TI_CC_msp430.h"
#include "wakeup_board.h"

#define LED1   0x01
#define LED2   0x02
#define BUTTON 0x04

/* function headers, defined in cc_transmitter.c */
void writeRFSettings(void);
void RFSendPacket(char *, char);
char RFReceivePacket(char *, char *);

 
