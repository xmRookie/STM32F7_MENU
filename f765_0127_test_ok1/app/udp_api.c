
#include "common.h"
#include "lwip/ip4.h"


#define P_ARP       0x0806
#define P_RARP      0x8035
#define P_IP		0x0800

 extern struct netif gnetif;
/* Global Ethernet handle */
extern ETH_HandleTypeDef heth;

static uint8_t link_state = 0;

 ip_addr_t destIPaddr;             //！！！！！！！！！！
// ip_addr_t sinkIPaddr;
uint16_t  destPort;
uint16_t  sinkPort;

extern ip4_addr_t ipaddr;
extern ip4_addr_t remoteipaddr;

extern NETIF_PAR netif_par;
extern NETIF_REMOTE_PAR netif_remote_par;

void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port);
void udp_echoclient_send(void);

u8_t   data[100] = {0x00};
__IO uint32_t message_count = 0;
static struct udp_pcb *upcb; //for server
static struct udp_pcb *cpcb; //for client

struct pbuf mPortBuf;

static uint16_t state=0;

typedef struct
{
	uint8_t dest_mac[6];
	uint8_t src_mac[6];
	uint8_t type[2];
} EtherHdr;

typedef struct
{
    uint8_t                  socpot[2];                     // 请求端端口
    uint8_t                  dstpot[2];                     // 服务器端口
    uint8_t                  len[2];                     // UDP数据长度
    uint8_t                  chksum[2];                     // UDP包校验核
   
} UDPPACKA_TYPE;


typedef struct
{
    uint8_t                  V_H_T[2];          // Version(4 bit) : Header Length(4 bit) : Type of Service(8 bit)
    uint8_t                  totlen[2];         // Total Length
    uint8_t                  idtf[2];           // Identifier
    uint8_t                  FLA_OFS[2];        // Eragmentation Flags(3 bit) : Eragment Offset(13 bit)
    uint8_t                  LIVE_PTC[2];       // Time To Live(8 bit) : Protocol(8 bit)
    uint8_t                  chksum[2];       // Header Checksum
    uint8_t                  socip[4];          // Sourct IP Address
    uint8_t                  dstip[4];          // Dest. IP Address
} IPPACKA_TYPE;



typedef struct
{
    EtherHdr               eth;           // 14
    IPPACKA_TYPE           ipH;            // 20
    UDPPACKA_TYPE          udpH;            // 8
    uint8_t                dt[1316];                    // UDP数据
}NETPACKA_TYPE; 

NETPACKA_TYPE   netUdpPack;


static uint16_t HexBuf_To_U16(uint8_t *src)
{
    return ((src[0] * 0x100) + src[1]);
}


static void U16_To_HexBuf(uint8_t *dest, uint16_t bin)
{
    *dest++ = bin >> 8;
    *dest = bin;
}


void udp_pack_init(void)
{
	uint16_t id;
	uint16_t len;
	
	len = 1316;
	len += UDP_HLEN;

    U16_To_HexBuf(netUdpPack.udpH.socpot, netif_par.port);
    U16_To_HexBuf(netUdpPack.udpH.dstpot, netif_remote_par.port);

    U16_To_HexBuf(netUdpPack.udpH.len, len);
    netUdpPack.udpH.chksum[1] =  netUdpPack.udpH.chksum[0] = 0;


	len += IP_HLEN;
	netUdpPack.ipH.V_H_T[0] = 0x45;
	netUdpPack.ipH.V_H_T[1] = 0x00;
	U16_To_HexBuf(netUdpPack.ipH.totlen, len);
	//Inc_ip_id();
	id = get_ip_id();
	U16_To_HexBuf(netUdpPack.ipH.idtf,id);
    U16_To_HexBuf(netUdpPack.ipH.FLA_OFS, 0x0000);
    netUdpPack.ipH.LIVE_PTC[0] = 0xFF;//ttl
    netUdpPack.ipH.LIVE_PTC[1] = 0x11;//proto
    memcpy(netUdpPack.ipH.socip, netif_par.ip, 4);
    memcpy(netUdpPack.ipH.dstip, netif_remote_par.ip,4);
	netUdpPack.ipH.chksum[1] =  netUdpPack.ipH.chksum[0] = 0;

	memcpy(netUdpPack.eth.dest_mac, netif_remote_par.mac, 6);
	memcpy(netUdpPack.eth.src_mac, netif_par.mac, 6);
	U16_To_HexBuf(netUdpPack.eth.type,0x0800);
	
}

extern void ap_udp_set_mac(uint8_t *mac)
{
	 memcpy(netUdpPack.eth.dest_mac, mac, 6);
}
int8_t date[10]={0x00,0x80,0x55,0x04,0x05,0x06,0x07,0x08,0x09,0x0a};

extern void eth_send(uint8_t *pstr,uint8_t dstpot_1,uint8_t dstpot_2,uint16_t data_len)
{
	//for test
	uint16_t i;
	uint16_t id;
	
	static uint8_t test_num=0;
	
//	memset(dat,0x5a,sizeof(dat));
	
	i = data_len;
//test	
#ifndef FPGA_LINK
	
	#if 0
	if(pstr[0] == 0x00){
		test_num++;
		if(test_num>255)
			test_num = 0;
		
			pstr[2] = test_num;
			pstr[1] = test_num;
	}
	//pstr[2] = test_num;
	//if(pstr[1] == 0x00)
	//	pstr[2] = 0x55;
	#endif
#endif	
						//i=sizeof(date);
						mPortBuf.len = i;

            Inc_ip_id();
            id = get_ip_id();
            U16_To_HexBuf(netUdpPack.ipH.idtf, id);

            i += UDP_HLEN;
            U16_To_HexBuf(netUdpPack.udpH.len, i);
            i += IP_HLEN;
            U16_To_HexBuf(netUdpPack.ipH.totlen, i);

            //netUdpPack.dt = &udp_tx_buf[av_udp_stream.Read];
            mPortBuf.next = NULL;
            mPortBuf.tot_len = i + LINK_HLEN;

            mPortBuf.payload = (void *)&netUdpPack;
						
						
						netUdpPack.udpH.dstpot[0]=dstpot_1;       //默认端口  目的端口  5003
						netUdpPack.udpH.dstpot[1]=dstpot_2;
						
				//		udp_ts_output(&gnetif, &mPortBuf, date, 0);   
				udp_ts_output(&gnetif, &mPortBuf, pstr, 0);  
			//test end			
}

