/* %%%%%%%%%%%%%%%%%%%%%%%  Timer and Interrupt  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * ------------Blink Blue LEd on LaunchPAd - TM4C123
 * The rate of binking is defined in periods variable and it uses timerA timout
 * interrupt
 * */
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"

#define USER_LED1  GPIO_PIN_1
#define USER_LED2  GPIO_PIN_2
#define USER_LED3  GPIO_PIN_3

int main(void)
{
	uint32_t periods, periods2; // Timer delays

    // Setup the system clock to run at 40 Mhz from PLL with crystal reference
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //Configure GPIO
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, USER_LED1|USER_LED2|USER_LED3);

    //Configure Timer 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

    //Configure Timer 1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE, TIMER_CFG_A_PERIODIC);

    // Define period for Timer 0
    periods = (SysCtlClockGet()/10)/2; // 10 HZ period
    TimerLoadSet(TIMER0_BASE, TIMER_A, periods-1);

    // Define Period for Timer 1
    periods2 = (SysCtlClockGet()/10)/2; // 10 HZ period
    TimerLoadSet(TIMER1_BASE, TIMER_A, periods2-1);

    //Enable iNTERRUPTS
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    IntEnable(INT_TIMER1A);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    IntMasterEnable();

    //Enable Timer
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER1_BASE, TIMER_A);

    while(1)
    {
    }
}

//ISR
void Timer0IntHandler(void){
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Read current state and write back the opposite state
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1)){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, ~GPIO_PIN_1);
	}else{
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
	}
}

void Timer1IntHandler(void){
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	//Read current state and write back the opposite state
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2)){
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, ~GPIO_PIN_2);
	}else{
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_2, GPIO_PIN_2);
	}
}
