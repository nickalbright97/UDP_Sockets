#include <stdio.h>
#include <arpa/inet.h>

#include "message.h"

char     *myName = "Mohamed Aboutabl" ;

/*--------------------------------------------------------------------
   Print a Calculator message buffer
----------------------------------------------------------------------*/
void printMsg( FILE *log , calcMsg *m )
{
    fprintf( log , "{ Name='%s' Type=%d , %3d %c %3d , result=%6d }" ,
       m->fullName , ntohl( m->msgType) ,     
       ntohl( m->num1 ) , m->operation , ntohl( m->num2 ) , ntohl(m->result )
    ) ;
}

/*--------------------------------------------------------------------
   Print a student's Client message
----------------------------------------------------------------------*/
void printStudent( FILE *log , student *m )
{
    fprintf( log , "{ Student=%20s , IP = %20s , Port = %hu }" ,
             m->fullName , m->ipStr , ntohs(m->port) ) ;
}
