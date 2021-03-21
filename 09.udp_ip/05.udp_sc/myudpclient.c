#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


int main(int argc, char *argv[]) {
	int sockfd;
	struct sockaddr_in their_addr;
	int numbytes;
	char buf[80];

	if(argc != 2) 	{
		fprintf(stderr, "usage : %s <server ip>\n", argv[0]);
		exit(1);
	}

	//UDP socket open
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(10000);
	their_addr.sin_addr.s_addr = inet_addr(argv[1]); 
	memset(&(their_addr.sin_zero), '\0',8);

	do {
		printf("send string? ");
		fgets(buf, 80, stdin);
		// send data to server
		if((numbytes = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *)&their_addr, 
															sizeof(struct sockaddr))) == -1) {
			perror("sendto");
			exit(1);
		}

		printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
	} while(strncmp(buf, "quit", 4));
	close(sockfd);

	return 0;
}

