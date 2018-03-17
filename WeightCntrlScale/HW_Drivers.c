#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "HW_Drivers.h"
#include "FreeRTOS\Source\include\FreeRTOS.h"
#include "FreeRTOS\Source\include\task.h"
#include "FreeRTOS\Source\include\queue.h"
#include "FreeRTOS\Source\include\semphr.h"


unsigned int Tick100ms = 0;


void Configure_Clock();
void Configure_Pin_PB7_GPIO(void);
void Configure_Pin_PB7_SSI2TX(void);
void Configure_SDCARD_CS();

void Configure_Timer_0_A_INT(void); //DEVICE LOOP TIMER
void Configure_Timer_1_A_INT(void); //DEVICE RTC
void Configure_Timer_2_A_INT(void); //CONTROLLER LOOP TIMER
void Configure_Timer_3_A_INT(void);	//SLC Pulse Out Timer
void Configure_Timer_4_A_INT(void); //SLC Monitor Timer

void Clear_Timer2A_INT_FLAG(void);
void Clear_Timer4A_INT_FLAG(void);

void Configure_EEPROM(void);

void ConfigureSPI_SS2(void);

void Configure_Pin_PA0_GPIO(void);
void Configure_Pin_PA7_GPIO_OUT(void); //EN_PWR_SLC
void Configure_Pin_PB2_GPIO_OUT(void); //DISPLAY_PWRDN_O
void Configure_Pin_PB5_GPIO_OUT(void); //DISPLAY_SPI_CS

void Configure_Pin_PD4_GPIO_OUT(void);
void Configure_Pin_PD4_GPIO_IN(void);	//SLC_CNTRL_PULSE

void Configure_Pin_PF1_GPIO_OUT(void);  //DIS_BAT_CHARGE
void Configure_Pin_PF2_GPIO_OUT(void); // Enable SLC 36V
void Configure_Pin_PF3_GPIO_OUT(void);  //SLC_CURRENT_TX




void UnitSWTimer(void);

unsigned char ReadRD4(void);

void Configure_HW(void)
{
 // Configure_Clock();
	PLL_Init();

  Configure_Pin_PB5_GPIO_OUT();//display_chipselect
  Configure_Pin_PB2_GPIO_OUT(); //display_powerDown

  Configure_Pin_PF1_GPIO_OUT(); //DIS_BAT_CHARGE
  Configure_Pin_PF2_GPIO_OUT();  //Enable SLC 36V

  //ENABLE_SLC_POWER is replaced with SLC_CNTRL_PULSE
  //Configure_Pin_PD4_GPIO_OUT();  //ENABLE_SLC_Power
  Configure_Pin_PD4_GPIO_IN();

  ConfigureSPI_SS2();             //Configure Display SPI
  //Configure_Timer_0_A();          //Set Timer to handle 1ms tick

  Configure_EEPROM();

  GPIO_PORTB_DATA_R |= (1 <<2U);  //Set Display Power Down to high (PB2)
  GPIO_PORTB_DATA_R |= (1 <<5U);  //Set Display CS to high (PA4)

  Configure_Timer_1_A_INT();

  Configure_Pin_PB1_GPIO_OUT();

 }


void Configure_Clock()
{
  
  //5.3 System Control, Clock Configuration - 50MHZ
  //---------------------------------------------------------------------//
  //The PLL is configured using direct register writes to the RCC/RCC2 register.
  //1. Bypass the PLL and system clock divider by setting the BYPASS bit and 
  //   clearing the USESYS bit in the RCC register, this configures the 
  //   microcontroller to run off a "raw" clock source
  //---Skip to 2, as we are not bypassing PLL
   
  //2. Select the crystal value (XTAL) and oscillator source (OSCSRC), 
  //   and clear the PWRDN bit.  
  SYSCTL_RCC_R &= ~(0x1F << 6); //CLEAR XTAL CLOCK BITS
  SYSCTL_RCC_R |= (0x15 << 6);  //SET XTAL to 16 Mhz
  SYSCTL_RCC_R &= ~(0x3 << 4);  //Select Main Oscillator
  SYSCTL_RCC_R &= ~(0x1 << 0U); //Main Oscillator is enabled.
  SYSCTL_RCC_R &= ~(0x1 << 13); //PLL is operating normally; (clear Power Dwn)
 
  
  //3. Select system divider (SYSDIV) in RCC/RCC2 and set the USESYS bit in RCC. 
  SYSCTL_RCC_R &= ~(0xF << 23); //CLEAR SYS DIVIDE bits
  SYSCTL_RCC_R |= (0x3 << 23);  //select Divide by 4
  SYSCTL_RCC_R |= (0x1 << 22);  //sys clk divider is the source for the clock
 
  
  //4. Wait, and Poll PLLLRIS bit when PLL is ready
  while((SYSCTL_RIS_R & (0x1 << 6U)) != (0x1 << 6U)) {}
  //5. Enable use of the PLL by clearing the BYPASS bit in RCC/RCC2.
  SYSCTL_RCC_R &= ~(0x1 << 11); //Clear PLL Bypass. System Clock is PLL out.
 
  //---------------------------------------------------------------------//
}

