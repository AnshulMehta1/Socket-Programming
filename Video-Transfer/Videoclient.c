/* CSD 304 Computer Networks, Fall 2016
   Lab 2, client
   Team: 5
   Anshul Mehta AU1940275
   Vishwa Rawal AU1940131
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 9000
#define BUF_SIZE 65500
/*Making the Buffer Size eqaute to PAYLOAD for Large Video Files*/


int main(int argc, char * argv[]){
  
  FILE *fp;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[BUF_SIZE];
  char recvbuf[BUF_SIZE];
  int s;
  int len;

  if ((argc==2)||(argc == 3)) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
    exit(1);
  }

  if(argc == 3) {
    fp = fopen(argv[2], "w");
    if (fp == NULL) {
      fprintf(stderr, "Error opening output file\n");
      exit(1);
    }
  }

  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "client: unknown host: %s\n", host);
    exit(1);
  }
  else
    printf("Host %s found!\n", argv[1]);

  /* build address data structure */
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);
  

  /* create socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("client: socket");
    exit(1);
  }
  
  printf("Client will get data from to %s:%d.\n", argv[1], SERVER_PORT);
  printf("To play the music, pipe the downlaod file to a player, e.g., ALSA, SOX, VLC: cat recvd_file.wav | vlc -\n"); 
  
  
  

  /* send message to server */  
  fgets(buf, sizeof(buf), stdin);
  buf[BUF_SIZE-1] = '\0';
  len = strlen(buf) + 1;
  /*Copying the message in the recvbuf to check in the While if we're sending GET only*/

  memcpy(recvbuf,buf,strlen(buf));
  if (sendto(s, buf, len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) { 
    perror("Client: sendto()"); 
    return 0; 
  }
    
  /* get reply, display it or store in a file*/ 
  /* Add code to receive unlimited data and either display the data
     or if specified by the user, store it in the specified file. 
     Instead of recv(), use recvfrom() call for receiving data */
    /*Clear the Buffer*/
	memset(buf,0,sizeof(buf));
  /*Negating to get 1 from strcmp 0 match*/
	if(!strcmp(recvbuf,"GET\n")){
		fp= fopen("nydhm.mp4","w");
    /* Addr len for the Server Address */
		len=sizeof(sin);
		while(recvfrom(s,buf,sizeof(buf),0,(struct sockaddr *)&sin,&len)){
      /*Termination Condition */

			if(!strcmp(buf,"BYE")){
				break;
			}

      /* Write the Data in the Buffer to the File  */
			fwrite(buf,sizeof(buf),1,fp);
			memset(buf,0,sizeof(buf));
		}
    /* Closing File and the Socket */
		close(fp);
		close(s);

	}
}
