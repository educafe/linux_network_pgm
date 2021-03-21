/*
*	The function inet_pton() is converting from a dots-and-numbers string into
*	struct sockaddr_in6.
*	in6.addr has a member unsigned char s6_addr[16], Refer man ipv6,
*	To print this, cast to (unsigned long) for %lu or %lx
*	2001:db8:8714:3a90::12
*/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	char *ip="2001:db8:8714:3a90::12";
	struct sockaddr_in6 addr_inet6;
	char addr[INET6_ADDRSTRLEN];
	unsigned char *addr6;
	
	if(!inet_pton(AF_INET6, ip, &(addr_inet6.sin6_addr)))
	{
		perror("inet_pton() error!!");
		exit(1);
	}
	else 
		printf("Network ordered integer addr: %#lx \n", 
					(unsigned long)addr_inet6.sin6_addr.s6_addr);
	
	printf("inet_ntop converstion : %s \n", 
			inet_ntop(AF_INET6, &(addr_inet6.sin6_addr), addr, INET6_ADDRSTRLEN));
	
	printf("%s\n", addr);
	
		
	return 0;
}