void ConfigureSPI_SS0()
{
  //15.4 Configure SSI Module 0 (spi0), Pins: PA2,PA4,PA5
  //--------------------------------------------------------------
  //1. Enable the SSI module using the RCGCSSI register
  SYSCTL_RCGCSSI_R |= 0x01;  //Enable Run Mode Clock to SSI Module 0
  //2. Enable the clock to the GPIO module via the RCGCGPIO register
  SYSCTL_RCGCGPIO_R |= 0x01; //Enable GPIO Port A;
  //3. Set the GPIO AFSEL bits for the appropriate pins 
  GPIO_PORTA_AFSEL_R |=0x04;  //PA2 (CLK)
  GPIO_PORTA_AFSEL_R |=0x10;  //PA4 (MISO)
  GPIO_PORTA_AFSEL_R |=0x20;  //PA5 (MOSI)
  //4. Configure the PMCn fields in the GPIOPCTL register to SSI signals
  GPIO_PORTA_PCTL_R &= ~(0xF <<8U); //Clear bits
  GPIO_PORTA_PCTL_R |= (2 <<8U); //PA2 //SPI_CLK
  GPIO_PORTA_PCTL_R &= ~(0xF <<16U); //Clear bits
  GPIO_PORTA_PCTL_R |= (2 <<16U); //PA4 //SPI_RX
  GPIO_PORTA_PCTL_R &= ~(0xF <<20U); //Clear bits
  GPIO_PORTA_PCTL_R |= (2 <<20U); //PA5 //SPI_TX
 //5. Program the GPIODEN register to enable the pin's digital function. 
 //   Configure, the drive strength, drain select and pull-up/pull-down 
  GPIO_PORTA_DEN_R |=0x04;  //PA2
  GPIO_PORTA_DEN_R |=0x10;  //PA4
  GPIO_PORTA_DEN_R |=0x20;  //PA5
  
  GPIO_PORTA_PUR_R |=0x04; //pull up on CLK.
  GPIO_PORTA_PUR_R |=0x10; //pull up on RX

  
 //1. Clear SSE bit in the SSICR1 register before making any changes.
  SSI0_CR1_R &= ~(0x02);
 //2. Select whether the SSI is a master or slave:
  SSI0_CR1_R = 0x00000000;  //configure SS1 as master mode;
 //3. Configure the SSI clock source by writing to the SSICC register.
  SSI0_CC_R &= ~(0x0F);//System Clock based on clock source and divisior factor
 //4. Configure the clock prescale divisor by writing the SSICPSR register.
 //SSInCLK = SysClk/(CPSDVSR *(1+SCR)) //SPI Clk Speed - (16Mhz/(4*1))= 4Mhz
  SSI0_CPSR_R = 4;  //Divde by 4;  //Set the CPSDVSR
 //5. Write the SSICR0 register with the following configuration:
 //¦ Serial clock rate (SCR)
  SSI0_CR0_R &= ~(0xFF << 8U); //Clear data bits;
  SSI0_CR0_R |= (0x00 << 8U); // SCR = 0;
 //¦ Desired clock phase/polarity, if using Freescale SPI mode (SPH and SPO)
  SSI0_CR0_R &= ~(0x1 << 7U);  //Data is captured on the first clock edge
 //¦ The protocol mode: Freescale SPI, TI SSF, MICROWIRE (FRF)
  SSI0_CR0_R &= ~(0x3 << 4U); //Clear data bits;
  SSI0_CR0_R |= (0x0 << 4U); //Select Freescale SPI Frame Format
 //¦ The data size (DSS)
  SSI0_CR0_R |= (0x7 << 0U); //8 - Bit data;
  
  
 //7. Enable the SSI by setting the SSE bit in the SSICR1 register.
  SSI0_CR1_R |= 0x02; //ENABLE SSI;
}


void ConfigureSPI_SS2(void)
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

void Configure_Pin_PA0_GPIO(void)
{
  SYSCTL_RCGCGPIO_R |= 0x01; //Enable GPIO Port A;
  
  GPIO_PORTA_DIR_R |= (1 <<0U);         // Enable Bit 3 on PORT A (SDCARD CS)
  
  GPIO_PORTA_AFSEL_R &= ~0x01;  		//CLEAR AFSEL bit
  
  GPIO_PORTA_LOCK_R = 0x4C4F434B;       //unlock code on PORT A;
  GPIO_PORTA_PCTL_R &= ~(0xFF <<0U);   //Overwrite Control on PA3 to GPIO
  GPIO_PORTA_LOCK_R = 0; //relock.      //lock code on PORT A;
  
}

void Configure_Pin_PA5_SSITX(void)
{
  SYSCTL_RCGCGPIO_R |= 0x01; //Enable GPIO Port A;
  
  GPIO_PORTA_AFSEL_R |=0x20;  //PA5 (MOSI)
  
  GPIO_PORTA_LOCK_R = 0x4C4F434B;       //unlock code on PORT A;
  
  GPIO_PORTA_PCTL_R &= ~(0xF <<20U); //Clear bits
  GPIO_PORTA_PCTL_R |= (2 <<20U); //PA5 //SPI_TX

  GPIO_PORTA_LOCK_R = 0; //relock.      //lock code on PORT A;
  
  GPIO_PORTA_DEN_R |=0x20;  //PA5
  
}


