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

#ifndef SPI_COMMANDS_H
#define	SPI_COMMANDS_H

#define R_REGISTER          0b00000000
#define W_REGISTER          0b00100000

#define R_RX_PAYLOAD        0b01100001
#define W_TX_PAYLOAD        0b10100000

#define FLUSH_TX            0b11100001
#define FLUSH_RX            0b11100010

#define REUSE_TX_PL         0b11100011

#define R_RX_PL_WID         0b01100000
#define W_ACK_PAYLOAD       0b10101000

#define W_TX_PAYLOAD_NOACK  0b10110000

#define NOP                 0b11111111

#endif	/* SPI_COMMANDS_H */

