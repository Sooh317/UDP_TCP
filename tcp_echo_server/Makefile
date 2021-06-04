tcpechoclient: tcpechoclient.c
	gcc -Wall tcpechoclient.c -o tcpechoclient

tcpechoserver1: tcpechoserver1.c
	gcc -Wall -pthread tcpechoserver1.c -o tcpechoserver1

tcpechoserver2: tcpechoserver2.c
	gcc -Wall tcpechoserver2.c -o tcpechoserver2

.PHONY: clean
clean:
	rm -f tcpechoclient tcpechoserver1 tcpechoserver2
