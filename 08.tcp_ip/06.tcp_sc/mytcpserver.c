#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>   
#include <sys/wait.h>

int main(void){
	int sockfd, new_fd, numbytes;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;
	int yes = 1;
	char tx_buf[128], rx_buf[128];
	int i;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket() error");
		exit(1);
	}

	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
		perror("setsockopt() error");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(10000); //server port number setting
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(&(my_addr.sin_zero), '\0', 8);

	//server ip & port number setting
	if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
		perror("bind() error");
		exit(1);
	}

	//client backlog setting
	if(listen(sockfd, 5) == -1){
		perror("listen() error");
		exit(1);
	}

	while(1){
		sin_size = sizeof(struct sockaddr_in);

		//wait for client request
		if((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size)) == -1){
			perror("accept() error");
			continue;
		}

		printf("server : got connection from %s \n", inet_ntoa(their_addr.sin_addr));

		if(!fork()){
			close(sockfd);
			for(i=1; ; i++) {
				memset(tx_buf, 0, sizeof(tx_buf));
				memset(rx_buf, 0, sizeof(rx_buf));
				//wait for rx data from client	
				if((numbytes = recv(new_fd, rx_buf, sizeof(rx_buf), 0)) == -1){
					perror("recv");
					exit(1);
				}
				if(numbytes == 0) break;	
				printf("Server Rx(%d) : %s", getpid(), rx_buf);
				sprintf(tx_buf, "Hi_%d, client(from %d)~~\n", i, getpid());
				//send data to client
				if(send(new_fd, tx_buf, strlen(tx_buf)+1, 0) == -1) perror("send");	
			}
			printf("Server(%d): Client Connection Socket Closed!!\n", getpid());
			//close client socket connection		
			close(new_fd);
			exit(0);
		}
		// close(new_fd);

		waitpid(-1, NULL, WNOHANG);
	}
	return 0;
}

