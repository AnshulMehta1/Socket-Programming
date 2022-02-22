/*
Name: Anshul Mehta
Enrolment Number: AU1940275
*/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_PORT 9000
#define MAX_PENDING 5
#define MAX_LINE 256

int main(){
    FILE *fp;
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    char OK[100] = "OK\n";
    char FnF[100] = "File not found\n";
    char strHello[100] = "Hello\n";
    char final[MAX_LINE] = "";

    int buf_len, addr_len, l, file_size;
    int sockfd, newsockfd;
    /* build address data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);
    /* setup passive open */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror("simplex-talk: socket");
        exit(1);
    }
    if ((bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))) < 0){
        perror("simplex-talk: bind");
        exit(1);
    }
    listen(sockfd, MAX_PENDING);

    if ((newsockfd = accept(sockfd, (struct sockaddr *)&sin, &addr_len)) < 0){
        perror("simplex-talk: accept");
        exit(1);
    }
    /*
    Sending the Initial Hello
    */
    send(newsockfd, strHello, sizeof(strHello),0);
    while (1){
        while (buf_len = recv(newsockfd, buf, sizeof(buf), 0)){
            FILE *fp;
            fp = fopen(buf, "r");
            char c;
            c = fgetc(fp);
            int i;
            i = 0;
            /*
            Reading the File till EOF
            */
            while (c != EOF){
                printf("%c",c);
                final[i] = c;
                c = fgetc(fp);
                i ++;
            }
            printf("%s",final);
            int len;
            len = strlen(final) + 1;
            int sent_status;
            // send status
            if (sent_status = send(newsockfd, final, sizeof(final), 0)){
                printf("File sent \n");
            }
        }
    }
    // Closing the Sockets
    close(sockfd);
    close(newsockfd);

    return 0;
}
