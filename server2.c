#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

void* serv(void* p) 
{
    int* tmp = (int*)p;
    int sock = *tmp;
    char buf[1024];
    int bytes_read;

    bytes_read = recv(sock, buf, 1024, 0);
    if(bytes_read == -1)
    {
        fprintf(stderr, "Incorrect server recv\n");
        exit(1);
    }
    printf("%s\n", buf);
    if(send(sock, buf, bytes_read, 0) == -1)
    {
        fprintf(stderr, "Incorrect server send\n");
        exit(1);
    }
    close(sock);
}

int main(int argc, char *argv[]) 
{
    int sock;
	int size;
    int i = 0;
    int cash = 32;
    int* mas_sock = (int*)malloc(sizeof(int) * cash);
    pthread_t* id = (pthread_t*)malloc(sizeof(pthread_t) * cash);
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
    if(bind(sock, (struct sockaddr *) &server, size) == -1) 
	{
	    fprintf(stderr, "Incorrect server bind\n");
        exit(1);	
	}       

    if(listen(sock, 5) == -1)
    {
    	fprintf(stderr, "Incorrect server listen\n");
        exit(1);
	}

    while(1) 
	{
        if(i >= cash) 
		{
            cash += 32;
            id = (pthread_t *)realloc(id, sizeof(pthread_t) * cash);
            mas_sock = (int *)realloc(mas_sock, sizeof(int) * cash);
        }
        mas_sock[i] = accept(sock, NULL, 0);
        if(mas_sock[i] == -1)
        {
            fprintf(stderr, "Incorrect server accept\n");
            exit(1);
        }
        else
        {
            printf("Accepted %d\n", i);
        }
        pthread_create(&id[i], NULL, serv, (void *)&mas_sock[i]);
        i++;
    }

    for(int i = 0; i < 32; i++)
    {
        pthread_join(id[i], NULL);
    }

    close(sock);
    return 0;
}
