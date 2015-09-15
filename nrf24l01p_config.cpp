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

uint8_t NRF24L01p::get_config() {
    uint8_t config;

    spi.read_register(CONFIG, &config, sizeof (uint8_t));

    return config;
}

void NRF24L01p::set_enable_crc(bool enable) {
    uint8_t config;

    if (enable) {
        config = (get_config()) | 1 << CONFIG_EN_CRC;
    } else {
        config = (get_config()) & ~(1 << CONFIG_EN_CRC);
    }

    spi.write_register(CONFIG, &config, sizeof (uint8_t));
}

void NRF24L01p::set_crco_encoding_scheme(bool encoding_scheme) {
    uint8_t config;

    if (encoding_scheme) {
        config = (get_config()) | 1 << CONFIG_CRCO;
    } else {
        config = (get_config()) & ~(1 << CONFIG_CRCO);
    }

    spi.write_register(CONFIG, &config, sizeof (uint8_t));
}

void NRF24L01p::setup_retries(uint8_t ard, uint8_t arc) {
    uint8_t data = (ard << SETUP_RETR_ARD | arc << SETUP_RETR_ARC);

    spi.write_register(SETUP_RETR, &data, sizeof (uint8_t));
}

void NRF24L01p::set_channel(uint8_t channel) {
    spi.write_register(RF_CH, &channel, sizeof (uint8_t));
}
