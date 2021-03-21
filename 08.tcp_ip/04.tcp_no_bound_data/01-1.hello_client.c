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
	int len;

	char message[]="Hello World! Hi~~~ Hahahaha^^ Hohoho...";
	char message1[]="Message boundary is the separation between two messages being sent over a protocol.";
	char message2[]="UDP preserves message boundaries, but TCP does not";

	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1) 
	{
		perror("socket() error!!");
		exit(1);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
	{
		perror("connect() error!!");
		exit(1);
	}
	
	len = send(sock, message, strlen(message), 0);
	printf("result1 = %d\n", len);
	len = send(sock, message1, strlen(message1), 0);
	printf("result2 = %d\n", len);
	len = send(sock, message2, strlen(message2), 0);
	printf("result3 = %d\n", len);

#if 1
	printf("Enter to quit...");
	getchar();
#endif
	close(sock);
	return 0;
}
