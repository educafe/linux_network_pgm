/*
*	The function inet_aton() is converting from a dots-and-numbers string into a in_addr_t
*	(which is the type of the field s_addr in your struct in_addr)
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	// char *addr="127.212.124.78";
	char *addr=argv[1];
	struct sockaddr_in addr_inet;
	
	if(argc < 2){
		fprintf(stderr, "Usage: %s <ip address>\n", argv[0]);
		exit(1);
	}
	
	if(!inet_aton(addr, &addr_inet.sin_addr))
	{
		perror("inet_aton() error!!");
		exit(1);
	}
	else 
		printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);
	
	printf("inet_ntoa converstion : %s \n", inet_ntoa(addr_inet.sin_addr));
	
		
	return 0;
}
