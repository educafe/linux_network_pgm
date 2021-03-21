#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char message[BUF_SIZE];
	int str_len, i;
	int yes=1;
	
	struct sockaddr_in serv_adr;
	struct sockaddr_in clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(serv_sock==-1)
	{
		perror("socket() error!!");
		exit(1);
	}
	
#ifdef REUSE
	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt() error!!");
		exit(1);
	}
#endif

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	{
		perror("bind() error!!");
		exit(1);
	}
	
	if(listen(serv_sock, 5)==-1)
	{
		perror("listen() error!!");
		exit(1);
	}
	
	clnt_adr_sz=sizeof(clnt_adr);

	for(i=0; i<5; i++)
	{
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		if(clnt_sock==-1)
		{
			perror("accept() error!!");
			exit(1);
		}
		else printf("Connected client %d \n", i+1);
	
		while((str_len=recv(clnt_sock, message, BUF_SIZE, 0))!=0){
			printf("Received from client: %s\n", message);
			send(clnt_sock, message, str_len, 0);
		}
		close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

