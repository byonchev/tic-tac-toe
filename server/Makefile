all: server

server: main.o common.o client_process.o server_process.o
	gcc main.o common.o client_process.o server_process.o -pthread -o server

main.o: main.c
	gcc -c main.c

common.o: common.c
	gcc -c common.c

client_process.o: client_process.c
	gcc -c client_process.c

server_process.o: server_process.c
	gcc -c server_process.c

clean:
	rm *.o server
