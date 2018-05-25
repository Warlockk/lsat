/****************************************/
/* This is the checkpasstime module for */
/* the LSAT program. It cehcks to see   */
/* when passwords are set to expire for */
/* user accounts on the system          */
/****************************************/

#include "lsatheader.h"
#include <stdio.h>
#include <stdlib.h>

int checkpasstime(filename, distribution, verbose, html)
const char *const filename;
int distribution;
int verbose;
int html;
{

    /* I am currently not checking solaris or Mac OS X	*/
    const char * tempfile =NULL;
    const char * shellcode=NULL;
    const char * header   =NULL;
    int chage_installed   = 0;

    if (verbose >= 0)
    {
        printf(" Running checkpasstime module...\n");
    }


    if (verbose > 0)
    {
    printf(" Checking for users password expiration dates...\n");
    }



    /* it was noted that some distros do not install lsof by    */
    /* default, mainly gentoo... we therefore check here.       */
    if ((system("test -f /usr/bin/chage")) != 0 ) 
    {
        /* perhaps chage is not installed */
        header = "chage is not installed on this system,\nor it is not in the path,\ncheckpasstime was not run.\n";
        chage_installed = 1;
    }
    else
    {
        tempfile = "/tmp/lsat1.lsat";
        header = "List of users on the system with UID over 1000\nand the associated password expiration information\n";
	    shellcode = "for x in `cat /etc/passwd |awk -F \":\" '{ if ( $3 > 999 && $3 < 65000) print $1 }'`; do echo \"User:\" $x; chage -l $x; done >/tmp/lsat1.lsat";
    }

    if (chage_installed == 1)
    {
    if ((dostuff(0, filename, 0, header, html)) < 0)
        {
                /* something went wrong */
                perror(" Creation of list failed.");
                return(-1);
        }
    }

    if (chage_installed == 0)
    {
        if ((dostuff(tempfile, filename, shellcode, header, html)) < 0)
        {
            /* rhut-rho...something bad happened */
            perror(" Creation of list failed.");
            return(-1);
        }
    }
    
    if (verbose > 0)
    {
        printf(" Finished in checkpasstime module.\n");
    }
   
    return(0);
}
