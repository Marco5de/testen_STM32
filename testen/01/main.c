#include "main.h"

void delay(int millis) {
    while (millis-- > 0) {
        volatile int x = 5971;
        while (x-- > 0) {
            __asm("nop");
        }
    }
}

int main(void) {

    // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_InitStructure;

    // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
	

    // configure port A1 for driving an LED
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // output push-pull mode
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   // highest speed
    GPIO_Init(GPIOA, &GPIO_InitStructure) ;             // initialize port
	
	//init input pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

    // main loop
    while(1) {
		u8 readval;
		readval = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
		
		if(readval){
        	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);    // turn the LED on
			//experiment with logic ananlyzer
			//delay(20);
		}
		else{
			GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
			//delay(20);
		}
    }
}
