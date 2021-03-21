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
	
	if((sock=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
#if 0
#else
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1) {
		perror("connect() error!!");
		exit(1);
	}
#endif

	while(1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);     
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))	
			break;
#if 1
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		message[str_len]=0;
		printf("Message from server %s: %s", inet_ntoa(from_adr.sin_addr), message);
		
		printf("tx1-done---\n");
		
		memset(&serv_adr, 0, sizeof(serv_adr));
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr("192.168.219.103");
		serv_adr.sin_port=htons(atoi("1234"));
		
		sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
		adr_sz=sizeof(from_adr);
		str_len=recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
#else
		send(sock, message, strlen(message), 0);
		str_len=recv(sock, message, BUF_SIZE, 0);
#endif

		message[str_len]=0;
		printf("Message from server %s: %s", inet_ntoa(from_adr.sin_addr), message);
		printf("tx2-done---\n");
	}
	close(sock);
	return 0;
}
