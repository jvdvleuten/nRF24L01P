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

#ifndef SpiInterface_H
#define	SpiInterface_H

#include <stdint.h>

class SpiInterface {
public:
    SpiInterface(uint8_t ce_pin, uint8_t csn_pin);
    
    void init(void);
    
    void write_register(uint8_t register_address, uint8_t* buf, uint8_t len);
    void read_register(uint8_t register_address, uint8_t* buf, uint8_t len);
    
    uint8_t write_command(uint8_t command);
    void write_command(uint8_t command, uint8_t* buf, uint8_t len);
    void read_command(uint8_t command, uint8_t* buf, uint8_t len);
    
    void set_ce_pin(bool state);
private:    
    uint8_t ce_pin_;
    uint8_t csn_pin_; 
};

#endif	/* SpiInterface_H */

