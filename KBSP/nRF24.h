/*
* ----------------------------------------------------------------------------
* “THE COFFEEWARE LICENSE” (Revision 1):
* <ihsan@kehribar.me> wrote this file. As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a coffee in return.
* -----------------------------------------------------------------------------
* This library is based on this library: 
*   https://github.com/aaronds/arduino-nrf24l01
* Which is based on this library: 
*   http://www.tinkerer.eu/AVRLib/nRF24L01
* -----------------------------------------------------------------------------
*/

#ifndef __nRF24_H__
#define __nRF24_H__

#ifdef __cplusplus
 extern "C" {
#endif


#include "nRF24_Def.h"
#include <stdint.h>

#include "ktype.h"  // kwj
#include "spi.h" // kwj

// SPI 20M bps
#define nRF_CE	   PortA.b11  //191129
#define nRF_IRQ    PinA.b12   //191129
#define nRF_CSN    PortA.b15  //191129
#define nRF24_SPI	 hspi1      //191129
//#define nRF_SCK  PortA.b5
//#define nRF_MISO PinA.b6
//#define nRF_MOSI PortB.b1

/*
// SPI 4M bps
#define nRF24_SPI		hspi1  // kwj
//#define nRF_IRQ  PinA.b10
#define nRF_CE	 PortA.b4
#define nRF_CSN  PortA.b7
//#define nRF_SCK  PortA.b5
//#define nRF_MISO PinA.b6
//#define nRF_MOSI PortB.b1
*/

//#define nrf24_ce_digitalWrite(state)  PortA.b0 = state
//#define nrf24_csn_digitalWrite(state)  PortA.b0 = state
//#define nrf24_sck_digitalWrite(state)  PortA.b0 = state
//#define nrf24_mosi_digitalWrite(state)  PortA.b0 = state
//#define nrf24_miso_digitalRead()  PinA.b0


#define LOW  0
#define HIGH 1

#define nrf24_ADDR_LEN 5
#define nrf24_CONFIG ((1<<EN_CRC)|(0<<CRCO))

#define NRF24_TRANSMISSON_OK 0
#define NRF24_MESSAGE_LOST   1

/* adjustment functions */
void    nrf24_init(void);
void    nrf24_rx_address(uint8_t* adr);
void    nrf24_tx_address(uint8_t* adr);
void    nrf24_config(uint8_t channel, uint8_t pay_length); // pay_length -- Data Size

/* state check functions */
uint8_t nrf24_dataReady(void);
uint8_t nrf24_isSending(void);
uint8_t nrf24_getStatus(void);
uint8_t nrf24_rxFifoEmpty(void);

/* core TX / RX functions */
void    nrf24_send(uint8_t* value);
void    nrf24_getData(uint8_t* data);

/* use in dynamic length mode */
uint8_t nrf24_payloadLength(void);

/* post transmission analysis */
uint8_t nrf24_lastMessageStatus(void);
uint8_t nrf24_retransmissionCount(void);

/* Returns the payload length */
uint8_t nrf24_payload_length(void);

/* power management */
void    nrf24_powerUpRx(void);
void    nrf24_powerUpTx(void);
void    nrf24_powerDown(void);

/* low level interface ... */
uint8_t spi_transfer(uint8_t tx); //kwj
uint8_t nrf24_transferSync(uint8_t* dataout,uint8_t* datain,uint8_t len); //kwj
uint8_t nrf24_transmitSync(uint8_t* dataout,uint8_t len); //kwj
void    nrf24_configRegister(uint8_t reg, uint8_t value);
void    nrf24_readRegister(uint8_t reg, uint8_t* value, uint8_t len);
void    nrf24_writeRegister(uint8_t reg, uint8_t* value, uint8_t len);

/* -------------------------------------------------------------------------- */
/* You should implement the platform spesific functions in your code */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* In this function you should do the following things:
 *    - Set MISO pin input
 *    - Set MOSI pin output
 *    - Set SCK pin output
 *    - Set CSN pin output
 *    - Set CE pin output     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_setupPins(void);

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_ce_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 CE pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_csn_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 SCK pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_sck_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 MOSI pin control function
 *    - state:1 => Pin HIGH
 *    - state:0 => Pin LOW     */
/* -------------------------------------------------------------------------- */
//extern void nrf24_mosi_digitalWrite(uint8_t state);

/* -------------------------------------------------------------------------- */
/* nrf24 MISO pin read function
 *    - returns: Non-zero if the pin is high */
/* -------------------------------------------------------------------------- */
//extern uint8_t nrf24_miso_digitalRead(void);



#ifdef __cplusplus
}
#endif


#endif
