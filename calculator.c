/*----------------------------------------------------------------------
PA-04: UDP Socket Programming
            Calculator program

Written By: Team-00
    1- Nick Albright
    2- Miguel Padilla

Submitted on: mm/dd/yyyy
----------------------------------------------------------------------*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "message.h"

#define MAXBUFLEN   256
#define IPSTRLEN    50
typedef struct sockaddr SA ;

/*------------------------------------------------------------------------
 * Error Handling Functions
 *----------------------------------------------------------------------*/
void err_sys(const char* msg) 
{ 
    fflush( stderr ) ;
    perror(msg); 
    exit(1); 
}

void  INThandler(int sig)
{
    // Add hostname to message here 
    signal(sig, SIG_IGN);
    printf("\n## I have been nicely asked to TERMINATE. goodbye\n");
    exit(0);
}

/*------------------------------------------------------------------------
 * main - Iterative UDP server for DAYTIME service
 *------------------------------------------------------------------------
 */

int main(int argc, char *argv[])
{
   struct sockaddr_in  
                 srvrSkt,        /* the address of this server   */
                 clntSkt;       /* remote client's socket       */
    unsigned short port = 13 ;  /* service name or port number  */
    char    buf[MAXBUFLEN];     /* "input" buffer; any size > 0 */
    int	    sd;               /* server socket                */
    time_t	now;                /* current time                 */
    char    timeStr[MAXBUFLEN]; /* time string   */
    char    ipStr[IPSTRLEN] ;  /* client's dotted-dec IP addr. */

    unsigned int    alen;       /* from-address length          */

	switch (argc) 
	{
      case 1:
        break;
      case 2:
        port = atoi( argv[1] );
        break;
      default:
        snprintf(buf, MAXBUFLEN , "usage: %s [port]\n" , argv[0] );
        err_sys( buf ) ;
    }

    // handle ctrl c
    signal(SIGINT, INThandler);

    sd = socket( AF_INET, SOCK_DGRAM  , 0 ) ;
    if ( sd < 0 )
        err_sys( "Could NOT create socket" ) ;

    // Prepare the server's socket address structure
    memset( (void *) &srvrSkt, 0 , sizeof(srvrSkt) );
    srvrSkt.sin_family      = AF_INET;
    srvrSkt.sin_addr.s_addr = htonl( INADDR_ANY );
    srvrSkt.sin_port        = htons( port ) ;

    // Now, bind the server to above socket
    if ( bind( sd, (SA *) &srvrSkt , sizeof(srvrSkt) ) < 0 )
    {
        snprintf( buf, MAXBUFLEN, "Could NOT bind to port %d", port );
        err_sys( buf ) ;
    }
    inet_ntop( AF_INET, (void *) & srvrSkt.sin_addr.s_addr , ipStr , IPSTRLEN ) ;
    printf( "Bound socket %d to IP %s Port %d\n" , sd , ipStr , ntohs( srvrSkt.sin_port ) );    

    while (1) 
    {
        alen = sizeof(clntSkt);
        fprintf(stderr , "Calculator server waiting\n" ) ;        
        if ( recvfrom( sd, buf, MAXBUFLEN , 0, (SA *) &clntSkt, &alen ) < 0 )
            err_sys( "recvfrom" ) ;

        fprintf(stderr , "Calculator server received '%s'\n" , buf ) ;

        inet_ntop( AF_INET, (void *) & clntSkt.sin_addr.s_addr , ipStr, IPSTRLEN ) ;
        printf("from IP %s Port %d\n" , ipStr , ntohs(clntSkt.sin_port) );

        time( &now ); /* get the current system's time */
	    ctime_r( &now , timeStr );    /* WARNING! ctime() is NOT thread-safe */
        fprintf(stderr , "Calculator server sending '%s'\n" , timeStr ) ;
		sendto( sd , (void *) timeStr , strlen(timeStr) , 0 , (SA *) &clntSkt, alen );
	}
}
