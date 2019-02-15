#ifndef __CC2500_H__ // include guard
#define __CC2500_H__

#define CC2500_IDLE    0x36      // Exit RX / TX, turn
#define CC2500_TX      0x35      // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_RX      0x34      // Enable RX. Perform calibration if enabled
#define CC2500_FTX     0x3B      // Flush the TX FIFO buffer. Only issue SFTX in IDLE or TXFIFO_UNDERFLOW states
#define CC2500_FRX     0x3A      // Flush the RX FIFO buffer. Only issue SFRX in IDLE or RXFIFO_OVERFLOW states
#define CC2500_TXFIFO  0x3F
#define CC2500_RXFIFO  0x3F

#define GDO0_PIN 4 // the number of the GDO0_PIN pin


void init_CC2500();

void WriteReg(char addr, char value);

char ReadReg(char addr);

void Read_Config_Regs(void);

char SendStrobe(char strobe);

#endif /* MY_CLASS_H */