void Configure_Pin_PA5_GPIO(void)
{
  SYSCTL_RCGCGPIO_R |= 0x01; //Enable GPIO Port A;
  
  GPIO_PORTA_DIR_R |= (1 <<5U);         // Enable Bit 3 on PORT A (SDCARD CS)
  
  GPIO_PORTA_AFSEL_R &= ~0x20;  //CLEAR AFSEL bit
  
  GPIO_PORTA_LOCK_R = 0x4C4F434B;       //unlock code on PORT A;
  GPIO_PORTA_PCTL_R &= ~(0xFF <<20U);   //Overwrite Control on PA3 to GPIO
  GPIO_PORTA_LOCK_R = 0; //relock.      //lock code on PORT A;
  
}

void Configure_Pin_PB7_GPIO(void)
{
  SYSCTL_RCGCGPIO_R |= 0x02; //Enable GPIO Port B;
  GPIO_PORTB_AFSEL_R &=~0x80;  //PB7 Clear AFSEL bit;
}

void Configure_Pin_PB7_SSI2TX(void)
{
  SYSCTL_RCGCGPIO_R |= 0x02; //Enable GPIO Port B;
  GPIO_PORTB_AFSEL_R |= 0x80;  //Set AFSEL bit
}


void Configure_SDCARD_CS()
{
  SYSCTL_RCGCGPIO_R |= (1 << 0U);  //Enable Clock for PORT A
  
  GPIO_PORTA_DIR_R |= (1 <<3U);         // Enable Bit 3 on PORT A (SDCARD CS)
  
  GPIO_PORTA_LOCK_R = 0x4C4F434B;       //unlock code on PORT A;
  GPIO_PORTA_PCTL_R &= ~(0xFF <<12U);   //Overwrite Control on PA3 to GPIO
  GPIO_PORTA_LOCK_R = 0; //relock.      //lock code on PORT A;
  
  GPIO_PORTA_DEN_R |= (1 <<3U);         //Enable Bit 3 on PORT A as DIGITAL IO
}

// EN_PWR_SLC
void Configure_Pin_PA7_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 0U);  //Enable Clock for PORT A
  
  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTA_DIR_R |= (1 <<7U);         //Enable Bit 7 on PORT A
  
  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTA_DEN_R |= (1 <<7U);         //Enable Bit 7 on PORT A as DIGITAL IO
}

void Configure_Pin_PB1_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 1U);  //Enable Clock for PORT B

  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTB_DIR_R |= (1 <<1U);         //Enable Bit 2 on PORT B

  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTB_DEN_R |= (1 <<1U);         //Enable Bit 2 on PORT B as DIGITAL IO
}


//Display PWRDN_O PIN
void Configure_Pin_PB2_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 1U);  //Enable Clock for PORT B
  
  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTB_DIR_R |= (1 <<2U);         //Enable Bit 2 on PORT B
  
  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTB_DEN_R |= (1 <<2U);         //Enable Bit 2 on PORT B as DIGITAL IO
}

//Display SPI CS PIN
void Configure_Pin_PB5_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 1U);  //Enable Clock for PORT B
  
  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTB_DIR_R |= (1 <<5U);         //Enable Bit 5 on PORT B
  
  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTB_DEN_R |= (1 <<5U);         //Enable Bit 5 on PORT B as DIGITAL IO
}

void Configure_Pin_PD0_GPIO_OUT(void)
{
	//1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U);  //Enable Clock for PORT D

	//2) Set the direction of the GPIO port pins by programming the GPIODIR
	GPIO_PORTD_DIR_R |= (1 <<0U);         //Enable Bit 0 on PORT D

	//6) Enable GPIO pin as digital IO in GPIODEN
	GPIO_PORTD_DEN_R |= (1 <<0U);         //Enable Bit 0 on PORT D as DIGITAL IO
}

void Configure_Pin_PD2_GPIO_OUT(void)
{
	//1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U);  //Enable Clock for PORT D

	//2) Set the direction of the GPIO port pins by programming the GPIODIR
	GPIO_PORTD_DIR_R |= (1 <<2U);         //Enable Bit 0 on PORT D

	//6) Enable GPIO pin as digital IO in GPIODEN
	GPIO_PORTD_DEN_R |= (1 <<2U);         //Enable Bit 0 on PORT D as DIGITAL IO
}

void Configure_Pin_PD4_GPIO_OUT(void)
{
	//1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U);  //Enable Clock for PORT D

	//2) Set the direction of the GPIO port pins by programming the GPIODIR
	GPIO_PORTD_DIR_R |= (1 <<4U);         //Enable Bit 4 on PORT D

	//6) Enable GPIO pin as digital IO in GPIODEN
	GPIO_PORTD_DEN_R |= (1 <<4U);         //Enable Bit 4 on PORT D as DIGITAL IO
}

