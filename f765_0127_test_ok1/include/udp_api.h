#ifndef __UDP_API_H
#define __UDP_API_H
#ifdef __cplusplus
 extern "C" {
#endif

extern void udp_demo_loop_init(uint8_t *buf,uint16_t len);
extern void  udp_echoclient_send(void);
extern void udp_echoclient_connect(void);
extern void udp_TimerTick(void);
extern void udp_send_to_host(uint8_t *buf,uint16_t len);
	 
	 extern void ap_udp_set_mac(uint8_t *mac);
	 
	 extern void Set_remoteLwip_udp_link(uint8_t link);
	 extern uint8_t get_remoteLwip_udp_link(void);
	 
	 extern void eth_send(uint8_t *pstr,uint8_t dstpot_1,uint8_t dstpot_2,uint16_t data_len);
	 
#ifdef __cplusplus
}
#endif
#endif /* __UDP_API_H */
