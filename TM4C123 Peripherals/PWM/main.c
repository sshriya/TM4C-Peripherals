/*########################### PWM ##################################*/
/*------------------------------TM4C123-----------------------------------*/
//Fade red REd on board - PF1

#include "driverlib/pin_map.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

#define USER_LED GPIO_PIN_1

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

int main(void)
{
    uint32_t period = 5000; //20ms (16Mhz / 64pwm_divider / 50)
    uint32_t duty = 250;    //1.5ms pulse width : 5%

    int i;

    // %%%%%%%%%%% 1. Clock Setup %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Set the clock 16 MHZ
   SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

   //Configure PWM Clock divide system clock by 64 - 0.25 MHz or 250 KHz
   SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

   // %%%%%%%%%%%%% 2. Enable Peripherals %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

   // GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // PWM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); // Module 1 for PF1

    // %%%%%%%%%%%% 3. PWM Congif %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //Configure PF1 and PD0 as PWM
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_1);


    //Configure PWM Options
    //PWM_GEN_2 Covers M1PWM5
    //PWM_GEN_3 Covers M1PWM6
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    //Set the Period (expressed in clock ticks)
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period);

    //Set PWM duty
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,duty);

    // Enable the PWM generator
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);

    // Turn on the Output pins
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);

    while(1)
    {
    	for (i = 0; i<period; i++){
    		duty = i;
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,duty);
    		delayMS(2);
    	}

    	i = 0;
    }
}
