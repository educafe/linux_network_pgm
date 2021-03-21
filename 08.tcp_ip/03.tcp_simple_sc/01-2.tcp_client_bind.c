#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

/*
	This example is to demonstrate that the client can also bind its port#.
	Otherwise, the port # will be allocated by the kernel within the port_range
	(sysctl -a | grep port_range)
*/

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr, my_addr;
	char message[500];
	int str_len;
	int ret;
	
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
	
	
	memset(&my_addr, 0, sizeof(struct sockaddr_in));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi("8888"));
	
	ret = bind(sock, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
	if(ret == -1){
		perror("bind()");
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
	
	str_len=recv(sock, message, sizeof(message), 0);
	//str_len=read(sock, message, sizeof(message));
	if(str_len==-1)
	{
		perror("recv() error!!");
		exit(1);
	}
	
	printf("Message from server: %s \n", message);  
	close(sock);
	return 0;
}
