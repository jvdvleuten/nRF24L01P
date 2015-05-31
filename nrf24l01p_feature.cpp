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

uint8_t NRF24L01P::get_feature() {
    uint8_t feature;

    spi.read_register(FEATURE, &feature, sizeof (uint8_t));

    return feature;
}

void NRF24L01P::enable_auto_ack(bool enable) {
    uint8_t en_aa;

    auto_ack_enabled = enable;

    en_aa = enable ? 0b00111111 : 0b00000000;

    spi.write_register(EN_AA, &en_aa, sizeof (uint8_t));
}

void NRF24L01P::enable_ack_payload(bool enable) {
    uint8_t feature;

    if (enable) {
        feature = get_feature() | 1 << FEATURE_EN_ACK_PAY;
    } else {
        feature = get_feature() & ~(1 << FEATURE_EN_ACK_PAY);
    }

    spi.write_register(FEATURE, &feature, sizeof (uint8_t));
}

void NRF24L01P::enable_dynamic_payload(bool enable) {
    uint8_t feature;

    if (enable) {
        feature = get_feature() | 1 << FEATURE_EN_DPL;
        spi.write_register(FEATURE, &feature, sizeof (uint8_t));

        // Needs auto ack
        enable_auto_ack(1);

        // Enable the dynamic payload for all pipes
        uint8_t dynpd = 0b00111111;
        spi.write_register(DYNPD, &dynpd, sizeof (uint8_t));
    } else {
        feature = get_feature() & ~(1 << FEATURE_EN_DPL);
        spi.write_register(FEATURE, &feature, sizeof (uint8_t));

        // Disable the dynamic payload for all pipes
        uint8_t dynpd = 0b00000000;
        spi.write_register(DYNPD, &dynpd, sizeof (uint8_t));
    }
}

void NRF24L01P::enable_dynamic_ack(bool enable) {
    uint8_t feature;

    if (enable) {
        feature = get_feature() | 1 << FEATURE_EN_DYN_ACK;
    } else {
        feature = get_feature() & ~(1 << FEATURE_EN_DYN_ACK);
    }

    spi.write_register(FEATURE, &feature, sizeof (uint8_t));
}
