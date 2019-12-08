/*----------------------------------------------------------------------
PA-04: UDP Socket Programming
            the student's Testing client application

Written By: Team-00
    1- Dr. Mohamed Aboutabl
    2- Dr. Mohamed Aboutabl

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

/* ---------------------------------------------------------------------
 * main - Student's UDP client for Testing service
 *----------------------------------------------------------------------*/

int main ( int argc , char * argv[] )
{

    int       nameLen = 1 + strlen( myName ) ;
    calcMsg   msg1 ;
    char	 *calcIP , *testerIP ;
    unsigned short calcPort , testPort ;
    struct   sockaddr_in srvSkt;	   /* Server's socket structure  */
    int	     sd, n;                     /* socket descriptor, read count */

    if( argc < 5 )
    {
        fprintf(stderr, "usage: %s  yourCalculatorIP  yourCalculatorPort   instructorIP  instructorPort\n" , argv[0] );
        exit(-1) ;
    }    

    /* Set up local and remote sockets */
    calcIP          = argv[1] ;
    int       ipLen = 1 + strlen( calcIP ) ;
    calcPort        = (unsigned short) atoi( argv[2] ) ;
    testerIP        = argv[3] ;
    testPort        = (unsigned short) atoi( argv[4] ) ;
    
	sd = socket( AF_INET, SOCK_DGRAM , 0 ) ;
	if ( sd < 0 )
		err_sys( "Testing client Could NOT create a local socket" ) ;
    
    // Prepare the Testing server's socket address structure
    memset( (void *) &srvSkt, 0 , sizeof(srvSkt) );
    srvSkt.sin_family   = AF_INET;
    srvSkt.sin_port     = htons( testPort ) ;
    if( inet_pton( AF_INET, testerIP, (void *) & srvSkt.sin_addr.s_addr ) != 1 )
      err_sys( "Invalid Testing server IP address" ) ;

    // Connect this UDP socket exclusively to the Testing server
    if( connect( sd , (SA *) &srvSkt , sizeof(srvSkt) ) < 0 )
    {
        err_sys( "Testing client failed connecting local UDP socket to Tester" ) ;    
    }

    /* prepare an information message to the Testing server */
    student  infoMsg ;
    memset( (void *) &infoMsg, 0 , sizeof(infoMsg) );
    snprintf( infoMsg.fullName, 50, "%s-Student" , myName ) ; 
    memcpy( infoMsg.ipStr      , calcIP , ipLen   )  ; 
    infoMsg.port      = htons( calcPort ) ;  
    
    /* Send the information message to the Testing server */
    int status ;
    status = send ( sd, (void *) &infoMsg, sizeof(infoMsg) , 0 );
    if ( status < 0 ) 
        err_sys("Testing client Failed to send message to the Testing Server\n");

    printf("\nTesting client sent this message to the Testing Server: " );
    printStudent( stdout , & infoMsg );  puts("");

    /* Now, wait for a Reply from the Tester server */
    printf ("\nTesting client is now waiting to receive confirmation ...\n" );

    char  msg2[100] ;
    memset( (void *) msg2, 0 , sizeof(msg2) );
    status = recv ( sd, (void *) msg2 , sizeof(msg2) , 0 );     
    if ( status < 0 ) 
        err_sys("Testing client Failed to receive Reply from the Testing server\n" );

    printf("Testing client received this from the Testing server: %s\n\n"  , msg2 );

    close ( sd ) ;
    return 0 ;
}
