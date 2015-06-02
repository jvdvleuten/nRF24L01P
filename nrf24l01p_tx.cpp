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

#include "nrf24l01p.h"

#include "spi_commands.h"
#include "register_map_table.h"
#include "time_util.h"

bool NRF24L01p::tx_fifo_empty() {
    return get_fifo_status() & (1 << FIFO_STATUS_TX_EMPTY);
}

void NRF24L01p::block_when_tx_mode_more_than_4ms() {

    // Timing functions are to slow and not precise enough for the Arduino
    // So we calculate how many packets can be send in 4 ms and keep track
    // of that.
    //
    // 1 byte preamble + 3 byte address + 32 byte payload
    //          = 36 bytes (minimum size with full payload)
    //
    // 2 MBPS = 250 kb/s = 250 byte/ms
    //
    // In 4 ms, there will be 1000 bytes sent
    // 1000 bytes / 36 bytes per packet = 27,7 packets/4ms
    //
    // So halt the TX FIFO every 28 packets will do the job
    //

    bool is_tx_fifo_empty = tx_fifo_empty();

    if (is_tx_fifo_empty) {
        fifo_tx_written_counter = 0;
    } 
    
    if (!is_tx_fifo_empty && fifo_tx_written_counter > 28) {        
        while (!tx_fifo_empty()) {
            // The nRF24L01+ transmitter PLL operates in open loop when in TX
            // mode. It is important never to keep the nRF24L01+ in TX mode for more than 4ms at a time. If the
            // Enhanced ShockBurst™ features are enabled, nRF24L01+ is never in TX mode longer than 4ms.
            // Do not use TX mode for more than 4 ms without auto acknowledgement
        }
        fifo_tx_written_counter = 0;
    }
}

void NRF24L01p::write_tx_payload(void* buf, uint8_t length) {

    if (!auto_ack_enabled) {
        block_when_tx_mode_more_than_4ms();
    }

    uint8_t tx_payload[32];
    uint8_t *p_tx_payload = tx_payload;

    uint8_t *p_buf = static_cast<uint8_t*> (buf);

    for (int i = 0; i < length; i++) {
        *p_tx_payload++ = *p_buf++;
    }

    spi.write_command(W_TX_PAYLOAD, tx_payload, 32);

    if (!auto_ack_enabled) {
        fifo_tx_written_counter++;       
    }
}

void NRF24L01p::write_tx_payload_noack(void* buf, uint8_t length) {
    block_when_tx_mode_more_than_4ms();

    uint8_t tx_payload[32];
    uint8_t *p_tx_payload = tx_payload;

    uint8_t *p_buf = static_cast<uint8_t*> (buf);

    for (int i = 0; i < length; i++) {
        *p_tx_payload++ = *p_buf++;
    }

    spi.write_command(W_TX_PAYLOAD_NOACK, tx_payload, 32);
}

bool NRF24L01p::transmit(void* buf, uint8_t length) {

    write_tx_payload(buf, length);

    // Pulse CE to start TX mode
    spi.set_ce_pin(1);

    // Pulse add least 10 us to start TX mode
    // Thce = 10us
    TimeUtil::delay_microseconds(11);

    // We are transmitting only one packet
    spi.set_ce_pin(0);

    TimeUtil::delay_microseconds(130);

    uint8_t status = get_status();

    while (!(status & (1 << STATUS_TX_DS | 1 << STATUS_MAX_RT))) {
        status = get_status();
    }

    reset_tx_interrupts();

    if (status & 1 << STATUS_TX_DS) {
        // Other party needs 130 us to get to RX mode again.
        // Tstby2a = 130us
        TimeUtil::delay_microseconds(130);

        return 1;
    }

    flush_tx();

    return 0;
}
