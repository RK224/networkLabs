ftp : client_broken_ftp.o server_broken_ftp.o 

client_broken_ftp.o : client_broken_ftp.c
	gcc -o client_broken_ftp.o client_broken_ftp.c
server_broken_ftp.o : server_broken_ftp.c
	gcc -o server_broken_ftp.o server_broken_ftp.c
