CC=g++
CFLAGS=-c -Wall -fPIC -Ofast -march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp -std=c++0x
LDFLAGS=-shared -lrt -lbcm2835
SOURCES=$(wildcard *.cpp) $(wildcard util/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
LIB=nRF24L01P

all: $(OBJECTS) lib$(LIB).so
    
lib$(LIB).so: $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%: %.cpp
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf *.o $(LIB).*

install:
	install -m 0755 lib$(LIB).so /usr/local/lib/
	mkdir -p /usr/local/include/$(LIB)
	install -m 0644 *.h /usr/local/include/$(LIB)/
	ldconfig