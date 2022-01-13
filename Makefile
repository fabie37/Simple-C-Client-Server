output: client.o server.o
	clang client.o -o client
	clang server.o -o server

client.o: client.c
	clang -W -Wall -Werror -c client.c 

server.o: server.c
	clang -W -Wall -Werror -c server.c

clean:
	rm client server *.o