CC=g++
CFLAGS=-Wall -fPIC -Ofast -march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -std=c++0x
LDFLAGS=-lrt -lnRF24L01P -lbcm2835
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=)

all: $(OBJECTS)
    
%: %.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
	
clean:
	rm -rf $(OBJECTS)