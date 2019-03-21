#include "main.h"

//busy waiting
void delay(int ms){
	while(ms-- > 0){
		volatile int x = 5971;
		while(x-- > 0){
			__asm("nop");
		}
	}
}
//polling
int putchar(int c){
	while ( USART_GetFlagStatus (USART1 , USART_FLAG_TXE ) == RESET);
	USART1 ->DR = (c & 0xff);
	return 0;
}

//polling until data is received
//should be done using interupts
int getchar (void){
	while ( USART_GetFlagStatus (USART1 , USART_FLAG_RXNE ) == RESET);
	return USART1 ->DR & 0xff;
}

int buffered_write(char *buf,int len){
	for(int i=0; i<len; i++){
		USART_SendData(USART1,buf[i]);
		//polling until transmit buffer/reg empty
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	return 0;
}

int main(void) {

    // GPIO structure for port initialization
    GPIO_InitTypeDef GPIO_InitStructure;
	
    // enable clock on APB2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,  ENABLE);
	
	GPIO_StructInit(&GPIO_InitStructure);
	
	//Init GPIO for USART_1Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Init GPIO forUSART1_Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//Init for USART1
	USART_InitTypeDef USART_InitStruct;
	USART_StructInit(&USART_InitStruct);

	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);

	while(1){
		delay(100);
		//USART_SendData(USART1,'H');		
		buffered_write("Hello World",11);
	}
}

