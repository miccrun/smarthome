CC = gcc
CFLAGS =  -std=gnu99 -I. -lbcm2835
DEPS = 
OBJ = Adafruit_DHT.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Adafruit_DHT: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
