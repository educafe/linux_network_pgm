#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]){
	int sock;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1){
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1){
		perror("connect() error!!");
		exit(1);
	}
	else puts("Connected...........");
	
	while(1){
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		str_len=send(sock, message, strlen(message), 0);
		
		recv_len=0;
		while(recv_len<str_len){
			recv_cnt=recv(sock, &message[recv_len], BUF_SIZE-1, 0);
			if(recv_cnt==-1){
				perror("recv() error!!");
				exit(1);
			}
			recv_len+=recv_cnt;
		}
		
		message[recv_len]=0;
		printf("Message from server: %s", message);
	}
	
	close(sock);
	return 0;
}
