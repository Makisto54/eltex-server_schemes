#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

int number = 1;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void *client()
{
    int sock;
    int size;
    char buf[1024] = "MESSAGE";
    char buf2[1024];
    int bytes_read;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        fprintf(stderr, "Incorrect server\n");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(7777);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    size = sizeof(struct sockaddr_in);
    if(connect(sock, (struct sockaddr*) &server, sizeof(struct sockaddr_in)) == -1)
    {
        fprintf(stderr, "Incorrect client connect\n");
        exit(1);
    }

    bytes_read = sizeof(buf);
    if(send(sock, buf, bytes_read, 0) == -1)
    {
        fprintf(stderr, "Incorrect client send\n");
        exit(1);
    }

    if(recv(sock, buf2, 1024, 0) == -1)
    {
        fprintf(stderr, "Incorrect client recv\n");
        exit(1);
    }

    pthread_mutex_lock(&mut);
    printf("%d %s\n", number, buf2);
    number++;
    pthread_mutex_unlock(&mut);
    close(sock);
}

int main(int argc, char *argv[]) 
{
    pthread_t th[32];
    
    for(int i = 0; i < 32; i++)
	{
        pthread_create(&th[i], NULL, client, NULL);
        pthread_join(th[i], NULL);
    }

    return 0;
}
