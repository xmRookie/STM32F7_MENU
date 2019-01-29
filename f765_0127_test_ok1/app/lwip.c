/**
 ******************************************************************************
  * File Name          : LWIP.c
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#if defined ( __CC_ARM )  /* MDK ARM Compiler */
#include "lwip/sio.h"
#endif /* MDK ARM Compiler */
#include "common.h"
#include "ethernetif.h"
/* Private function prototypes -----------------------------------------------*/
/* ETH Variables initialization ----------------------------------------------*/
void _Error_Handler(char * file, int line);


/* Variables Initialization */
struct netif gnetif;
ip4_addr_t ipaddr;
ip4_addr_t netmask;
ip4_addr_t gw;
ip4_addr_t remoteipaddr;
//uint8_t IP_ADDRESS[4];
//uint8_t NETMASK_ADDRESS[4];
//uint8_t GATEWAY_ADDRESS[4];


NETIF_PAR netif_par;
NETIF_REMOTE_PAR netif_remote_par;

void Lwip_Par_conf(NETIF_PAR *mypar)
{
    u8_t i;
    memcpy(netif_par.ip, mypar->ip, 4);
    memcpy(netif_par.netmask, mypar->netmask, 4);
    memcpy(netif_par.gateway, mypar->gateway, 4);
    memcpy(netif_par.mac, mypar->mac, 6);
    netif_par.port = mypar->port;

    /* IP addresses initialization without DHCP (IPv4) */
    IP4_ADDR(&ipaddr, netif_par.ip[0], netif_par.ip[1], netif_par.ip[2], netif_par.ip[3]);
    IP4_ADDR(&netmask, netif_par.netmask[0], netif_par.netmask[1], netif_par.netmask[2], netif_par.netmask[3]);
    IP4_ADDR(&gw, netif_par.gateway[0], netif_par.gateway[1], netif_par.gateway[2], netif_par.gateway[3]);
    ethernetif_mac_conf(netif_par.mac);

    for(i=0;i<4;i++)
    {
    	printf("ip 0x%x\r\n",netif_par.ip[i]);
    	printf("mask 0x%x\r\n",netif_par.netmask[i]);
    	printf("gate 0x%x\r\n",netif_par.gateway[i]);
    }
}

void User_notification(struct netif *netif) 
{
uint8_t iptxt[20];

  if (netif_is_up(netif))
  {
  
    sprintf((char *)iptxt, "%s", ip4addr_ntoa((const ip4_addr_t *)&netif->ip_addr));
  }
  else
  {
	printf("The network cable is not connected \n");
  }
}


ip4_addr_t get_Lwip_Ip(void)
{
    return ipaddr;
}


uint16_t get_Lwip_port(void)
{
    return netif_par.port;
}

/*
void remoteLwip_mac_conf(const ip4_addr_t *ipaddr, struct eth_addr *ethaddr)
{
	if(ipaddr && ip4_addr_cmp(ipaddr, &remoteipaddr) &&netif_remote_par.link == 0)
	{
		netif_remote_par.link = 1;
		memcpy(netif_remote_par.mac, ethaddr, ETH_HWADDR_LEN);
	}
}


void remoteLwip_Par_conf(NETIF_REMOTE_PAR *remotepar)
{
    memcpy(netif_remote_par.ip, remotepar->ip, 4);
    netif_remote_par.port = remotepar->port;
    IP4_ADDR(&remoteipaddr, netif_remote_par.ip[0], netif_remote_par.ip[1], netif_remote_par.ip[2],
             netif_remote_par.ip[3]);
}
*/
void remoteLwip_mac_conf(const ip4_addr_t *ipaddr, struct eth_addr *ethaddr)
{
	if(ipaddr && ip4_addr_cmp(ipaddr, &remoteipaddr) && get_remoteLwip_udp_link() == NET_UNLINK)
	{
		//netif_remote_par.link = 1;
		set_remoteLwip_udp_link(NET_LINKED);
		memcpy(netif_remote_par.mac, ethaddr, ETH_HWADDR_LEN);
		ap_udp_set_mac(netif_remote_par.mac);
	}
}

