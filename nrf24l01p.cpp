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

NRF24L01p::NRF24L01p(uint8_t ce_pin, uint8_t csn_pin) : spi(ce_pin, csn_pin) {

}

void NRF24L01p::init(void) {
    spi.init();

    // *** Reset to reset values  ***

    // Config 
    set_enable_crc(1);
    set_crco_encoding_scheme(0);

    // Features
    enable_auto_ack(1);
    enable_ack_payload(0);
    enable_dynamic_payload(0);
    enable_dynamic_ack(0);

    // Retries
    setup_retries(0, 3);

    // 2 MBPS, -0dBm
    set_rf_setup(0, 0, 0, 1, 0b11);

    power_up();
    
    set_standby1();

    flush_rx();
    flush_tx();

    reset_rx_interrupt();
    reset_tx_interrupts();

    set_prim_tx();
}

uint8_t NRF24L01p::get_status(void) {
    return spi.write_command(NOP);
}

void NRF24L01p::set_rf_setup(bool cont_wave, bool rf_dr_low, bool pll_lock, bool rf_dr_high, uint8_t rf_pwr) {
    uint8_t data = cont_wave << RF_SETUP_CONT_WAVE |
            rf_dr_low << RF_SETUP_RF_DR_LOW |
            pll_lock << RF_SETUP_PLL_LOCK |
            rf_dr_high << RF_SETUP_RF_DR_HIGH |
            rf_pwr << RF_SETUP_RF_PWR;

    spi.write_register(RF_SETUP, &data, sizeof (uint8_t));
}

