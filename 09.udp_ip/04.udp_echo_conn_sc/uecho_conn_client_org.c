#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int sock, str_len;
	char message[BUF_SIZE];
	socklen_t adr_sz;
	struct sockaddr_in serv_adr, from_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	if(sock=socket(PF_INET, SOCK_DGRAM, 0) ==-1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) {
		perror("connect() error!!");
		exit(1);
	}
	
	while(1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);     
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;
		
		send(sock, message, strlen(message), 0);
		str_len=recv(sock, message, BUF_SIZE, 0);
		message[str_len]=0;
		printf("Message from server: %s", message);
	}	
	close(sock);
	return 0;
}