void Configure_Pin_PD4_GPIO_IN(void)
{
	//1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U);  //Enable Clock for PORT D

	//2) Set the direction of the GPIO port pins by programming the GPIODIR
	GPIO_PORTD_DIR_R &= ~(1 <<4U);         //Clear Bit 4 on PORT D

	//6) Enable GPIO pin as digital IO in GPIODEN
	GPIO_PORTD_DEN_R |= (1 <<4U);         //Enable Bit 4 on PORT D as DIGITAL IO
}


void Pin_PD4_GPIO_OUT_HIGH(void)
{
	GPIO_PORTD_DATA_R |= (1 <<4U);
}

void Pin_PD4_GPIO_OUT_LOW(void)
{
	GPIO_PORTD_DATA_R &= ~(1 <<4U);
}

// FSLT Rev E Removes connection to ENABLE_SLC_PWR
//void Configure_SLC_PWR_OUT(void)
//{
//	Configure_Pin_PD4_GPIO_OUT();
//}

//void SLC_PWR_HIGH(void)
//{
//	Pin_PD4_GPIO_OUT_HIGH();
//}

//void SLC_PWR_LOW(void)
//{
//	Pin_PD4_GPIO_OUT_LOW();
//}


//DIS_BAT_CHARGE
void Configure_Pin_PF1_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 5U);  //Enable Clock for PORT F

  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTF_DIR_R |= (1 <<1U);         //Enable Bit 1 on PORT F

  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTF_DEN_R |= (1 <<1U);         //Enable Bit 1 on PORT F as DIGITAL IO

  //initilize startup position;
  GPIO_PORTF_DATA_R &= ~(1 << 1U);
}


//ENABLE_SLC_36V
void Configure_Pin_PF2_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 5U);  //Enable Clock for PORT F
  
  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTF_DIR_R |= (1 <<2U);

  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTF_DEN_R |= (1 <<2U);

  //initilize startup position;
  GPIO_PORTF_DATA_R &= ~(1 << 2U);
}

//SLC_CURRENT_TX
void Configure_Pin_PF3_GPIO_OUT(void)
{
  //1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
  SYSCTL_RCGCGPIO_R |= (1 << 5U);  //Enable Clock for PORT F

  //2) Set the direction of the GPIO port pins by programming the GPIODIR
  GPIO_PORTF_DIR_R |= (1 <<3U);         //Enable Bit 1 on PORT F
  
  //6) Enable GPIO pin as digital IO in GPIODEN
  GPIO_PORTF_DEN_R |= (1 <<3U);         //Enable Bit 1 on PORT F as DIGITAL IO

  //initilize startup position;
  GPIO_PORTF_DATA_R &= ~(1 << 3U);
}


void Pin_PB1_GPIO_OUT_HIGH(void)
{
#define GPIOB_BIT1_BYTE_OFFSET		 0x53FC
#define GPIOB_BIT1_BIT_OFFSET		((GPIOB_BIT1_BYTE_OFFSET)*32 + 1*4)
#define ALIAS_GPIOB_BIT1			(0x42000000 + GPIOB_BIT1_BIT_OFFSET)
//#define PORTBBit1					(*((volatile unsigned long*) 0x420A7F84))
#define PORTBBit1					(*((volatile unsigned long*) ALIAS_GPIOB_BIT1))

	//GPIO_PORTB_DATA_R |= (1 <<1U);
	PORTBBit1 = 0x1;

}

void Pin_PB1_GPIO_OUT_LOW(void)
{
	GPIO_PORTB_DATA_R &= ~(1 <<1U);
}

void Pin_PB1_GPIO_OUT_TOG(void)
{
	//GPIO_PORTB_DATA_R ^= (1 <<1U);
	//PORTBBit1 ^= 1;
	PB1 ^= 1;
}


void Pin_PF1_GPIO_OUT_HIGH(void)
{
	GPIO_PORTF_DATA_R |= (1 <<1U);
}

void Pin_PF1_GPIO_OUT_LOW(void)
{
	GPIO_PORTF_DATA_R &= ~(1 <<1U);
}

void Pin_PF2_GPIO_OUT_HIGH(void)
{
	GPIO_PORTF_DATA_R |= (1 <<2U);
}

void Pin_PF2_GPIO_OUT_LOW(void)
{
	GPIO_PORTF_DATA_R &= ~(1 <<2U);
}

void Pin_PF3_GPIO_OUT_HIGH(void)
{
	GPIO_PORTF_DATA_R |= (1 <<3U);
}

void Pin_PF3_GPIO_OUT_LOW(void)
{
	GPIO_PORTF_DATA_R &= ~(1 <<3U);
}


void Configure_SLC_33V_OUT(void)
{
	 Configure_Pin_PF2_GPIO_OUT();
}

void BATCharger_Disable(void)
{
	Pin_PF1_GPIO_OUT_HIGH();
}

void BATCharger_Enable(void)
{
	Pin_PF1_GPIO_OUT_LOW();
}
void SLC_33V_HIGH(void)
{
	Pin_PF2_GPIO_OUT_HIGH();
}

void SLC_33V_LOW(void)
{
	Pin_PF2_GPIO_OUT_LOW();
}

