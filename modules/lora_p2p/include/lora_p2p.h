/*
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory of RIOT for more details.
 */

/**
 * @ingroup     modules
 * @{
 * @file
 * @brief       minimalist module for point-to-point lora using SX127X modem driver
 *
 * @author		Tristan Lailler <tristan.lailler@etu.univ-grenoble-alpes.fr>
 * @}
 */
#ifndef LORA_P2P_H
#define LORA_P2P_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "net/netdev.h"
#include "net/netdev/lora.h"
#include "net/lora.h"

#include "sx127x_internal.h"
#include "sx127x_params.h"
#include "sx127x_netdev.h"

#include "fmt.h"

#include "board.h"


 /** @brief Sets the SX127X in RX mode
 * 
 *  @param netdev The sx127x device structure pointer
 *  @param bandwith is either 125, 250 or 500
 *  @param spreading_factor must be between 7 and 12 included
 *  @param code_rate must be between 5 and 8 included
 *  @param chan is the frequency you want to use in Hz
 * 
 *  @return void
 */
netdev_t* setup_lora(int bandwith, uint8_t spreading_factor, int code_rate, uint32_t chan);

/** @brief Sets the SX127X in RX mode
 * 
 *  @param netdev The sx127x device structure pointer
 *  @param playload The string to send
 *  @return void
 */
void send_lora(netdev_t *netdev, char* playload);

/** @brief Sets the SX127X in RX mode
 * 
 *  @param netdev The sx127x device structure pointer
 *  @return void
 */
void listen(netdev_t *netdev);

#endif
