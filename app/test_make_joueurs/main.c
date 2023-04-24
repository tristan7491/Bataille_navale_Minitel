/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory of RIOT for more details.
 */

/**
 * @ingroup     app
 * @{
 * @file
 * @brief       Hello World application for point-to-point lora using lora_p2p module
 *
 * @author		Tristan Lailler <tristan.lailler@etu.univ-grenoble-alpes.fr>
 * @}
 */

#include <stdio.h>

#include "thread.h"

#include "board.h"

#include "xtimer.h"

#include "lora_p2p.h"

/* Include lecture UART */
#include "msg.h"
#include "thread.h"
#include "fmt.h"
#include "periph/uart.h"
#include "stdio_uart.h"

#undef STDIO_UART_DEV
#define STDIO_UART_DEV      	UART_DEV(1)

#define UART  UART_DEV(1)
#define BAUDRATE (1200U)

char buffer[10];

extern volatile uint8_t flag_rx_lora; //lora rx message avilable

extern char message[32]; //content of lora rx message

static void uart_cb(void *uart, uint8_t c);

void read_lora(char* msg);

int main(void)
{
	/* Init UART : 7 bits, parité pair, stop bit 1 */
    uart_init(UART, BAUDRATE, uart_cb, (void *)UART);
    uart_mode(UART, UART_DATA_BITS_7, UART_PARITY_EVEN, UART_STOP_BITS_1);
    /* ================================================================ */
    printf("begin main\n");
    struct netdev* dev;
    
    dev = setup_lora(125,12,5,868000000);
	if(dev==NULL){
		return 0;
	}
	printf("joueur=%d",JOUEUR_BATAILLE);
	while(1){
		LED_GREEN1_ON;
		send_lora(dev, "Hello World!");
		read_lora(buffer);
		printf("Received in main: \"%s\"\n",buffer);
		xtimer_sleep(5);
	}
	
    return 0;
}

/* thread pour la lecture d'un caractere sur la liaison uart */
static void uart_cb(void *uart, uint8_t c)
{
    /* A character was received on an UART interface and triggered
       this callback through an interruption, we forward it via a message
       to the main thread. */
    msg_t msg;
    msg.type = (int)uart;
    msg.content.value = (uint32_t)c;
    msg_send(&msg, 1);
}

void read_lora(char* msg){
	while(flag_rx_lora!=1){
		if(flag_rx_lora==1){
			printf("flag dans read = %d\n",flag_rx_lora);
		}
	}
	sprintf(msg,"%s",message);
	flag_rx_lora=0;
}
