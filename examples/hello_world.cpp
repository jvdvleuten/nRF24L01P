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


#include <cstdlib>
#include <iostream>
#include <nrf24l01p.h>

using namespace std;

/*
 * Send "Hello world"
 */
int main(int argc, char** argv) {

    
    NRF24L01p nRF24L01p(22, 0);

    nRF24L01p.init();

    uint8_t address1[5];
    address1[0] = 0xE7;
    address1[1] = 0xE7;
    address1[2] = 0xE7;
    address1[3] = 0xE7;
    address1[4] = 0xE7;

    uint8_t address2[5];
    address2[0] = 0xC2;
    address2[1] = 0xC2;
    address2[2] = 0xC2;
    address2[3] = 0xC2;
    address2[4] = 0xC2;

    int choice;

    cout << "Use 0 to receive packets and use 1 to send packets: ";
    cin >> choice;

    if (choice) {
        nRF24L01p.set_tx_addr(address2, 5);
        nRF24L01p.set_rx_addr_p0(address2, 5);
        nRF24L01p.set_rx_addr_p1(address1, 5);

        char data[32] = "Hello world";

        // Prepare transceiver for TX mode
        nRF24L01p.set_prim_tx();

        while (1) {
            bool result = nRF24L01p.transmit(&data, 32);

            if (result) {
                printf("Package sent: %s\n", data);
            } else {
                printf("Failed sending package.\n");
            }

            sleep(1);
        }
    } else {

        nRF24L01p.set_tx_addr(address1, 5);
        nRF24L01p.set_rx_addr_p0(address1, 5);
        nRF24L01p.set_rx_addr_p1(address2, 5);

        // Listen for incoming packets
        nRF24L01p.set_prim_rx();

        // Size of "Hello world" + null character
        char data[32];

        while (1) {

            if (nRF24L01p.rx_data_ready()) {
                nRF24L01p.read_rx_payload(&data, 32);

                printf("Received package: %s\n", data);
            }
        }
    }

    return 0;
}

