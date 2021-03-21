/*
*	The function inet_pton() is converting from a dots-and-numbers string into
*	struct sockaddr_in
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	// char *addr="127.212.124.78";
	char *addr=argv[1];
	struct sockaddr_in addr_inet;
	char str[INET_ADDRSTRLEN];
	
	if(argc < 2){
		fprintf(stderr, "Usage: %s <ip address>\n", argv[0]);
		exit(1);
	}
	
	if(!inet_pton(AF_INET, addr, &(addr_inet.sin_addr)))
	{
		perror("inet_pton() error!!");
		exit(1);
	}
	else 
		printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
	
	printf("inet_ntop converstion : %s \n", 
			inet_ntop(AF_INET, &(addr_inet.sin_addr), str, INET_ADDRSTRLEN));
	
	printf("%s\n", str);
	
	
		
	return 0;
}
