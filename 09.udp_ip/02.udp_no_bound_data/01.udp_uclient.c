#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	socklen_t addr_size;

	char message[]="Hello World! Hi~~~ Hahahaha^^ Hohoho...";

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	if((sock=socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket() error!!");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	addr_size = sizeof(serv_addr);

	//tx message to server
	sendto(sock, message, strlen(message), 0, (struct sockaddr*)&serv_addr, addr_size);

#if 0
	printf("Enter to Quit....");
	getchar();
#endif
	close(sock);
	return 0;
}
