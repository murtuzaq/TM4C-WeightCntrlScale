/*
 * tm4c_adc0.c
 *
 *  Created on: Apr 5, 2017
 *      Author: mquaizar
 */

#include <stdint.h>

#define SYSCTL_RCGCGPIO_R		(*((volatile uint32_t *)0x400FE608))
#define SYSCTL_RCGCADC_R        (*((volatile uint32_t *)0x400FE638))

#define GPIO_PORTD_DEN_R		(*((volatile uint32_t *)0x4000751C))
#define GPIO_PORTD_AFSEL_R      (*((volatile uint32_t *)0x40007420))
#define GPIO_PORTD_AMSEL_R      (*((volatile uint32_t *)0x40007528))


#define ADC1_ACTSS_R            (*((volatile uint32_t *)0x40039000))
#define ADC1_RIS_R              (*((volatile uint32_t *)0x40039004))
#define ADC1_ISC_R              (*((volatile uint32_t *)0x4003900C))
#define ADC1_SSMUX3_R           (*((volatile uint32_t *)0x400390A0))
#define ADC1_SSFIFO3_R          (*((volatile uint32_t *)0x400390A8))
#define ADC1_ACTSS_R            (*((volatile uint32_t *)0x40039000))
#define ADC1_EMUX_R             (*((volatile uint32_t *)0x40039014))
#define ADC1_PSSI_R             (*((volatile uint32_t *)0x40039028))
#define ADC1_SAC_R              (*((volatile uint32_t *)0x40039030))
#define ADC1_SSCTL3_R           (*((volatile uint32_t *)0x400390A4))



void ADC1_Init(void)
{
	//13.4.1 ADC1 (Module 1) Initilization and Configuration
	//---------------------------------------------------------------------//
	//1) Enable the ADC clock using the RCGADC register
	SYSCTL_RCGCADC_R |= (1 << 1U); //Turn on Module AD1

	//2) Enable the clock to the GPIO module via RCGGPIO
	SYSCTL_RCGCGPIO_R |= (1 << 3U); //enable GPIO D  (AIN4)

	//3) Set GPIO AFSEL bits for the ADC Input Pins
	GPIO_PORTD_AFSEL_R |= (1 << 3U);      //Pin PD3;

	//4)Configure the AINx signal to be analog inputs; clear GPIODEN bit
	GPIO_PORTD_DEN_R &= ~(1 << 3U);       //Pin PD3;

	//5) Disable the analog isloation circuit; Set bits in GPIOAMSEL
	GPIO_PORTD_AMSEL_R  |= (1 << 3U);      //Pin PD3;

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

uint32_t ADC1_Run(void)
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


