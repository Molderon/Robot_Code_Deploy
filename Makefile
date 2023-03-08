defaut: RCD_server.o RCD_network.o
	g++ RCD_server.o RCD_network.o RCD_server.h -std=c++20 -lpthread -lczmq -o Server
	make clean

clean:
	rm -r *.o 


RCD_server.o: RCD_server.cpp
	g++ -c -o RCD_server.o RCD_server.cpp -O2  -lpthread -lczmq

RCD_network.o: RCD_network.cpp
	g++ -c -o RCD_network.o RCD_network.cpp -O2  -lpthread -lczmq

run:
	./Server
	make clean
	