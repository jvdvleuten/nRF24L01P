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

void NRF24L01P::reset_tx_interrupts() {
    uint8_t status = get_status() & (1 << STATUS_MAX_RT | 1 << STATUS_TX_DS);

    spi.write_register(STATUS, &status, sizeof (uint8_t));
}

void NRF24L01P::reset_rx_interrupt() {
    uint8_t status = get_status() & (1 << STATUS_RX_DR);

    spi.write_register(STATUS, &status, sizeof (uint8_t));
}

bool NRF24L01P::tx_fifo_full() {
    return get_status() & (1 << STATUS_TX_FULL);
}

bool NRF24L01P::tx_max_rt() {
    return get_status() & (1 << STATUS_MAX_RT);
}
