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
#define MAX_LINE 256

int main(int argc, char *argv[]){
    FILE *fp;
    int valread;
    struct hostent *hp;
    struct sockaddr_in sin;
    char *host;
    char buf[MAX_LINE];
    char bufout[MAX_LINE];
    int sockfd;
    int len, buf_len, bufout_len;
    if (argc == 2){
        host = argv[1];
    }
    else{
        fprintf(stderr, "usage: simplex-talk host\n");
        exit(1);
    }
    /* translate host name into peer’s IP addbufouts */
    hp = gethostbyname(host);
    if (!hp){
        fprintf(stderr, "simplex-talk: unknown host: %s\n", host);
        exit(1);
    }
    /* build addbufouts data structure */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    bcopy(hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons(SERVER_PORT);
    /* active open */
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(sockfd, (struct sockaddr *)&sin, sizeof(sin)) < 0){
        perror("simplex-talk: connect");
        close(sockfd);
        exit(1);
    }

    /*
    Reading the Hello after the Connection is Establsihed
    */
    bzero(buf,255);
    valread = read(sockfd, buf, sizeof(buf));
    printf("%s\n",buf);
    // bzero(buf,255);
    /* main loop: get and send file names to the server */
    while (1){
        printf("Enter file name to be requested\n");
        scanf("%s", buf);
        len = strlen(buf) + 1;
        /*
        Escape Condition
        */
        if ((strncmp(buf, "Bye", 3)) == 0) {
			printf("Bye\n");
			break;
		}
        // Status for request
        int status;
        if (status = send(sockfd, buf, sizeof(buf), 0)){
            printf("%s File Requested\n",buf);
        }
        // Status for received
        int status1;
        if (status1 = recv(sockfd, bufout, sizeof(bufout), 0))
        {
            printf("%s File received\n",bufout);

        }
        /*
        Appending a received string to differentiate between original and received file
        */
        char append[]="received";
        
        strcat(append,buf);
        fp = fopen(append, "w");
        fputs(bufout, fp);
        fclose(fp);
    }

    close(sockfd);

    return 0;
}
