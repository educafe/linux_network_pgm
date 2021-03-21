/*
*	The function inet_aton() is converting from a dots-and-numbers string into a in_addr_t
*	(which is the type of the field s_addr in your struct in_addr)
*	the function inet_addr() is an older function that does basically the same thing as
*	inet_aton()
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	// char *addr1="127.212.124.78";
	char *addr1=argv[1];
	char *addr2="127.212.124.257";
	unsigned char *pt;
	
	if(argc < 2){
		fprintf(stderr, "Usage: %s <ip addr>\n", argv[0]);
		exit(1);
	}

	printf("INADDR_ANY:%x, INADDR_NONE:%x\n", INADDR_ANY, INADDR_NONE);
	// unsigned long conv_addr=inet_addr(addr1);
	in_addr_t conv_addr=inet_addr(addr1);
	if(conv_addr==INADDR_NONE)
		printf("Error occured! \n");
	else{
		printf("\nBefore inet_addr -----------------\n");
		printf("%s\n", addr1);
		
		printf("\nAfter inet_addr ------------------\n");
		printf("Network ordered integer addr: %#x \n", conv_addr);
		pt = (char *)&conv_addr;
#if 1
		printf("Address 1 : %p -> %#x\n", pt++, (unsigned char)*((char *)&conv_addr));
		printf("Address 2 : %p -> %#x\n", pt++, (unsigned char)*(((char *)&conv_addr)+1));
		printf("Address 3 : %p -> %#x\n", pt++, (unsigned char)*(((char *)&conv_addr)+2));
		printf("Address 4 : %p -> %#x\n", pt, (unsigned char)*(((char *)&conv_addr)+3));
#else		
		printf("Address 1 : %p -> %#x\n", pt, *pt++);
		printf("Address 2 : %p -> %#x\n", pt, *pt++);
		printf("Address 3 : %p -> %#x\n", pt, *pt++);
		printf("Address 4 : %p -> %#x\n", pt, *pt++);
#endif
	}
		
	
	conv_addr=inet_addr(addr2);
	if(conv_addr==INADDR_NONE)
		printf("Error occureded \n");
	else 
		printf("Network ordered integer addr: %#x \n\n", conv_addr);
			
	return 0;
}
