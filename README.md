# nRF24L01p-data-link

nRF24L01p-data-link driver is a high-speed, optimized and reliable Raspberry Pi and Arduino driver for the Nordic nRF24L01+ Ultra low power 2.4GHz RF Transceiver.

Driver features and goals:
  - High speed: up to 215 KB/s transfer speeds between 2 Raspberry PIs and 200 KB/s on the Arduino
  - Reliable: driver is made according to the Nordic specification. This ensures correct timing between transceivers.
  - Durable: Over one hour worth of packets sent (over 1 million) without a single dropped packet.
  - Clear written API for easy use in your projects
  - Tested on the Raspberry Pi 2 Model B and Arduino Uno/Nano
  - Clear examples so you can dive right in
 
## Installation and connecting
### Pin layout of the nRF24L01+

The pin layout of the nRF24L01+ when you have the GND on the top right.

> You can identify the GND pin as the 'white boxed' pin on the chip-side of the nRF24L01+ board.

| nRF24L01+     | pin layout    |
| ------------- | ------------- | 
| VCC           | GND (boxed)   |  
| CSN           | CE            | 
| MOSI          | SCK           | 
| IRQ           | MISO          | 

### Downloading and installing on the Raspberry Pi 2
First install the [bcm2835] library 
```sh
$ wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.42.tar.gz
$ tar zxvf http://www.airspayce.com/mikem/bcm2835/bcm2835-1.42.tar.gz
$ cd bcm2835-1.42
$ ./configure
$ make
$ sudo make check
$ sudo make install
```

Enable Device Tree support for the bcm2835 library to work
```sh
$ sudo raspi-config
 under Advanced Options - enable Device Tree
 Reboot.
```

Install the nRF24L01p-data-link library

```sh
$ git clone http://github.com/jvdvleuten/nRF24L01p-data-link.git
$ cd nRF24L01p-data-link
$ make
$ sudo make install
```

### Connecting your nRF24L01+ to your Raspberry Pi 2 Model B
Please refer http://pi.gadgetoid.com/pinout for the pin layout on your Raspberry Pi 2.

Connect the following pins of your nRF24L01+ to your Raspberry Pi 2:

| nRF24L01+     | RPi2          | nRF24L01+     | RPi2             |
| ------------- |---------------| ------------- |------------------| 
| VCC           | 3v3  (any 3v3)| GND (boxed)   | GND     (any GND)|  
| CSN           | CE0  (pin 24) | CE            | GPIO 22 (pin 15) | 
| MOSI          | MOSI (pin 19) | SCK           | SCLK    (pin 23) | 
| IRQ           | -             | MISO          | MISO    (pin 21) | 

Do ***not*** connect the VCC to the 5v output of the Raspberry Pi. This will destroy your transceiver.

### Downloading and installing on the Arduino IDE

1. [Download](https://github.com/jvdvleuten/nRF24L01p-data-link/archive/master.zip) this repository to your computer.
2. Copy all files from the repository in your `Arduino/Libraries` folder.
3. Copy all files from `Arduino/libraries/nRF24L01p-data-link/arch/arduino` to `Arduino/libraries/nRF24L01p-data-link/`.
4. (Re)start the Arduino IDE
5. Go to *Files -> Examples -> nRF24L01p-data-link* in the top menu to view and run the examples 

### Connecting your nRF24L01+ to your Arduino Uno/Nano
Connect the following pins of your nRF24L01+ to your Arduino Nano/Uno:

| nRF24L01+     | Arduino       | nRF24L01+     | Arduino          |
| ------------- |---------------| ------------- |------------------| 
| VCC           | 3v3  (any 3v3)| GND (boxed)   | GND     (any GND)|  
| CSN           | D10           | CE            | D9               | 
| MOSI          | D11           | SCK           | D13              | 
| IRQ           | -             | MISO          | D12              | 

### Improve stability with a 10 uF Capacitor

To improve stability **a lot**, connect a 10 uF capacitor between the VCC and the GND of your nRF24L01+ transceiver. Connect the +(plus)side of the capacitor to the VCC and the -(minus)side to the GND. Without the capacitor, our transceivers were not even working!

> Note:  Be careful not to short-wire the 3v3 and GND pins with a capacitor while your Raspberry or Arduino is turned on.

You can even solder it directly on the nRF24L01+ transceiver if you want to.

### Changing the CE and CSN pin on your board
You can choose which CE and CSN pin are in use in the constructor:
```c++
// NRF24L01p nRF24L01p(CE, CSN);
NRF24L01p nRF24L01p(22, 0); // Default RPi2 as in the wiring example above.
NRF24L01p nRF24L01p(9, 10); // Default Arduino as in the wiring example above.
```

## Code Example
### Basic send on the Raspberry Pi 2+
```c++
NRF24L01p nRF24L01p(22, 0);
nRF24L01p.init();

// Transmit
nRF24L01p.set_prim_tx();

while(1) {
	unsigned long data = rand();
	bool result = nRF24L01p.transmit(&data, sizeof(unsigned long));

	if (result) {
		printf("Package sent: %lu\n", data);
	} else {
		printf("Failed sending package.\n");
	}
}
```

### Basic receive on the Arduino
```c++
void setup() {
	Serial.begin(115200);

	NRF24L01p nRF24L01p(9, 10);
	nRF24L01p.init();
	
	// Receive
	nRF24L01p.set_prim_rx();
}

void loop() {
	if (nRF24L01p.rx_data_ready()) {
		nRF24L01p.read_rx_payload(&data, sizeof(unsigned long));

		Serial.print(F"Received package: ");
		Serial.println(data);
	}
}
```

### Compiling your projects
Use the **-lnRF24L01p** and the **-lbcm2835** flag to proper link the libraries to your project, e.g.:
```sh
$ g++ *other flags* -lnRF24L01p -lbcm2835 -o *output* *source*
```
### Other examples
Further examples can be found in the `/examples/` dir:
* *hello_word*: Basic sending and receiving of a package
* *high_speed*: Transfer without ACK (fastest)
* *high_speed_ack*: Transfer with ACK (fastest and reliable)
* *retry*: Slowest speed transfer with indefinite retries

To compile and run examples:
```sh
$ cd examples
$ make
$ sudo ./high_speed (or other example)
```

## Version
0.1.1 - Ported to Arduino

### Todo
- Create more and clear options for the API (channel, output, CRC encoding)
- Create more examples

## Dependencies

nRF24L01p-data-link uses the following open source project to work properly on the Raspberry Pi 2:
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

