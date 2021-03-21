#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define OPSZ 4
int calculate(int opnum, int opnds[], char oprator);

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	char opinfo[BUF_SIZE];
	int result, opnd_cnt, i;
	int recv_cnt, recv_len;	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	int yes = 1;

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

#if 1
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
		opnd_cnt=0;
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);	
		printf("Connected to the client ip --> %s\n", inet_ntoa(clnt_adr.sin_addr));
		recv(clnt_sock, &opnd_cnt, 1, 0);
		printf("Received operand count : %d\n", opnd_cnt);
		recv_len=0;
		while((opnd_cnt*OPSZ+1)>recv_len)
		{
			recv_cnt=recv(clnt_sock, &opinfo[recv_len], BUF_SIZE, 0);
			recv_len+=recv_cnt;
		}
		result=calculate(opnd_cnt, (int*)opinfo, opinfo[recv_len-1]);
		send(clnt_sock, (char*)&result, sizeof(result), 0);
		printf("Send the result to the client : %d\n", result);
		close(clnt_sock);
	}
	close(serv_sock);
	return 0;
}

int calculate(int opnum, int opnds[], char op)
{
	int result=opnds[0], i;
	
	switch(op)
	{
	case '+':
		for(i=1; i<opnum; i++) result+=opnds[i];
		break;
	case '-':
		for(i=1; i<opnum; i++) result-=opnds[i];
		break;
	case '*':
		for(i=1; i<opnum; i++) result*=opnds[i];
		break;
	}
	return result;
}
