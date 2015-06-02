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

#include "../../spi_interface.h"

#include <stdint.h>
#include <bcm2835.h>

#include "../../spi_commands.h"
#include "../../register_map_table.h"

SpiInterface::SpiInterface(uint8_t ce_pin, uint8_t csn_pin) {
    ce_pin_ = ce_pin;
    csn_pin_ = csn_pin;
}

void SpiInterface::init(void) {
    if (!bcm2835_init()) {
        return;
    }

    bcm2835_spi_begin();
    
    bcm2835_gpio_fsel(ce_pin_, BCM2835_GPIO_FSEL_OUTP);
    set_ce_pin(0);    

    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
    bcm2835_spi_chipSelect(csn_pin_);
    bcm2835_spi_setChipSelectPolarity(csn_pin_, 0);
}

void SpiInterface::set_ce_pin(bool state) {
    bcm2835_gpio_write(ce_pin_, state);
}

void SpiInterface::write_register(uint8_t register_address, uint8_t *buf, uint8_t len) {

    uint8_t command = (W_REGISTER | (0b00011111 & register_address));

    return write_command(command, buf, len);
}

void SpiInterface::read_register(uint8_t register_address, uint8_t* buf, uint8_t len) {

    uint8_t command = (R_REGISTER | (0b00011111 & register_address));

    return read_command(command, buf, len);
}

uint8_t SpiInterface::write_command(uint8_t command) {
    return bcm2835_spi_transfer(command);
}

void SpiInterface::write_command(uint8_t command, uint8_t* buf, uint8_t len) {
    uint8_t mosi[32 + 1];
    uint8_t miso[32 + 1];

    uint8_t *p_mosi = mosi;

    *p_mosi++ = command;

    for (uint8_t i = 0; i < len; i++) {
        *p_mosi++ = *buf++;
    }

    bcm2835_spi_transfernb((char*) mosi, (char*) miso, len + 1);
}

void SpiInterface::read_command(uint8_t command, uint8_t* buf, uint8_t len) {
    uint8_t mosi[32 + 1];
    uint8_t miso[32 + 1];

    uint8_t *p_mosi = mosi;
    uint8_t *p_miso = miso;

    *p_mosi = command;

    bcm2835_spi_transfernb((char*) mosi, (char*) miso, len + 1);

    p_miso++; // Skip status byte

    for (uint8_t i = 0; i < len; i++) {
        *buf++ = *p_miso++;
    }
}