extern void udp_TimerTick(void)
{
	uint16_t id;
	if(state == 0 && get_remoteLwip_udp_link())
	{
	    state=2;
		memcpy(netUdpPack.eth.dest_mac, netif_remote_par.mac, 6);
	//	printf("static =0 ");
	}
	else if(state>=2 && state<102)
    {
    	state++;
		Inc_ip_id();
		id = get_ip_id();
		U16_To_HexBuf(netUdpPack.ipH.idtf,id);
		mPortBuf.len = 1358;
		mPortBuf.next = NULL;
		mPortBuf.payload = (void *)&netUdpPack;
		++netUdpPack.dt[0];
    udp_ts_output(&gnetif, &mPortBuf, date, 0);
	//		printf("static >=2 ");
     }else{
	//		 udp_ts_output(&gnetif, &mPortBuf, date, 0);
	//		printf("send");
		 }
}



void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    /* Tell the client that we have accepted it */
    // udp_sendto(upcb, p,addr,port);
    /* Free the p buffer */
	 /*increment message count */
  	message_count++;
   //printf("udp_echoserver_receive_callback I\n");
  	/* Free receive pbuf */
 	 pbuf_free(p);
}

/*
void HAL_ETH_TxCpltCallback(ETH_HandleTypeDef *heth)
{
	 printf("HAL_ETH_TxCpltCallback\n");
}
void HAL_ETH_ErrorCallback(ETH_HandleTypeDef *heth)
{
	printf("HAL_ETH_ErrorCallback\n");
}
*/

extern void udp_send_to_host(uint8_t *buf, uint16_t len)
{
#if 0
    struct pbuf *p;
    p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);

    if(p != NULL) {
        /* copy data to pbuf */
        pbuf_take(p, (char*)buf, len);
        udp_send(cpcb, p);
        pbuf_free(p);
    }
	#endif
}

//UDP
extern void udp_echoclient_connect(void)
{
    err_t err;
    /*assign destination IP address */
    //IP4_ADDR( &destIPaddr, 192, 168, 0, 190 );
   // IP4_ADDR( &sinkIPaddr, 192, 168, 0, 188 );
    //sinkIPaddr = get_Lwip_Ip();
    sinkPort = get_Lwip_port();
   // destIPaddr =  get_remoteLwip_Ip();        //！！！
    destPort = get_remoteLwip_port();
	memset(netUdpPack.dt,0x55,1316);
    /* Create a new UDP control block  */
    //upcb = NULL;//;udp_new();
    udp_pack_init();
    cpcb = udp_new();
	if(cpcb != NULL) {
        err = udp_bind(cpcb,&ipaddr, sinkPort);
        if(err == ERR_OK) {
            printf("local port ip  %x sinkPort %d\r\n", ipaddr.addr, cpcb->local_port);
            /* Set a receive callback for the upcb */
            udp_recv(cpcb, udp_receive_callback, NULL);
		}
		/* configure destination IP address and port */
        err = udp_connect(cpcb, &remoteipaddr, destPort);
        if(err == ERR_OK) {
             printf("udp_connect ip %x destPort is %d\r\n", remoteipaddr.addr, destPort);
            /* Set a receive callback for the upcb */
            udp_recv(cpcb, udp_echoserver_receive_callback, NULL);
        }
    }

	state=0;

}

void udp_demo_loop_init(uint8_t *buf,uint16_t len)
{
	
	 //pbuf_take(mPortBuf, (char*)data, len);
}

//
void udp_echoclient_send(void)
{
	state=2;
}

//
void udp_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	/*increment message count */
  message_count++;
 
  /* Free receive pbuf */
  pbuf_free(p);
}



static void udpFun(uint8_t *netBuffer,uint16_t len)
{
    NETPACKA_TYPE *rxpt;

    rxpt = (NETPACKA_TYPE *)netBuffer;

    printf("udp len %d\r\n",HexBuf_To_U16(rxpt->udpH.len));

	
}

extern void nicIP(uint8_t *netBuffer,uint16_t len)
{

    EtherHdr *rxpt;

    rxpt = (EtherHdr *)netBuffer;
	
	 if (memcmp(netif_par.mac, rxpt->dest_mac, 6) != 0)//???????????????
	 {
		 return;
	 }

    switch(HexBuf_To_U16(rxpt->type))          
    {
        case P_ARP:                                 
            //arpFun(netBuffer);
            break;
        case P_RARP:                               
            break;
        case P_IP:                                  
            udpFun(netBuffer,len);//376 or 188 or 42
            break;
        default:
            break;
    }
}


/*
extern void Set_remoteLwip_udp_link(uint8_t link)
{
	if(link)
		link_state = NET_LINKED;
	else
		link_state = NET_UNLINK;
}
extern uint8_t get_remoteLwip_udp_link(void)
{
    return link_state;
}
*/

