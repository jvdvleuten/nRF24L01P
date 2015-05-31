# nRF24L01P-data-link

nRF24L01P-data-link driver is a high-speed, optimized and reliable Raspberry Pi driver for the Nordic nRF24L01+ Ultra low power 2.4GHz RF Transceiver.

Driver features and goals:
  - High speed: up to 203 KB/s transfer speeds!
  - Reliable: driver is made according to the Nordic specification. This ensures correct timing between transceivers.
  - Clear written API for easy use in your projects
  - Support for the Raspberry Pi 2 Model B
  - Clear examples so you can dive right in
 
## Installation
### Downloading and installing the nRF24L01P library
```sh
$ git clone http://github.com/jvdvleuten/nRF24L01P-data-link.git
$ cd nRF24L01P-data-link
$ make
$ sudo make install
```

### Connecting your nRF24L01+ to your Raspberry Pi 2 Model B
Please refer http://pi.gadgetoid.com/pinout for the pin layout on your Raspberry Pi 2.

Connect the following pins of your nRF24L01+:

| nRF24L01+     | RPi2          | nRF24L01+     | RPi2             |
| ------------- |---------------| ------------- |------------------| 
| VCC           | 3v3  (any 3v3)| GND (boxed)   | GND     (any GND)|  
| CSN           | CE0  (pin 24) | CE            | GPIO 22 (pin 15) | 
| MOSI          | MOSI (pin 19) | SCK           | SCLK    (pin 23)  | 
| IRQ           | -             | MISO          | MISO    (pin 21)  | 

You can choose which CE and CSN pin are in use in the constructor:
```c++
// NRF24L01P nRF24L01p(CE, CSN);
NRF24L01P nRF24L01p(22, 0); // Default as in wiring example above.
```

## Code Example
### Basic sending and receiving with default init
```c++
NRF24L01P nRF24L01p(22, 0);
nRF24L01p.init();

// Transmit
nRF24L01p.set_prim_tx();

unsigned long data = rand();

bool result = nRF24L01p.transmit(&data, sizeof(unsigned long));

if (result) {
    printf("Package sent: %lu\n", data);
} else {
    printf("Failed sending package.\n");
}

// Receive
nRF24L01p.set_prim_rx();

if (nRF24L01p.rx_data_ready()) {
    nRF24L01p.read_rx_payload(&data, sizeof(unsigned long));

    printf("Received package: %lu\n", data);
}
```

### Compiling your projects
Use the **-lnRF24L01P** and the **-lbcm2835** flag to proper link the libraries to your project, e.g.:
```sh
$ g++ -Wall -fPIC -Ofast -march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -lnRF24L01P -lbcm2835 -o output source
```
### Other examples
Further examples can be found in the `/examples/` dir:
* hello_word: Basic sending and receiving of a package
* high_speed: High speed transfer without ACK

To compile and run examples:
```sh
$ cd examples
$ make
$ sudo ./high_speed (or other example)
```

## Version
0.1.0 - initial release

## Dependencies

nRF24L01P-data-link uses the following open source projects to work properly:
* [bcm2835] - C library for Broadcom BCM 2835 as used in Raspberry Pi

## Contributors

Want to contribute? Great! Fork the library and make a pull-request.

License
----

Copyright (C) 2015  Johan van der Vleuten
 
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

[bcm2835]:http://www.airspayce.com/mikem/bcm2835/