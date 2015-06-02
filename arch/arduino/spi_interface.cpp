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

#include "spi_interface.h"

#include <stdint.h>
#include <SPI.h>

#include "spi_commands.h"
#include "register_map_table.h"

SpiInterface::SpiInterface(uint8_t ce_pin, uint8_t csn_pin) {
    ce_pin_ = ce_pin;
    csn_pin_ = csn_pin;
}

void SpiInterface::init(void) {
    
    SPI.begin();
    
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV2);

    pinMode(csn_pin_, OUTPUT);
    digitalWrite(csn_pin_, 1);

    pinMode(ce_pin_, OUTPUT);
    set_ce_pin(0);    
}

void SpiInterface::set_ce_pin(bool state) {
    digitalWrite(ce_pin_, state);
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

    //SPI.beginTransaction(spi_settings);
    digitalWrite(csn_pin_, 0);
    
    uint8_t result = SPI.transfer(command);
    
    digitalWrite(csn_pin_, 1);
    //SPI.endTransaction();
    
    return result;
}

void SpiInterface::write_command(uint8_t command, uint8_t* buf, uint8_t len) {
    
    digitalWrite(csn_pin_, 0);
    
    SPI.transfer(command);
    
    while(len--)
    {
        SPI.transfer(*buf++);
    }
    
    digitalWrite(csn_pin_, 1);    
}

void SpiInterface::read_command(uint8_t command, uint8_t* buf, uint8_t len) {
    digitalWrite(csn_pin_, 0);
    
    SPI.transfer(command);
    
    while(len--)
    {
        *buf++ = SPI.transfer(0);
    }
    
    digitalWrite(csn_pin_, 1); 
}
