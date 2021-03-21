#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>

/*
	This example is to demonstrate how to detect lost of the socket connection 
	by using signal SIGPIPE which will be sent to the application when send() is failed 
	when socket is closed by the client
*/

int serv_sock;
int clnt_sock;

void handler(int signo){
	int ret;
	int status;
	
	printf("Signal Handler starts to close socket!!!\n");
	close(clnt_sock);	
	close(serv_sock);	
	printf("Signal Handler ends -----\n");
	
}

int main(int argc, char *argv[])
{
	// int serv_sock;
	// int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char message[]="Welcome to Linux Network Programming!";

	int yes = 1;
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	signal(SIGPIPE, handler);
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1) 
	{
		perror("socket() error!!");
		exit(1);
	}

#if 1
	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt() error!!");
		exit(1);
	}
#endif
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
	{
		perror("bind() error!!");
		exit(1);
	}
	
	if(listen(serv_sock, 5)==-1)
	{
		perror("listen() error!!");
		exit(1);
	}
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	printf("Connected to the client port --> %d\n", htons(clnt_addr.sin_port));
	if(clnt_sock==-1)
	{
		perror("accept() error!!");
		exit(1);
	}
	
	send(clnt_sock, message, sizeof(message), 0);
	sleep(1);
	send(clnt_sock, message, sizeof(message), 0);
	sleep(1);
	send(clnt_sock, message, sizeof(message), 0);
	//write(clnt_sock, message, sizeof(message));
	
#if 1
	printf("Enter to quit...");
	getchar();
#endif
	
	return 0;
}