void SLC_IPULSE_LOW(void)
{
	Pin_PF3_GPIO_OUT_LOW();
}

void SLC_IPULSE_HIGH(void)
{
	Pin_PF3_GPIO_OUT_HIGH();
}

void SLC_IPULSE_TOGGLE(void)
{
	GPIO_PORTF_DATA_R ^= (1 <<3U);
}

void Configure_Timer_0_A_INT(void)
{
	  //11.4 TIMERS (Timer0 A)
	  SYSCTL_RCGCTIMER_R |= 0x01;
	  //11.4.1 One/Shot Periodic Timer Mode
	  //1) Clear the TnEN bit in the GPTMCTL register
	  TIMER0_CTL_R &=  ~(0x01);
	  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
	  TIMER0_CFG_R  = 0x00000000;
	  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	      a. Write a value of 0x1 for One-Shot mode.
	      b. Write a value of 0x2 for Periodic mode
	  */
	  TIMER0_TAMR_R = 0x02;
	 // TIMER0_TBMR_R = 0x02;
	  //4 -----skip----
	  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
	  TIMER0_TAILR_R = 1600;
	  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER0_IMR_R |= (0x1<<0U); //Enable Timer A Time-Out Interrupt
	  NVIC_EN0_R |= (0x1 <<19U);  //Enable Nested Vector Interrupt Control for ISR
	  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.

	  TIMER0_CTL_R |=  (0x01);
}

void Configure_Timer_1_A_INT(void)
{
	  //11.4 TIMERS (Timer1 A)
	  SYSCTL_RCGCTIMER_R |= 0x02;
	  //11.4.1 One/Shot Periodic Timer Mode
	  //1) Clear the TnEN bit in the GPTMCTL register
	  TIMER1_CTL_R &=  ~(0x01);
	  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
	  TIMER1_CFG_R  = 0x00000000;
	  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	      a. Write a value of 0x1 for One-Shot mode.
	      b. Write a value of 0x2 for Periodic mode
	  */
	  TIMER1_TAMR_R = 0x02;
	  //4 -----skip----
	  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
	  TIMER1_TAILR_R = 1600000; // //100ms Resolution;
	  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER1_IMR_R |= (0x1<<0U); //Enable Timer A Time-Out Interrupt
	  NVIC_EN0_R |= (0x1 <<21U);  //Enable Nested Vector Interrupt Control for ISR
	  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.

	  TIMER1_CTL_R |=  (0x01);
}

void Configure_Timer_2_A_INT(void)
{
	  //11.4 TIMERS (Timer2 A)
	  SYSCTL_RCGCTIMER_R |= 0x04; //Enable GP Timer Module 0 in Run Mode
	  //11.4.1 One/Shot Periodic Timer Mode
	  //1) Clear the TnEN bit in the GPTMCTL register
	  TIMER2_CTL_R &=  ~(0x01);
	  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
	  TIMER2_CFG_R  = 0x00000000;
	  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	      a. Write a value of 0x1 for One-Shot mode.
	      b. Write a value of 0x2 for Periodic mode
	  */
	  TIMER2_TAMR_R = 0x02;
	  //4 -----skip----
	  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
	  TIMER2_TAILR_R = 1600;
	  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER2_IMR_R |= (0x1<<0U); //Enable Timer A Time-Out Interrupt
	  NVIC_EN0_R |= (0x1 <<23U);  //Enable Nested Vector Interrupt Control for ISR
	  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.

	  TIMER2_CTL_R |=  (0x01);
}

void Configure_Timer_3_A_INT(void)
{
	  //11.4 TIMERS (Timer3 A)
	  SYSCTL_RCGCTIMER_R |= 0x08; //Enable GP Timer Module 0 in Run Mode
	  //11.4.1 One/Shot Periodic Timer Mode
	  //1) Clear the TnEN bit in the GPTMCTL register
	  TIMER3_CTL_R &=  ~(0x01);
	  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
	  TIMER3_CFG_R  = 0x00000000;
	  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	      a. Write a value of 0x1 for One-Shot mode.
	      b. Write a value of 0x2 for Periodic mode
	  */
	  TIMER3_TAMR_R = 0x02;
	  //4 -----skip----
	  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
	  TIMER3_TAILR_R = 1600;
	  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER3_IMR_R |= (0x1<<0U); //Enable Timer A Time-Out Interrupt
	  NVIC_EN1_R |= (0x1 <<3U);  //Enable Nested Vector Interrupt Control for ISR
	  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.

	  TIMER3_CTL_R |=  (0x01);
}

