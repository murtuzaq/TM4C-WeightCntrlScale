/*
 * tm4c_spi2.c
 *
 *  Created on: Apr 4, 2017
 *      Author: Murtuza
 */

#include <stdint.h>

#define SYSCTL_RCGCSSI_R			(*((volatile uint32_t *)0x400FE61C))
#define SYSCTL_RCGCGPIO_R			(*((volatile uint32_t *)0x400FE608))
#define GPIO_PORTB_AFSEL_R			(*((volatile uint32_t *)0x40005420))
#define GPIO_PORTB_PCTL_R			(*((volatile uint32_t *)0x4000552C))
#define GPIO_PORTB_DEN_R			(*((volatile uint32_t *)0x4000551C))
#define GPIO_PORTB_PUR_R			(*((volatile uint32_t *)0x40005510))
#define SSI2_CR0_R					(*((volatile uint32_t *)0x4000A000))
#define SSI2_CR1_R					(*((volatile uint32_t *)0x4000A004))
#define SSI2_DR_R					(*((volatile uint32_t *)0x4000A008))
#define SSI2_SR_R					(*((volatile uint32_t *)0x4000A00C))
#define SSI2_CPSR_R					(*((volatile uint32_t *)0x4000A010))
#define SSI2_CC_R					(*((volatile uint32_t *)0x4000AFC8))

void SPI2_Init(void)
{
  //15.4 Configure SSI Module 2 (spi2), Pins: PB4,PB6,PB7
  //--------------------------------------------------------------
  //1. Enable the SSI module using the RCGCSSI register
  SYSCTL_RCGCSSI_R |= 0x04;  //Enable Run Mode Clock to SSI Module 2
  //2. Enable the clock to the GPIO module via the RCGCGPIO register
  SYSCTL_RCGCGPIO_R |= 0x02; //Enable GPIO Port B;
  //3. Set the GPIO AFSEL bits for the appropriate pins
  GPIO_PORTB_AFSEL_R |=0x10;  //PB4 (CLK)
  GPIO_PORTB_AFSEL_R |=0x40;  //PB6 (MISO)
  GPIO_PORTB_AFSEL_R |=0x80;  //PB7 (MOSI)
  //4. Configure the PMCn fields in the GPIOPCTL register to SSI signals
  GPIO_PORTB_PCTL_R &= ~(0xF <<16U); //Clear bits
  GPIO_PORTB_PCTL_R |= (2 <<16U); //PB4 //SPI_CLK
  GPIO_PORTB_PCTL_R &= ~(0xF <<24U); //Clear bits
  GPIO_PORTB_PCTL_R |= (2 <<24U); //PB6 //SPI_RX
  GPIO_PORTB_PCTL_R &= ~(0xF <<28U); //Clear bits
  GPIO_PORTB_PCTL_R |= (2 <<28U); //PB7 //SPI_TX
 //5. Program the GPIODEN register to enable the pin's digital function.
 //   Configure, the drive strength, drain select and pull-up/pull-down
  GPIO_PORTB_DEN_R |=0x10;  //PB4
  GPIO_PORTB_DEN_R |=0x40;  //PB6
  GPIO_PORTB_DEN_R |=0x80;  //PB7

  GPIO_PORTB_PUR_R |=0x10; //pull up on CLK.

 //1. Clear SSE bit in the SSICR1 register before making any changes.
  SSI2_CR1_R &= ~(0x02);
 //2. Select whether the SSI is a master or slave:
  SSI2_CR1_R = 0x00000000;  //configure SS1 as master mode;
 //3. Configure the SSI clock source by writing to the SSICC register.
  SSI2_CC_R &= ~(0x0F);//System Clock based on clock source and divisior factor
 //4. Configure the clock prescale divisor by writing the SSICPSR register.
 //SSInCLK = SysClk/(CPSDVSR *(1+SCR)) //SPI Clk Speed - (16Mhz/(4*1))= 4Mhz
  SSI2_CPSR_R = 4;  //Divde by 4;  //Set the CPSDVSR
 //5. Write the SSICR0 register with the following configuration:
 //¦ Serial clock rate (SCR)
  SSI2_CR0_R &= ~(0xFF << 8U); //Clear data bits;
  SSI2_CR0_R |= (0x00 << 8U); // SCR = 0;
 //¦ Desired clock phase/polarity, if using Freescale SPI mode (SPH and SPO)
  SSI2_CR0_R &= ~(0x1 << 7U);  //Data is captured on the first clock edge
 //¦ The protocol mode: Freescale SPI, TI SSF, MICROWIRE (FRF)
  SSI2_CR0_R &= ~(0x3 << 4U); //Clear data bits;
  SSI2_CR0_R |= (0x0 << 4U); //Select Freescale SPI Frame Format
 //¦ The data size (DSS)
  SSI2_CR0_R |= (0x7 << 0U); //8 - Bit data;


 //7. Enable the SSI by setting the SSE bit in the SSICR1 register.
  SSI2_CR1_R |= 0x02; //ENABLE SSI;
}

uint8_t SPI2_RdWt(uint8_t data)
{

   uint8_t u8data;


   //Wait for Transmit buffer to empty
    while((SSI2_SR_R & 0x02) == 0){}
    SSI2_DR_R  = data;

   //wait for Receive buffer to fill up
    while((SSI2_SR_R & 0x04) == 0){}
    u8data = SSI2_DR_R;

    return(u8data);

}



