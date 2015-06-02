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

#include "time_util.h"
#include "spi_commands.h"
#include "register_map_table.h"

bool NRF24L01p::rx_data_ready() {
    bool data_ready = get_status() & (1 << STATUS_RX_DR);

    if (data_ready) {
        /*
         * When the RX_DR interrupt is 1, the receiver will send back
         * an ACK package to the transmitter.
         * This takes 2 x Tstby2a (130us) + Tack (165 us for 2MBPS)
         * 
         * To ensure this receiver can change to TX mode and other party has
         * received the ACK package, we wait that time period.         
         */
        TimeUtil::delay_microseconds(130 + 165 + 130); 
        
        reset_rx_interrupt();
    }

    return data_ready;
}

void NRF24L01p::read_rx_payload(void* buf, uint8_t length) {
    uint8_t rx_payload[32];
    uint8_t *p_rx_payload = rx_payload;

    spi.read_command(R_RX_PAYLOAD, rx_payload, 32);

    uint8_t *p_buf = static_cast<uint8_t*> (buf);

    for (int i = 0; i < length; i++) {
        *p_buf++ = *p_rx_payload++;
    }
}