void Configure_Timer_4_A_INT(void)
{
	  //11.4 TIMERS (Timer4 A)
	  SYSCTL_RCGCTIMER_R |= 0x10; //Enable GP Timer Module 0 in Run Mode
	  //11.4.1 One/Shot Periodic Timer Mode
	  //1) Clear the TnEN bit in the GPTMCTL register
	  TIMER4_CTL_R &=  ~(0x01);
	  //2) Write the GPTMCFG Configuration Register with a value of 0x0000.0000.
	  TIMER4_CFG_R  = 0x00000000;
	  /*3 Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	      a. Write a value of 0x1 for One-Shot mode.
	      b. Write a value of 0x2 for Periodic mode
	  */
	  TIMER4_TAMR_R = 0x02;
	  //4 -----skip----
	  //5 Load the start value into the GPTM Timer n Interval GPTMTnILR)
	  TIMER4_TAILR_R = 1600;
	  //6 For interrupts, set bits in the GPTM Interrupt Mask Register (GPTMIMR)
	  TIMER4_IMR_R |= (0x1<<0U); //Enable Timer A Time-Out Interrupt
	  NVIC_EN2_R |= (0x1 <<6U);  //Enable Nested Vector Interrupt Control for ISR
	  //7 Set the TnEN bit in GPTMCTL to enable the timer and start counting.

	  TIMER4_CTL_R |=  (0x01);
}

void Set_Timer3_Time_us(unsigned int time)
{
	 TIMER3_CTL_R &=  ~(0x01);
	 TIMER3_TAILR_R = 16*time;
	 TIMER3_CTL_R |=  (0x01);

}

void Clear_Timer0A_INT_FLAG(void)
{
	if((TIMER0_RIS_R & 0x01) == 0x01)
	{
		TIMER0_ICR_R |= 0x01; //clear Timer A Time Out.
	}
}

void Clear_Timer2A_INT_FLAG(void)
{
	if((TIMER2_RIS_R & 0x01) == 0x01)
	{
		TIMER2_ICR_R |= 0x01; //clear Timer A Time Out.
	}
}

void Clear_Timer3A_INT_FLAG(void)
{
	if((TIMER3_RIS_R & 0x01) == 0x01)
	{
		TIMER3_ICR_R |= 0x01; //clear Timer A Time Out.
	}
}

void Clear_Timer4A_INT_FLAG(void)
{
	if((TIMER4_RIS_R & 0x01) == 0x01)
	{
		TIMER4_ICR_R |= 0x01; //clear Timer A Time Out.
	}
}

void Clear_Pulse_Out_TIMER(void)
{
	Clear_Timer3A_INT_FLAG();
}

void Configure_SLC_INT_TIMER_100uS(void)
{
	Configure_Timer_0_A_INT();
}

void Clear_SLC_INT_TIMER(void)
{
	Clear_Timer0A_INT_FLAG();
}

void Clear_SLC_SENSE_INT_TIMER(void)
{
	Clear_Timer2A_INT_FLAG();
}

void TurnOff_IPULSE_Timer(void)
{
	TIMER3_CTL_R &=  ~(0x01);
}

void TurnOn_IPULSE_Timer(void)
{
	TIMER3_CTL_R &=  (0x01);
}

void Clear_SLC_MONITOR_INT_TIMER(void)
{
	Clear_Timer4A_INT_FLAG();
}

unsigned char spi0RdWt(unsigned char data)
{
  
   unsigned int ui32RcvDat;
    
   //Wait for Transmit buffer to empty
    while((SSI0_SR_R & 0x02) == 0){}
    SSI0_DR_R  = data;
    
   //wait for Receive buffer to fill up
    while((SSI0_SR_R & 0x04) == 0){}
    ui32RcvDat = SSI0_DR_R;
    
    return((unsigned char)ui32RcvDat);
    
}
unsigned char spiRdWt(unsigned char data)
{
  
   unsigned int ui32RcvDat;


   //Wait for Transmit buffer to empty
    while((SSI2_SR_R & 0x02) == 0){}
    SSI2_DR_R  = data;
    
   //wait for Receive buffer to fill up
    while((SSI2_SR_R & 0x04) == 0){}
    ui32RcvDat = SSI2_DR_R;
    
    return((unsigned char)ui32RcvDat);
    
}

void delayT(unsigned short ms)
{
/*  
  while(ms--)
  {
    while(!((TIMER0_RIS_R & 0x01) == 0x01)){}  
    TIMER0_ICR_R |= 0x01; //clear Timer A Time Out.
    
  }
  */
  portTickType ui32WakeTime = xTaskGetTickCount();
  vTaskDelayUntil(&ui32WakeTime, ms / portTICK_RATE_MS);
}