void remoteLwip_Par_conf(NETIF_REMOTE_PAR *remotepar)
{
    memcpy(netif_remote_par.ip, remotepar->ip, 4);
    netif_remote_par.port = remotepar->port;
    IP4_ADDR(&remoteipaddr, netif_remote_par.ip[0], netif_remote_par.ip[1], netif_remote_par.ip[2],
             netif_remote_par.ip[3]);
	netif_remote_par.link = NET_UNLINK;
}

ip4_addr_t get_remoteLwip_Ip(void)
{
    return remoteipaddr;
}


uint16_t get_remoteLwip_port(void)
{
    return netif_remote_par.port;
}

void set_remoteLwip_udp_link(uint8_t link)
{
	if(link)
		netif_remote_par.link = NET_LINKED;
	else
		netif_remote_par.link = NET_UNLINK;
}


uint8_t get_remoteLwip_udp_link(void)
{
	return netif_remote_par.link;
}
/**
  * @brief  This function notify user about link status changement.
  * @param  netif: the network interface
  * @retval None
  */
void ethernetif_notify_conn_changed(struct netif *netif)
{
  if(netif_is_link_up(netif))
  {
    netif_set_addr(netif, &ipaddr, &netmask, &gw);   

    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);     
  }
  else
  {
		set_remoteLwip_udp_link(NET_UNLINK);      //£¡£¡£¡
    /*  When the netif link is down this function must be called.*/
    netif_set_down(netif);
   
  }
}

extern void pollint_ethernetif_link_status(void)
{

#ifdef	LWIP_NETIF_LINK_CALLBACK
	ethernetif_set_link(&gnetif);
#endif
}


/**
  * LwIP initialization function
  */
void MX_LWIP_Init(void)
{
    /* Initilialize the LwIP stack without RTOS */
    lwip_init();

    /* add the network interface (IPv4/IPv6) without RTOS */
    netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);

    /* Registers the default network interface */
    netif_set_default(&gnetif);

    if(netif_is_link_up(&gnetif)) {
        /* When the netif is fully configured this function must be called */
        netif_set_up(&gnetif);
    } else {
        /* When the netif link is down this function must be called */
        netif_set_down(&gnetif);
    }
#if LWIP_NETIF_LINK_CALLBACK	  //£¡£¡£¡
  /* Set the link callback function, this function is called on change of link status */
  netif_set_link_callback(&gnetif, ethernetif_update_config);
#endif

}

#ifdef USE_OBSOLETE_USER_CODE_SECTION_4
/* Kept to help code migration. (See new 4_1, 4_2... sections) */
/* Avoid to use this user section which will become obsolete. */
/* USER CODE BEGIN 4 */
/* USER CODE END 4 */
#endif

/**
 * ----------------------------------------------------------------------
 * Function given to help user to continue LwIP Initialization
 * Up to user to complete or change this function ...
 * Up to user to call this function in main.c in while (1) of main(void)
 *-----------------------------------------------------------------------
 * Read a received packet from the Ethernet buffers
 * Send it to the lwIP stack for handling
 * Handle timeouts if LWIP_TIMERS is set and without RTOS
 * Handle the llink status if LWIP_NETIF_LINK_CALLBACK is set and without RTOS
 */
void MX_LWIP_Process(void)
{
#if 0	
	static int8_t i=1;
	if(i){
		printf("MX_LWIP_Process");
		i=0;
	}
#endif	
    ethernetif_input(&gnetif);
    sys_check_timeouts();
}

#if defined ( __CC_ARM )  /* MDK ARM Compiler */
/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
    sio_fd_t sd;

    sd = 0; // dummy code

    return sd;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
    /* USER CODE BEGIN 8 */
    /* USER CODE END 8 */
}

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
    u32_t recved_bytes;

    /* USER CODE BEGIN 9 */
    recved_bytes = 0; // dummy code
    /* USER CODE END 9 */
    return recved_bytes;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
    u32_t recved_bytes;

    /* USER CODE BEGIN 10 */
    recved_bytes = 0; // dummy code
    /* USER CODE END 10 */
    return recved_bytes;
}
#endif /* MDK ARM Compiler */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
