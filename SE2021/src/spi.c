#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include "spi.h"

void SPI_Init(void) {
    LPC_PINCON->PINSEL0 |= 0x03<<30; //Pin P0.15 SCK

    LPC_PINCON->PINSEL1 |= 0x03<<2; //Pin P0.17 MISO
    LPC_PINCON->PINSEL1 |= 0x03<<4; //Pin P0.18 MOSI 

}

void SPI_ConfigTransfer(int frequency, int bitData, int mode) {
	LPC_SPI->SPCR = 0;
    
	if (bitData != 8 && bitData < 17 && bitData > 8) {
        LPC_SPI->SPCR |= 0b100; //BitEnable = 1;
        LPC_SPI->SPCR |= (bitData & 0xf) << 8;
    }
    
    LPC_SPI->SPCR |= (mode & 0b11) << 3;

    LPC_SPI->SPCR |= 1 << 5; //MSTR = 1; LSBF = 0; SPIE = 0
    
    // CREATE DIVIDER BASED ON FREQUENCY
    int divider = SystemCoreClock / frequency;
    divider = divider < 8 ? 8: divider;
    divider = divider%2 == 0 ? divider : (divider-1);
    
    LPC_SPI->SPCCR = divider;
}

int SPI_Transfer(unsigned short *txBuffer, unsigned short *rxBuffer, int lenght) {

    
    for (int i = 0; i < lenght; i++) {
        LPC_SPI->SPDR = txBuffer[i];	        //transfer
    
        while (!(LPC_SPI->SPSR & (1<<7))) {     //wait for transfer to end
            if ((LPC_SPI->SPSR & 0x1C) != 0)
                return LPC_SPI->SPSR & 0x1C;    // returns the error
        }	

        rxBuffer[i] = LPC_SPI->SPDR;
    }

    return 0;
}
