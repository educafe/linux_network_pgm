#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
	int serv_sd, clnt_sd, read_cnt;;
	FILE * fp;
	char buf[BUF_SIZE];
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	fp=fopen("file_server.c", "rb"); 
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	
	while(1){
		read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
		if(read_cnt<BUF_SIZE){
			write(clnt_sd, buf, read_cnt);
			break;
		}
		write(clnt_sd, buf, BUF_SIZE);
	}
	fclose(fp);
	
#ifdef HALF
	printf("Enter to close half socket...");
	getchar();
	shutdown(clnt_sd, SHUT_WR);	
	while( read_cnt=read(clnt_sd, buf, BUF_SIZE) ) {
		if(read_cnt < 0) break;
		printf("Message from client(read_cnt:%d): %s \n", read_cnt, buf);
	}
#endif
	
#if 1
	printf("Enter to close socket...");
	getchar();
	close(clnt_sd); 
	printf("Enter to quit...");
	getchar();
#endif
	close(serv_sd);
	return 0;
}




