all:
	rm serv.exe
	rm client.exe

serv:
	gcc reader.c parser.c alloc.c fileStruct.c server.c -lws2_32 -o serv
	./serv

client:
	gcc client.c -lws2_32 -o client
	./client

.PHONY: all