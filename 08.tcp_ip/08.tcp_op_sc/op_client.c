#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE];
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
	{
		perror("socket() error!!");
		exit(1);
	}
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
	{
		perror("connect() error!!");
		exit(1);
	}
	else puts("Connected...........");

	printf("Operand count: ");
	scanf("%d", &opnd_cnt);
	opmsg[0]=(char)opnd_cnt;
	
	for(i=0; i<opnd_cnt; i++)
	{
		printf("Operand %d: ", i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]);
	}
	getchar();
	printf("Operator(+,-,*): ");
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]);
	send(sock, opmsg, opnd_cnt*OPSZ+2, 0);
	recv(sock, &result, RLT_SIZE, 0);
	
	printf("\n--> Operation result : %d \n", result);
	close(sock);
	return 0;
}
