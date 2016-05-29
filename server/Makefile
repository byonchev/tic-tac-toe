all: server

server: main.o sockets.o helpers.o client_process.o server_process.o
	gcc main.o sockets.o helpers.o client_process.o server_process.o -pthread -o server

main.o: main.c
	gcc -c main.c

helpers.o: ../common/helpers.c
	gcc -c ../common/helpers.c

sockets.o: ../common/sockets.c
	gcc -c ../common/sockets.c

client_process.o: client_process.c
	gcc -c client_process.c

server_process.o: server_process.c
	gcc -c server_process.c

clean:
	rm *.o server
