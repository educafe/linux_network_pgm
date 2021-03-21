#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE	256
#define CHUNK_SIZE	50

int main(int argc, char* argv[]){
	int serv_sock;
	struct sockaddr_in serv_addr, from_addr;
	socklen_t addr_size;
	char message[BUF_SIZE];
	int str_len=0, recv_len=0, idx=0;
	char t_rxbuf[CHUNK_SIZE];
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	if((serv_sock=socket(PF_INET, SOCK_DGRAM, 0)) == -1){
		perror("socket() error!!");
		exit(1);
	}
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )	{
		perror("bind() error!!");
		exit(1);
	}

	addr_size = sizeof(from_addr);
	memset(message, 0, sizeof(message));
	while(recv_len=recvfrom(serv_sock, &message[idx], CHUNK_SIZE, 0, 
										(struct sockaddr*)&from_addr, &addr_size))
	{
		if(recv_len==-1){
			perror("recvfrom() error!!");
			exit(1);
		} else {
			memset(t_rxbuf, 0, sizeof(t_rxbuf));
			strncpy(t_rxbuf, &message[idx], recv_len);			
			printf("recv ok(%d:%s)!!\n", idx, t_rxbuf);
			idx += recv_len;
		}
		str_len += recv_len;
		if(str_len > (BUF_SIZE - CHUNK_SIZE))
			break;
	}
	printf("Message from %s: %s\n", inet_ntoa(from_addr.sin_addr), message);  
	close(serv_sock);
	return 0;
}
