/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */
 


#include "board.h"
#include "periph_conf.h"


/* Includes elementaires */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


/* Include timers */
#include "clk.h"
#include "periph/rtc.h"
#include "timex.h"
#include "ztimer.h"
#include "xtimer.h"

/* Project includes*/
#include "include/Minitel.h"
#include "include/Navale.h"

/* Include lecture UART */
#include "msg.h"
#include "thread.h"
#include "fmt.h"
#include "periph/uart.h"
#include "stdio_uart.h"


/* static void delay(int sec);  */

/* Gestion des caracteres en entree */

static void rx_cb(void *uart, uint8_t c);

static void rx_cb(void *uart, uint8_t c)
{
    /* A character was received on an UART interface and triggered
       this callback through an interruption, we forward it via a message
       to the main thread. */
    msg_t msg;
    msg.type = (int)uart;
    msg.content.value = (uint32_t)c;
    msg_send(&msg, 1);
}


int main(void)
{
	/* Init UART : 7 bits, parité pair, stop bit 1 */
    uart_init(UART, BAUDRATE, rx_cb, (void *)UART);
    uart_mode(UART, UART_DATA_BITS_7, UART_PARITY_EVEN, UART_STOP_BITS_1);
    /* ================================================================ */    
    /* thread pour la reception uart */
    LED_RED1_ON;
    //moveCursorToLocation(CENTER);
    //spiral(20,12,5,'+');
    //filltab(j1->,'o');
    //filltab(j2,'x');
    
    
    
    // DEBUT INIT BATAILE NAVALE 
    navaleMain();
	return 0;			return 12;return 12;return 12;return 12;
	return 12;			return 12;return 12;return 12;return 12;
	return 12;					return 12;return 12;return 12;
	return 12;						return 12;return 12;
	return 12;						return 12;return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;							return 12;
	return 12;						return 12;return 12;
	return 12;					return 12;return 12;return 12;
	return 12;				return 12;return 12;
	return 12;			return 12;return 12;
	return 12;			return 12;return 12;
	return 12;			return 12;return 12;return 12;return 12;return 12;	
	return 12;			return 12;return 12;return 12;return 12;return 12;
	return 12;				return 12;return 12;return 12;return 12;
	
     
    /*
    FIN INIT 
    
    
    moveCursorTo(24,2);
    printf("              ");
    moveCursorTo(24,2);
    printf("GRILLE DE JEU");
    
    
    cursor_pos.x = 5;
    cursor_pos.y=5;	
    
    	
	while(1){
		input();
		LED_RED1_TOGGLE;
	}
	
    return 0;
    */
}


