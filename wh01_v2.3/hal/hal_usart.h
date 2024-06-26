#ifndef _HAL_USART_H
#define _HAL_USART_H


#define DEBUG_TX_PORT	GPIOA
#define DEBUF_TX_PIN	GPIO_Pin_9

#define DEBUG_RX_PORT	GPIOA
#define DEBUF_RX_PIN	GPIO_Pin_10

#define DEBUG_USART_PORT	USART1

#define WIFI_TX_PORT	GPIOA
#define WIFI_TX_PIN	GPIO_Pin_2

#define WIFI_RX_PORT	GPIOA
#define WIFI_RX_PIN	GPIO_Pin_3

#define WIFI_USART_PORT	USART2

#define DEBUG_TXBUFF_SIZE_MAX 400

void hal_UsartInit(void);
void hal_UsartProc(void);

#endif
