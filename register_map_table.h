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

#ifndef REGISTER_MAP_TABLE_H
#define	REGISTER_MAP_TABLE_H

#define CONFIG              0x00
#define CONFIG_MASK_RX_DR   6
#define CONFIG_MASK_TX_DS   5
#define CONFIG_MASK_MAX_RT  4
#define CONFIG_EN_CRC       3
#define CONFIG_CRCO         2
#define CONFIG_PWR_UP       1
#define CONFIG_PRIM_RX      0

#define EN_AA               0x01

#define EN_RXADDR           0x02
#define EN_RXADDR_ERX_P5    5
#define EN_RXADDR_ERX_P4    4
#define EN_RXADDR_ERX_P3    3
#define EN_RXADDR_ERX_P2    2
#define EN_RXADDR_ERX_P1    1
#define EN_RXADDR_ERX_P0    0

#define SETUP_AW            0x03

#define SETUP_RETR          0x04
#define SETUP_RETR_ARD      4
#define SETUP_RETR_ARC      0

#define RF_CH               0x05

#define RF_SETUP            0x06
#define RF_SETUP_CONT_WAVE  7
#define RF_SETUP_RF_DR_LOW  5
#define RF_SETUP_PLL_LOCK   4
#define RF_SETUP_RF_DR_HIGH 3
#define RF_SETUP_RF_PWR     1

#define STATUS              0x07
#define STATUS_RX_DR        6
#define STATUS_TX_DS        5
#define STATUS_MAX_RT       4
#define STATUS_RX_P_NO      1
#define STATUS_TX_FULL      0

#define OBSERVE_TX          0x08
#define CD                  0x09

#define RX_ADDR_P0          0x0A
#define RX_ADDR_P1          0x0B
#define RX_ADDR_P2          0x0C
#define RX_ADDR_P3          0x0D
#define RX_ADDR_P4          0x0E
#define RX_ADDR_P5          0x0F
#define TX_ADDR             0x10

#define RX_PW_P0            0x11
#define RX_PW_P1            0x12
#define RX_PW_P2            0x13
#define RX_PW_P3            0x14
#define RX_PW_P4            0x15
#define RX_PW_P5            0x16

#define FIFO_STATUS         0x17
#define FIFO_STATUS_TX_REUSE 6
#define FIFO_STATUS_TX_FULL  5
#define FIFO_STATUS_TX_EMPTY 4
#define FIFO_STATUS_RX_FULL  1
#define FIFO_STATUS_RX_EMPTY 0

#define DYNPD               0x1C

#define FEATURE             0x1D
#define FEATURE_EN_DPL	    2
#define FEATURE_EN_ACK_PAY  1
#define FEATURE_EN_DYN_ACK  0

#endif	/* REGISTER_MAP_TABLE_H */

