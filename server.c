/* CSD 304 Computer Networks, Fall 2016
   Lab 2, server
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
#include <unistd.h>
#include <netdb.h>
#include <semaphore.h>


#define SERVER_PORT 9000
#define BUF_SIZE 65500


int main(int argc, char * argv[]){
  struct sockaddr_in sin;
  struct sockaddr_storage client_addr;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
  socklen_t client_addr_len;
  char buf[BUF_SIZE];
  int len;
  int s;
  char *host;
  struct hostent *hp;

  /*Structs for ARQ and  SWS */
  /* This is the Header for the Sliding Window Protocol*/
  typedef u_char SwpSeqNo;
  typedef struct{
      SwpSeqNo SeqNum;
      SwpSeqNo AckNum;
      u_char Flags;
  } SwpHdr;

  /* Implementing the Sending of Frames via a Semaphore */
  /*Sem>0, send a Frame and Sem<0 Block the Sending of Frames*/
  /*Norm -> SemWait on each send and SemSignal on each ACK Indicating free space in the Frames */

  




  /* Declarations for file(s) to be sent 
     ...
  */

				
  
  /* For inserting delays, use nanosleep()
     struct timespec ... */ 
	 struct timespec time={0,100000000L};
 

  /* To get filename from commandline */
  /* if (argc==...) {} */
   
   
  /* Create a socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("server: socket");
    exit(1);
  }
 
 
  /* build address data structure and bind to all local addresses*/
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
 
  /* If socket IP address specified, bind to it. */
  if(argc == 2) {
    host = argv[1];
    hp = gethostbyname(host);
    if (!hp) {
      fprintf(stderr, "server: unknown host %s\n", host);
      exit(1);
    }
    memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  }
  /* Else bind to 0.0.0.0 */
  else
    sin.sin_addr.s_addr = INADDR_ANY;
  
  sin.sin_port = htons(SERVER_PORT);
  
  if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("server: bind");
    exit(1);
  }
  else{
    /* Add code to parse IPv6 addresses */
    inet_ntop(AF_INET, &(sin.sin_addr), clientIP, INET_ADDRSTRLEN);
    printf("Server is listening at address %s:%d\n", clientIP, SERVER_PORT);
  }
  
  printf("Client needs to send \"GET\" to receive the file %s\n", argv[1]);
    
  
  client_addr_len = sizeof(client_addr);
  
  /* Receive messages from clients*/
  /*Server Listens for req in an Infinite Loop*/
  while(1){
	  	FILE *fp;
				
	  while(len = recvfrom(s, buf, sizeof(buf), 0,(struct sockaddr *)&client_addr, &client_addr_len)){
				  inet_ntop(client_addr.ss_family,
					&(((struct sockaddr_in *)&client_addr)->sin_addr),
					clientIP, INET_ADDRSTRLEN);
				
				/*Forking a New Process everytime there is a Connection*/
        if(fork()==0){
        
          printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);
				  fp=fopen("jsr.mp4","r");
          /*Negating the strcmp because it gives 0 on match*/
          if(!strcmp(buf,"GET\n")){

					  memset(buf,0,sizeof(buf));
            /*Test if the fp is pointing to EOF*/
					  while(!feof(fp)){
              /* Reading by default packets of 65.5 Kb */
              fread(buf,sizeof(buf),1,fp);
              if((len=sendto(s,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,client_addr_len))==-1){
                perror("server:sendto");
                exit(1);
              }
              printf("Data Block Sent\n");
              /*Nanosleep after each Packet to avoid Packer Loss*/
              nanosleep(&time,NULL);
              memset(buf,0,sizeof(buf));
					  }
          /*Termination Condition*/
					memset(buf,0,sizeof(buf));
					strcpy(buf,"BYE");
					sendto(s,buf,sizeof(buf),0,(struct sockaddr *)&client_addr,client_addr_len);
					printf("Terminated the Connection");
				  }
			  close(fp);

        }
        
        // Change the time for data rate  as  payload cannot be hampered with

				

			}



  }
  
}


