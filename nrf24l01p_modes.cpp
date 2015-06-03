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


void NRF24L01p::set_standby2() {    
    set_prim_tx();    
    spi.set_ce_pin(1);
}

void NRF24L01p::set_standby1() {
    spi.set_ce_pin(0);
}

void NRF24L01p::set_prim_rx() {
    spi.set_ce_pin(0);

    uint8_t config = get_config() | 1 << CONFIG_PRIM_RX;

    spi.write_register(CONFIG, &config, sizeof (uint8_t));

    uint8_t payload_width = 32;

    spi.write_register(RX_PW_P0, &payload_width, sizeof (uint8_t));
    spi.write_register(RX_PW_P1, &payload_width, sizeof (uint8_t));
    spi.write_register(RX_PW_P2, &payload_width, sizeof (uint8_t));
    spi.write_register(RX_PW_P3, &payload_width, sizeof (uint8_t));
    spi.write_register(RX_PW_P4, &payload_width, sizeof (uint8_t));
    spi.write_register(RX_PW_P5, &payload_width, sizeof (uint8_t));

    spi.set_ce_pin(1);

    TimeUtil::delay_microseconds(130); // Standby to RX mode
}

void NRF24L01p::set_prim_tx() {
    spi.set_ce_pin(0);

    uint8_t config = get_config() & ~(1 << CONFIG_PRIM_RX);
    spi.write_register(CONFIG, &config, sizeof (uint8_t));    
}
