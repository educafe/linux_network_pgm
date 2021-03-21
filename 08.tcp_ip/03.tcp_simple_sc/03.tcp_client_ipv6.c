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
	struct sockaddr_in6 serv_addr, my_addr;
	char message[500];
	int str_len;
	socklen_t len;
	struct hostent *server;
	
	if(argc!=2){			//should be changed to 2 when using fixed ipv6 address
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET6, SOCK_STREAM, 0);
	if(sock == -1)
	{
		perror("socket() error!!");
		exit(1);
	}

#if 1	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin6_family=AF_INET6;
	if(inet_pton(AF_INET6, "::1", &serv_addr.sin6_addr) !=1){
		fprintf(stderr, "inet_pton() error");
		exit(1);
	}else{
		serv_addr.sin6_port=htons(atoi(argv[1]));
	}
	
#else	
	//Sockets Layer Call: gethostbyname2()
    server = gethostbyname2(argv[1],AF_INET6);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
	
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_flowinfo = 0;
    serv_addr.sin6_family = AF_INET6;
    memmove((char *) &serv_addr.sin6_addr.s6_addr, (char *) server->h_addr, server->h_length);
    serv_addr.sin6_port = htons(atoi(argv[2]));
#endif			
		
	if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1) 
	{
		perror("connect() error!!");
		exit(1);
	}
	
#if 0						//this is for testing of getpeername when necessary
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
