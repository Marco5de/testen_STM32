#include "main.h"

//Distanz prop. zur Zeit die echo Pin High ist

//busy waiting
void delay(int ms){
	while(ms-- > 0){
		volatile int x = 5971;
		while(x-- > 0){
			__asm("nop");
		}
	}
}


int buffered_write(char *buf,int len){
	for(int i=0; i<len; i++){
		USART_SendData(USART1,buf[i]);
		//polling until transmit buffer/reg empty
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	return 0;
}

int measure(){
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	volatile int x = 57;
	while(x-- > 0){
		__asm("nop");
	}
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	while(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1));
	int tix = 0;	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)){
		tix++;
	}
	
	return tix;
}

int itoa(int d, char *out){
	char *ptr = out;
	do{
		*ptr++ = '0' + d%10;
		d/=10;
	}while(d);

	while(out<ptr--){
		char tmp = *out;
		*out++ = *ptr;
		*ptr=tmp;

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

	//Init HC-SR04 Pins
	//Trigger Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//Echo Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//1cm =^= 168 tix
	while(1){
		delay(5000);
		char buf[10]={0};
		int dist = measure()/168;
		itoa(dist,buf);	
		buffered_write(buf,10);		
	}
}

