#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr, my_addr;
	char message[500];
	int str_len;
	socklen_t len;
	
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
	// serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if(inet_aton(argv[1], &serv_addr.sin_addr) !=1){
		fprintf(stderr, "inet_aton() error");
		exit(1);
	}else{
		serv_addr.sin_port=htons(atoi(argv[2]));
	}
		
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
	{
		perror("connect() error!!");
		exit(1);
	}

#if 0			//It is to see the peer information in terms of name and port #
	len = sizeof(serv_addr);
	getpeername(sock, (struct sockaddr*)&serv_addr, &len);
	printf("Peer IP address: %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("Peer port      : %d\n", ntohs(serv_addr.sin_port));
#endif
	
	str_len=recv(sock, message, sizeof(message), 0);
	//str_len=read(sock, message, sizeof(message));
	if(str_len==-1)
	{
		perror("recv() error!!");
		exit(1);
	}
	printf("Message from server: %s (%d)\n", message, str_len); 


#if 0
	printf("Press Enter to close the socket : ");
	getchar();
#endif
	close(sock);
	return 0;
}
