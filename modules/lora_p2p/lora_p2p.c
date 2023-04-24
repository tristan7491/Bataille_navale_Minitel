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
 
#include "lora_p2p.h"

#define SX127X_LORA_MSG_QUEUE   (16U)
#define SX127X_STACKSIZE        (THREAD_STACKSIZE_DEFAULT)

static char stack[SX127X_STACKSIZE];
static kernel_pid_t _recv_pid;

#define MSG_TYPE_ISR            (0x3456)

static sx127x_t sx127x;

uint8_t flag_rx_lora; //lora rx message avilable

char message[32]; //content of lora rx message

static void _event_cb(netdev_t *dev, netdev_event_t event)
{
    if (event == NETDEV_EVENT_ISR) {
        msg_t msg;

        msg.type = MSG_TYPE_ISR;
        msg.content.ptr = dev;

        if (msg_send(&msg, _recv_pid) <= 0) {
            puts("gnrc_netdev: possibly lost interrupt.");
        }
    }
    else {
        size_t len;
        netdev_lora_rx_info_t packet_info;
        switch (event) {
        case NETDEV_EVENT_RX_STARTED:
            LED_RED2_ON;
            //puts("Data reception started");
            break;

        case NETDEV_EVENT_RX_COMPLETE:
            len = dev->driver->recv(dev, NULL, 0, 0);
            dev->driver->recv(dev, message, len, &packet_info);
            
            /*printf(
                "{Payload: \"%s\" (%d bytes), RSSI: %i, SNR: %i, TOA: %" PRIu32 "}\n",
                message, (int)len,
                packet_info.rssi, (int)packet_info.snr,
                sx127x_get_time_on_air((const sx127x_t *)dev, len));*/
            
            flag_rx_lora = 1;
            /*printf("Recieved in event_cb: %s, flag=%d\n",message,flag_rx_lora);*/
            LED_RED2_OFF;
            break;

        case NETDEV_EVENT_TX_COMPLETE:
            sx127x_set_sleep(&sx127x);
            LED_GREEN1_OFF;
            //puts("Transmission completed");
            listen(dev);//put in RX mode once message is sent
            break;

        case NETDEV_EVENT_CAD_DONE:
            break;

        case NETDEV_EVENT_TX_TIMEOUT:
            sx127x_set_sleep(&sx127x);
            break;

        default:
            /* printf("Unexpected netdev event received: %d\n", event); */
            break;
        }
    }
}

void *_recv_thread(void *arg)
{
    (void)arg;

    static msg_t _msg_q[SX127X_LORA_MSG_QUEUE];

    msg_init_queue(_msg_q, SX127X_LORA_MSG_QUEUE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        if (msg.type == MSG_TYPE_ISR) {
            netdev_t *dev = msg.content.ptr;
            dev->driver->isr(dev);
        }
        else {
            /* puts("Unexpected msg type"); */
        }
    }
}

netdev_t* setup_lora(int bandwith, uint8_t spreading_factor, int code_rate, uint32_t chan)
{
	flag_rx_lora = 0;
	sx127x.params = sx127x_params[0];
    netdev_t *netdev = &sx127x.netdev;

    netdev->driver = &sx127x_driver;

    netdev->event_callback = _event_cb;
    
    if (netdev->driver->init(netdev) < 0) {
        puts("Failed to initialize SX127x device, exiting");
        return NULL;
    }

    _recv_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                              THREAD_CREATE_STACKTEST, _recv_thread, NULL,
                              "recv_thread");

    if (_recv_pid <= KERNEL_PID_UNDEF) {
        puts("Creation of receiver thread failed");
        return NULL;
    }
    
    /* Check bandwidth value */
    uint8_t lora_bw;

    switch (bandwith) {
    case 125:
        //puts("setup: setting 125KHz bandwidth");
        lora_bw = LORA_BW_125_KHZ;
        break;

    case 250:
        //puts("setup: setting 250KHz bandwidth");
        lora_bw = LORA_BW_250_KHZ;
        break;

    case 500:
        //puts("setup: setting 500KHz bandwidth");
        lora_bw = LORA_BW_500_KHZ;
        break;

    default:
        puts("[Error] setup: invalid bandwidth value given, "
             "only 125, 250 or 500 allowed.");
        return NULL;
    }

    /* Check spreading factor value */
    uint8_t lora_sf = spreading_factor;

    if (lora_sf < 7 || lora_sf > 12) {
        puts("[Error] setup: invalid spreading factor value given");
        return NULL;
    }

    /* Check coding rate value */
    int cr = code_rate;

    if (cr < 5 || cr > 8) {
        puts("[Error] setup: invalid coding rate value given");
        return NULL;
    }
    uint8_t lora_cr = (uint8_t)(cr - 4);

    /* Configure radio device */
    netdev->driver->set(netdev, NETOPT_BANDWIDTH,
                        &lora_bw, sizeof(lora_bw));
    netdev->driver->set(netdev, NETOPT_SPREADING_FACTOR,
                        &lora_sf, sizeof(lora_sf));
    netdev->driver->set(netdev, NETOPT_CODING_RATE,
                        &lora_cr, sizeof(lora_cr));
    netdev->driver->set(netdev, NETOPT_CHANNEL_FREQUENCY,
						&chan, sizeof(chan));
	
    printf("[Info] setup: %dKHz bandwidth, %d spreading factor, %d coding rate, %lu channel frequency. Set with success!\n",bandwith,spreading_factor,code_rate,chan);
    
    /* Always be in listen mode except when transmitting */
    listen(netdev);

    return netdev;
}

void send_lora(netdev_t *netdev, char* playload)
{
    /* printf("Sending: \"%s\"\n", playload); */

    iolist_t iolist = {
        .iol_base = playload,
        .iol_len = (strlen(playload) + 1)
    };

    if (netdev->driver->send(netdev, &iolist) == -ENOTSUP) {
        puts("Cannot send: radio is still transmitting");
    }
}

void listen(netdev_t *netdev)
{
    /* Switch to continuous listen mode */
    const netopt_enable_t single = false;

    netdev->driver->set(netdev, NETOPT_SINGLE_RECEIVE, &single, sizeof(single));
    const uint32_t timeout = 0;

    netdev->driver->set(netdev, NETOPT_RX_TIMEOUT, &timeout, sizeof(timeout));

    /* Switch to RX state */
    netopt_state_t state = NETOPT_STATE_RX;

    netdev->driver->set(netdev, NETOPT_STATE, &state, sizeof(state));

    //printf("Listen mode set\n");
}
