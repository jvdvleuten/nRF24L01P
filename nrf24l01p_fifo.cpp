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

#include <stdio.h>
#include <string.h>
#include <bcm2835.h>

#include "spi_commands.h"
#include "register_map_table.h"

uint8_t NRF24L01P::get_fifo_status(void) {
    uint8_t fifo_status;

    spi.read_register(FIFO_STATUS, &fifo_status, sizeof (uint8_t));

    return fifo_status;
}

void NRF24L01P::flush_tx() {
    spi.write_command(FLUSH_TX);
}

void NRF24L01P::flush_rx() {
    spi.write_command(FLUSH_RX);
}

bool NRF24L01P::payloads_available() {
    return !(get_fifo_status() & 1 << FIFO_STATUS_RX_EMPTY);
}