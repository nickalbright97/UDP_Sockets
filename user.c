/*----------------------------------------------------------------------
PA-04: UDP Socket Programming
            User program

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
#include "message.h"

#define MAXBUFLEN   256
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

/*------------------------------------------------------------------------
 * main - UDP client for DAYTIME service that prints the resulting time
 *----------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    char	    *serverIP;
    unsigned short port;

    char str[80];

    char    timeStr[MAXBUFLEN];        /* time string   */ 
    int	    sd, n;                     /* socket descriptor, read count */

    if ( argc < 6 ) 
    {
      fprintf(stderr, "usage: %s  serverIP  port calculation\n" , argv[0] );
      exit(1);
    }

    // Set up expression string
    strcpy(str, argv[3]);
    strcat(str, " ");
    strcat(str, argv[4]);
    strcat(str, " ");
    strcat(str, argv[5]);

    puts(str);
     
    serverIP          = argv[1] ;
    port              = (unsigned short) atoi( argv[2] ) ;

    /* Allocate a socket */
	sd = socket( AF_INET, SOCK_DGRAM , 0 ) ;
	if (sd < 0)
		err_sys( "Could NOT create socket" ) ;
    
    // Prepare the server's socket address structure
    struct sockaddr_in srvSkt;	   /* Server's socket structrue  */
    memset( (void *) &srvSkt, 0 , sizeof(srvSkt) );
    srvSkt.sin_family   = AF_INET;
    srvSkt.sin_port     = htons( port ) ;
    if( inet_pton( AF_INET, serverIP, (void *) & srvSkt.sin_addr.s_addr ) != 1 )
    {
      err_sys( "Invalid server IP address" ) ;
    }

    fprintf(stderr , "User sending '%s'\n" , str ) ;
    /* must send the \0 at end of msg, too */
    sendto ( sd, (void *) str, strlen(str)+1 , 0 , (SA *) &srvSkt, sizeof(srvSkt)  );


    n = recvfrom ( sd, (void*) timeStr , MAXBUFLEN , 0 , NULL , NULL );
    if ( n <= 0 )
        err_sys( "Failed to get the result from the server" );

    timeStr[strlen(timeStr)-1] = 0 ; /* I hate the \n char placed by ctime_r() */
    printf( "User received '%s'\n"  , timeStr );
    close( sd ) ;
    exit(0);
}

