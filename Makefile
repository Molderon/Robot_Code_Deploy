defaut: RCD_main.o RCD_net.o
	g++ RCD_main.o RCD_net.o RCD_header.h -lczmq -o Client
	make clean

clean:
	rm -r *.o


RCD_main.o: RCD_main.cpp
	g++ -c -o RCD_main.o RCD_main.cpp -O2 -Wall -lpthread -lczmq

RCD_net.o: RCD_net.cpp
	g++ -c -o RCD_net.o RCD_net.cpp -O2 -Wall -lpthread -lczmq

run:
	./Client