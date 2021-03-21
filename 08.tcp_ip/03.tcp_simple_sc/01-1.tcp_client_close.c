#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
	This example requites to work with 02-1.tcp_server_close.c to see if what happens
	when server close the socket while waiting for recv().
	To test it, server keeps hiting the "Enter" until the socket is closed
*/

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr, my_addr;
	char message[500];
	int str_len;
	
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
		
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
	{
		perror("connect() error!!");
		exit(1);
	}
	for(;;){
		str_len=recv(sock, message, sizeof(message), 0);
	//str_len=read(sock, message, sizeof(message));
		if(str_len==-1)
		{
			perror("recv() error!!");
			exit(1);
		}
		if(str_len == 0){
			printf("socket is closed by the other end!!!\n");
			break;
		}
		printf("Message from server: %s \n", message); 
	}
	
#if 1
	printf("Press Enter to close the socket : ");
	getchar();
#endif
	close(sock);
	return 0;
}
