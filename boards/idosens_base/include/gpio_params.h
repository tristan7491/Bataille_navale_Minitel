/*
 * Copyright (C) 2019 Laboratoire informatique de Grenoble
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     boards_idosens_base
 * @{
 *
 * @file
 * @brief       Board specific configuration of direct mapped GPIOs
 *
 * @author      Clément Monaton, Germain Lemasson <germain.lemasson@univ-grenoble-alpes.fr>
 */

#ifndef GPIO_PARAMS_H
#define GPIO_PARAMS_H

#include "board.h"
#include "saul/periph.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief    GPIO pin configuration
 */
static const  saul_gpio_params_t saul_gpio_params[] =
{
    {
        .name = "LED(red1)",
        .pin = LED_RED1_PIN,
        .mode = GPIO_OUT
    },
	{
        .name = "LED(red2)",
        .pin = LED_RED2_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LED(green1)",
        .pin = LED_GREEN1_PIN,
        .mode = GPIO_OUT
    },
	{
        .name = "LED(green2)",
        .pin = LED_GREEN2_PIN,
        .mode = GPIO_OUT
    },
    {
        .name = "LED(blue1)",
        .pin = LED_BLUE1_PIN,
        .mode = GPIO_OUT
    },
	{
        .name = "LED(blue2)",
        .pin = LED_BLUE2_PIN,
        .mode = GPIO_OUT
    }
	
};

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PARAMS_H */
/** @} */

