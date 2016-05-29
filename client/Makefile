all: client

client: main.o sockets.o helpers.o server_socket.o client_socket.o
	gcc main.o sockets.o helpers.o server_socket.o client_socket.o -o client

main.o: main.c
	gcc -c main.c

server_socket.o: server_socket.c
	gcc -c server_socket.c

client_socket.o: client_socket.c
	gcc -c client_socket.c

helpers.o: ../common/helpers.c
	gcc -c ../common/helpers.c

sockets.o: ../common/sockets.c
	gcc -c ../common/sockets.c

clean:
	rm *.o client
