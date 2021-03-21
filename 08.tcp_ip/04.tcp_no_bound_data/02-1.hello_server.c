#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	char message[1024];						//should be big enough to place the entire tcp_rx_buf
	int str_len=0, recv_len=0, idx=0;
	char t_rxbuf[100];						//should be enough to place the receive() size
	int yes = 1;
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
	{
		perror("socket() error!!");
		exit(1);
	}

	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		perror("setsockopt() error!!");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 )
	{
		perror("bind() error!!");
		exit(1);
	}
	
	if(listen(serv_sock, 5)==-1)
	{
		perror("listen() error!!");
		exit(1);
	}
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
	{
		perror("accept() error!!");
		exit(1);
	}

#if 0
	str_len=recv(clnt_sock, message, sizeof(message), 0);
#else
	printf("Press Enter to receive !!!!\n");
	getchar();
	memset(message, 0, sizeof(message));
	while( recv_len=recv(clnt_sock, &message[idx], 50, 0) )
	{
		printf("Receiving index %d(%d)\n", idx, recv_len);
		if(recv_len==-1)
		{
			perror("recv() error!!");
			exit(1);
		} else {
			memset(t_rxbuf, 0, sizeof(t_rxbuf));
			strncpy(t_rxbuf, &message[idx], recv_len);			
			printf("recv ok(%d:%s)!!\n", idx, t_rxbuf);
			idx += recv_len;
		}
		str_len += recv_len;
	}
#endif
	
	printf("Message from %s: %s\n", inet_ntoa(clnt_addr.sin_addr), message);  
	close(clnt_sock);
	close(serv_sock);
	return 0;
}
