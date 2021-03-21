#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
	int serv_sock;
	char message[BUF_SIZE];
	int str_len;
	socklen_t clnt_adr_sz;
	struct sockaddr_in serv_adr, clnt_adr;
	int ret;

	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if((serv_sock=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	// if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	ret = bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	if(ret == -1) {
		perror("bind() error!!");
		exit(1);
	}
	// memset(message, 0, BUF_SIZE);
	while(1) {
		clnt_adr_sz=sizeof(clnt_adr);
		str_len=recvfrom(serv_sock, message, BUF_SIZE, 0, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
		printf("Received from client (%s) : %s\n", inet_ntoa(clnt_adr.sin_addr), message);
		sendto(serv_sock, message, str_len, 0, 
				(struct sockaddr*)&clnt_adr, clnt_adr_sz);
		memset(message, 0, str_len);
	}	
	close(serv_sock);
	return 0;
}