void Configure_EEPROM(void)
{
	uint32_t i;
     //pg 8.2.4.2;  539
   //EPROM Initialization and Configuration
   //Before writing to any EEPROM registers, the clock to the EEPROM module
   //must be enabled through the EEPROM Run Mode Clock Gating Control
   //(RCGCEEPROM) register (see page 356) and the following initialization

    SYSCTL_RCGCEEPROM_R  = 0x00000001;
   //steps must be executed:
   //1. Insert delay (6 cycles plus function call overhead).
    for ( i = 0; i < 0xFF; i++); //added a bit more delay since its startup;

   //2. Poll the WORKING bit in the EEPROM Done Status (EEDONE) register
   // until it is clear, indicating that the EEPROM has completed its power-on
   //initialization. When WORKING =0, continue.

    while(EEPROM_EEDONE_R & 0x01 == 0x01){}

   //3. Read the PRETRY and ERETRY bits in the EEPROM Support Control and
   //Status (EESUPP) register. If either of the bits are set, return an error,
    if ( ((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
      while(1); //DIE HERE
    }
    //else continue.

   //4. Reset the EEPROM module using the EEPROM Software Reset (SREEPROM)
   //register at offset 0x558 in the System Control register space.
    SYSCTL_SREEPROM_R = 0x01;
    for (i = 0; i < 0xFF; i++);
    SYSCTL_SREEPROM_R = 0x00;

   //5. Insert delay (6 cycles plus function call overhead).
    for (i = 0; i < 0xFF; i++);

   //6. Poll the WORKING bit in the EEPROM Done Status (EEDONE) register to
   //determine when it is clear. When WORKING =0, continue.
    while(EEPROM_EEDONE_R & 0x01 == 0x01){}

   //7. Read the PRETRY and ERETRY bits in the EESUPP register. If either
   // of the bits are set, return an error, else the EEPROM initialization
   //is complete and software may use the peripheral as normal.

    if ( ((EEPROM_EESUPP_R & 0x08) == 0x08) || ((EEPROM_EESUPP_R & 0x04) == 0x04))
    {
      while(1); //DIE HERE
    }

    //if not dead, then select block 0 for EEPROM write.
    EEPROM_EEBLOCK_R  = 0x01;
    //set current offset of block to 0;
    EEPROM_EEOFFSET_R = 0;

    while(EEPROM_EEDONE_R & 0x01 == 0x01){}
}

void WriteEEPROM(unsigned int data, unsigned int offset)
{
  EEPROM_EEOFFSET_R = offset;
  EEPROM_EERDWR_R = data;

  while(EEPROM_EEDONE_R & 0x01 == 0x01){}
}

unsigned int ReadEEPROM(unsigned int offset)
{
  EEPROM_EEOFFSET_R = offset;
  return(EEPROM_EERDWR_R);
}

void Configure_TC77_IO(void)
{
	//---------------------------------------------------------------------//
	//1) Enable Clock to the port by setting the appropriate bits in RCGCGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U);

	GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTD_CR_R |= 0x80;

	//2) Set the direction of the GPIO port pins by programming the GPIODIR
	GPIO_PORTD_DIR_R |= (1 <<5U);
	GPIO_PORTD_DIR_R |= (1 <<6U);
	GPIO_PORTD_DIR_R |= (1 <<7U);
	//3) Configure the GPIOAFSEL register to program each bit as GPIO;
	GPIO_PORTD_AFSEL_R &= ~(1 <<5U);
	GPIO_PORTD_AFSEL_R &= ~(1 <<6U);
	GPIO_PORTD_AFSEL_R &= ~(1 <<7U);

	//6) Enable GPIO pin as digital IO in GPIODEN
	GPIO_PORTD_DEN_R |= (1 <<5U);
	GPIO_PORTD_DEN_R |= (1 <<6U);
	GPIO_PORTD_DEN_R |= (1 <<7U);

	GPIO_PORTD_LOCK_R = 0;
}


unsigned char ReadRD4(void)
{
	if ( (GPIO_PORTD_DATA_R & (1 <<4U)) == (1 <<4U))
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

unsigned char PinSLCCntrl(void)
{
	if(ReadRD4() == 1)
		return(0);
	else
	{
		return(1);
	}
}

void Configure_BOARDA2D0_AIN0(void)
{
	//13.4.1 ADC0 (Module 1) Initilization and Configuration
	  //---------------------------------------------------------------------//
	  //1) Enable the ADC clock using the RCGADC register
	  SYSCTL_RCGCADC_R |= (1 << 0U); //Turn on Module AD0

	  //2) Enable the clock to the GPIO module via RCGGPIO
	  SYSCTL_RCGCGPIO_R |= (1 << 4U); //enable GPIO E  (AIN0)

	  //3) Set GPIO AFSEL bits for the ADC Input Pins
	  GPIO_PORTE_AFSEL_R |= (1 << 3U);   //Pin PE3;

	  //4)Configure the AINx signal to be analog inputs; clear GPIODEN bit
	  GPIO_PORTE_DEN_R &= ~(1 << 3U);    //Pin PE3;

	  //5) Disable the analog isloation circuit; Set bits in GPIOAMSEL
	  GPIO_PORTE_AMSEL_R  |= (1 << 3U);   //Pin PE3;

	  //6) If desired reconfigrure sample priority in ADCSSPRI
	  //by default; SS0 - highest prioirty; SS3 lowest priority;
}

void Configure_BOARDA2D1(void)
{
	//13.4.1 ADC1 (Module 1) Initilization and Configuration
	  //---------------------------------------------------------------------//
	  //1) Enable the ADC clock using the RCGADC register
	  SYSCTL_RCGCADC_R |= (1 << 1U); //Turn on Module AD1

	  //2) Enable the clock to the GPIO module via RCGGPIO
	  SYSCTL_RCGCGPIO_R |= (1 << 3U); //enable GPIO D  (AIN4)
	  SYSCTL_RCGCGPIO_R |= (1 << 4U); //enable GPIO E  (AIN0:3, AIN8:9)

	  //3) Set GPIO AFSEL bits for the ADC Input Pins
	  GPIO_PORTD_AFSEL_R |= (1 << 3U);      //Pin PD3;
	  GPIO_PORTE_AFSEL_R |= (0x3F << 0U);   //Pin PE0 - PE5;

	  //4)Configure the AINx signal to be analog inputs; clear GPIODEN bit
	  GPIO_PORTD_DEN_R &= ~(1 << 3U);       //Pin PD3;
	  GPIO_PORTE_DEN_R &= ~(0x3F << 0U);    //Pin PE0 - PE5;

	  //5) Disable the analog isloation circuit; Set bits in GPIOAMSEL
	  GPIO_PORTD_AMSEL_R  |= (1 << 3U);      //Pin PD3;
	  GPIO_PORTE_AMSEL_R  |= (0x3F << 0U);   //Pin PE0 - PE5;

	  //6) If desired reconfigrure sample priority in ADCSSPRI
	  //by default; SS0 - highest prioirty; SS3 lowest priority;
}

void Select_A2DEarthGnd(void)
{
	  //13.4.2 ADC1 (AIN1) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x1 <<0U); // AN1


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN1 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DIBAT1(void)
{
	  //13.4.2 ADC1 (AIN2) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x2 <<0U); // AN2


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN2 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DVBAT1(void)
{
	  //13.4.2 ADC1 (AIN3) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x3 <<0U); // AN3


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN3 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DMODE(void)
{
	  //13.4.2 ADC1 (AIN4) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x4 <<0U); // AN4


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN4 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DIBAT2(void)
{
	  //13.4.2 ADC1 (AIN8) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x8 <<0U); // AN8


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN8 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}


void Select_A2DVBAT2(void)
{
	  //13.4.2 ADC1 (AIN9) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all;
	  ADC1_SSMUX3_R |= (0x9 <<0U); // AN9


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0x6 <<0U); // AN9 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DTM4CTemp(void)
{
	  //13.4.2 ADC1 (Internal Temperature of TM4C) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC1_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC1_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC1_SSMUX3_R  = 0; //clear all; //this value is ignored when we do temp.


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC1_SSCTL3_R = (0xE <<0U); // TEMP Sensor //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC1_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;

}

void Select_A2DISLC(void)
{
	  //13.4.2 ADC0 (AIN0) Sample Sequence Configuration;
	  //---------------------------------------------------------------------//
	  //1) Disable corresponding ASENn bit in ADCACTSS
	  ADC0_ACTSS_R &= ~(0x0F << 0U);  // Disable ASEN0 - ASEN3

	  //2)configure the trigger even for the sample sequenceer in the ADCEMUX
	  ADC0_EMUX_R &= ~(0xFFFF <<0U); //Select Processor Control;

	  //3)if PWM is chosen, select which PWM module will be used
	  //not used, so ignored;

	  //4) Configure the corresponding input source in the ADCSSMUXn register
	  ADC0_SSMUX3_R  = 0; //clear all;
	  ADC0_SSMUX3_R |= (0x0 <<0U); // AN0


	  //5) Configure the ADCSSCTL0 register for the sequence order above;
	  ADC0_SSCTL3_R = (0x6 <<0U); // AN0 //ASSERT END bit & Interrupt;

	  //6) If interrupt is to be used, set corresponding MASK bit in ADCIM;
	  //not using interrupt;

	  //7) Enable the sample sequence logic by setting ASEN in ADCACTSS;
	  ADC0_ACTSS_R |= (0x1 << 3U);  // Enable sequence 3 ASEN0;
}

unsigned int Run_A2D1(void)
{
	  uint32_t result;

	  //Enable 64 x AVG sampling;
	  ADC1_SAC_R = 0x06;

	  //clear ISC bit;
	  ADC1_ISC_R |= (0x1 << 3U);

	  ADC1_PSSI_R |= (0x1 <<3U);          // 1) initiate SS3
	  while((ADC1_RIS_R&0x08)==0){};      // 2) wait for conversion done
	  result = ADC1_SSFIFO3_R;//&0xFFF;   // 3) read result
	  ADC1_ISC_R = 0x0008;                // 4) acknowledge completion

	  return(result);

}

void Start_A2D0_Sample(void)
{
	  //Enable 64 x AVG sampling;
	  ADC0_SAC_R = 0x06;

	  //clear ISC bit;
	  ADC0_ISC_R |= (0x1 << 3U);

	  ADC0_PSSI_R |= (0x1 <<3U);          // 1) initiate SS3
}

unsigned int Get_A2D0_Sample(void)
{
	  unsigned int result;
	  while((ADC0_RIS_R&0x08)==0){};      // 2) wait for conversion done
	  result = ADC0_SSFIFO3_R & 0xFFF;    // 3) read result from seq3 fifo
	  ADC0_ISC_R = 0x0008;                // 4) acknowledge seq3 completion

	  return(result);

}

void UnitSWTimer(void)
{
	if((TIMER1_RIS_R & 0x01) == 0x01)
	{
		TIMER1_ICR_R |= 0x01; //clear Timer A Time Out.
	}
	Tick100ms++;
}

unsigned int GetUnitTick_100ms(void)
{
	return(Tick100ms);
}



