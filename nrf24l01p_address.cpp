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

void NRF24L01p::set_address_width(uint8_t *address_width) {
    spi.write_register(SETUP_AW, address_width, sizeof(uint8_t));
}

void NRF24L01p::set_tx_addr(uint8_t *address, uint8_t width) {
    spi.write_register(TX_ADDR, address, width);
}

void NRF24L01p::set_rx_addr(uint8_t pipe, uint8_t* address, uint8_t width) {
    spi.write_register(RX_ADDR_P0 + pipe, address, width);
}

void NRF24L01p::set_rx_addr_p0(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P0, address, width);
}

void NRF24L01p::set_rx_addr_p1(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P1, address, width);
}

void NRF24L01p::set_rx_addr_p2(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P2, address, width);
}

void NRF24L01p::set_rx_addr_p3(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P3, address, width);
}

void NRF24L01p::set_rx_addr_p4(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P4, address, width);
}

void NRF24L01p::set_rx_addr_p5(uint8_t *address, uint8_t width) {
    spi.write_register(RX_ADDR_P5, address, width);
}
