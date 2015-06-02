/* 
 * Copyright (C) 2015  Johan van der Vleuten
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA. 
 */

#ifndef NRF24L01p_H
#define	NRF24L01p_H

#include <stdint.h>
#include "spi_interface.h"

class NRF24L01p {
    SpiInterface spi;
public:
    NRF24L01p(uint8_t ce_pin, uint8_t csn_pin);
    
    void init(void);    
     
    // Get config byte
    uint8_t get_config(void); 
    // Get status byte
    uint8_t get_status(void);  
    // Get fifo status byte
    uint8_t get_fifo_status(void);
    // Get feature byte
    uint8_t get_feature(void);
    
    void setup_retries(uint8_t ard, uint8_t arc);
    
    void set_enable_crc(bool enable);
    /*
     * Setup 1 or 2 byte encoding scheme
     * 
     * @param encoding_scheme 0:1byte, 1:2byte
     */
    void set_crco_encoding_scheme(bool encoding_scheme);  
    
    void set_rf_setup(bool cont_wave, bool rf_dr_low, bool pll_lock, bool rf_dr_high, uint8_t rf_pwr);
    
    // Enable or disable auto ack for all reading pipes
    void enable_auto_ack(bool enable);
    /* 
     * Enable or disable dynamic payload for all pipes.
     * Will enable auto_ack as well, as it is required for dynamic payload.
     */
    void enable_dynamic_payload(bool enable);
    
    // Enable payload with ack message
    void enable_ack_payload(bool enable);
    
    /*
     * Enables w_tx_payload_noack() for selective no ack transmitting.
     * 
     * This is not the same as disabling auto ack with enable_auto_ack(0) as 
     * packages sent with w_tx_payload() will be acknowledged by the receiver.
     */
    void enable_dynamic_ack(bool enable);
    
    void set_address_width(uint8_t *address_width);
    void set_tx_addr(uint8_t *address, uint8_t width);
    void set_rx_addr_p0(uint8_t *address, uint8_t width);
    void set_rx_addr_p1(uint8_t *address, uint8_t width);      
    
    // Power up transceiver
    void power_up(void);
    
    /*
     * Checks maximum number of TX retransmits interrupt.
     * Use this function to check for failed packets during transmit.
     * 
     * If the auto retransmit counter exceeds the maximum retries, 
     * the function returns true. The payload in TX FIFO is NOT removed. 
     * Call flush_tx() to remove the payload. 
     * 
     * The transmitter will not enter TX mode while tx_max_rt() returns true.
     * Reset the max_rt interrupt by calling reset_tx_interrupts();
     * 
     * @return true when auto retransmit counter exceeds the maximum retries. 
     */
    bool tx_max_rt(void);
    
    // Reset tx_did_send() and tx_max_rt() interrupt.
    void reset_tx_interrupts(void);
    // Reset rx_data_ready() interrupt
    void reset_rx_interrupt(void);
    
    /*
     * Standby-I mode is used to minimize average current consumption while
     * maintaining short start up times. 
     * 
     * Call power_up() before calling set_standby1();
     */
    void set_standby1(); 
    
    /*
     * In standby-II mode extra clock buffers are active and more current is 
     * used compared to standby-I mode. nRF24L01+ enters standby-II mode 
     * if CE is held high on a PTX device with an empty TX FIFO. 
     * If a new packet is uploaded to the TX FIFO, the PLL
     * immediately starts and the packet is transmitted after the normal 
     * PLL settling delay (130μs).
     */
    void set_standby2();
    
    void set_prim_rx(void);
    void set_prim_tx(void);
    
    bool tx_fifo_full(void);
    bool tx_fifo_empty(void);
    
    void flush_tx(void);
    void flush_rx(void);  
    
    /*
     * Checks Data Ready in RX FIFO interrupt. 
     * 
     * Read the payload with read_rx_payload() when function returns true. 
     * Use payloads_available() to check if there are more packets available.
     * 
     * This function ensures both parties are ready to send and receive after
     * a packet has been read with read_rx_payload() and other party used 
     * transmit().
     * 
     * The interrupt will be automatically reset when rx_data_ready 
     * returns true.
     * 
     * @return true when new data arrives in RX FIFO. 
     */
    bool rx_data_ready(void);
    
    /*
     * Checks if payloads are available in the RX FIFO.
     * 
     * This does not ensure both parties are ready to send or receive packets
     * when a packet has been read with read_rx_payload().
     * 
     * @return true when RX FIFO not empty.
     */
    bool payloads_available(void);
    
    /*
     * Read RX payload from the RX FIFO.
     * 
     * @param buf buffer to read the payload in
     * @param length length of the payload
     */
    void read_rx_payload(void *buf, uint8_t length);    
    
    /*
     * Transmit a payload and ensuring both parties are ready 
     * to send and receive after single transmit.
     * 
     * Will acknowledge the payload when auto ack is enabled.
     * 
     * The receiver should use rx_data_ready() to ensure both parties are
     * ready to send an receive again.
     * 
     * @param buf buffer to read the payload from
     * @param length length of the payload 
     */
    bool transmit(void *buf, uint8_t length);   
    
    /*
     * Write payload into TX buffer.
     * 
     * To send the payload in TX Mode, ensure transmitter is in Standby2 mode
     * by calling set_standby2(). This will transmit the payload as soon
     * as writing in the TX FIFO is completed.
     * 
     * Use tx_fifo_full() to check if the TX FIFO is full before writing to
     * the TX FIFO. 
     * 
     * When TX FIFO is full, keep the transmitter in Standby 2 mode. 
     * If the TX FIFO is not empty the nRF24L01+ remains 
     * in TX mode and transmits the next packet until TX_FIFO is empty 
     * 
     * Check tx_max_rt() to see if the auto retransmit counter exceeds 
     * the maximum retries. When tx_max_tr() is true, the transmitter
     * will not enter TX mode and therefore the TX FIFO will not empty.
     * Flush the TX FIFO when this happens and reset the TX interrupts with
     * flush_tx() and reset_tx_interrupts().
     * 
     * If the TX FIFO is empty the nRF24L01+ goes into standby-II mode. 
     * 
     * @param buf buffer to read the payload from
     * @param length length of the payload 
     */
    void write_tx_payload(void *buf, uint8_t length);  
    
    /*
     * Write payload into TX buffer and set the NO_ACK flag bit in the 
     * Packet Control Field.
     * 
     * Needs to have dynamic_ack enabled with enable_dynamic_ack(1);
     * 
     * To send the payload in TX Mode, ensure transmitter is in Standby2 mode
     * by calling set_standby2(). This will transmit the payload as soon
     * as writing in the TX FIFO is completed.  
     * 
     * Use tx_fifo_full() to check if the TX FIFO is full before writing to
     * the TX FIFO. 
     * 
     * When TX FIFO is full, keep the transmitter in Standby 2 mode. 
     * If the TX FIFO is not empty the nRF24L01+ remains 
     * in TX mode and transmits the next packet until TX_FIFO is empty.
     * 
     * When you use this option the PTX goes directly to standby-I mode after
     * transmitting the packet. The PRX does not transmit an ACK packet when
     * it receives the packet.
     * 
     * If the TX FIFO is empty the nRF24L01+ goes into standby-II mode. 
     * 
     * @param buf buffer to read the payload from
     * @param length length of the payload 
     */
    void write_tx_payload_noack(void *buf, uint8_t length);  
      
private:        
    bool auto_ack_enabled;
    void block_when_tx_mode_more_than_4ms(void);    
    unsigned long fifo_tx_empty_timestamp;
};

#endif	/* NRF24_H */

