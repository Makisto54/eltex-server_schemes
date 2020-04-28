#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) 
{
    int sock;
    int size;
    int new_sock;
    int i = 0;
    int bytes_read;
    char buf[1024];
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        fprintf(stderr, "Incorrect server socket\n");
        exit(1);
	}

    server.sin_family = AF_INET;
    server.sin_port = htons(7777);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    size = sizeof(struct sockaddr_in);
    if(bind(sock, (struct sockaddr *)&server, size) == -1)  
	{
	    fprintf(stderr, "Incorrect server bind\n");
        exit(1);	
	}      

    if(listen(sock, 5) == -1)
    {
    	fprintf(stderr, "Incorrect server listen\n");
        exit(1);
	}

    while(i < 1024)
	{
        printf("%d ", i);
        new_sock = accept(sock, NULL, 0);
        bytes_read = recv(new_sock, buf, 1024, 0);
        if(bytes_read == -1)
        {
        	fprintf(stderr, "Incorrect server recv\n");
            exit(1);
		}
        puts(buf);
        if(send(new_sock, buf, bytes_read, 0) == -1)
        {
        	fprintf(stderr, "Incorrect server send\n");
            exit(1);
		}
        close(new_sock);
        i++;
    }

    close(sock);
    return 0;
}
