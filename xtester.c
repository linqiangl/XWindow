/* XTester - Chris Covington, June 2000
   used to demonstrate XTest extension ability to send keystrokes
   ./xtester remote_windowid remote_displayname 
   compile with gcc xtester.c -o xtester -lXtst -lX11 */
   /*
   xtester compile with below µ“Î√¸¡Ó»Áœ¬:

°°°°gcc xtester.c-o xtester -lXtst -lX11 -L/usr/X11R6/lib
   */

   
   
#include <stdio.h>   
#include <string.h>   
#include <X11/Xlib.h>   
#include <X11/Xutil.h>   
#include <X11/cursorfont.h>  
#include <X11/keysym.h>  
#include <X11/Xos.h>  
#include <X11/Xfuncs.h>  
#include <X11/Xfuncproto.h>  
#include <X11/X.h>  
   
main(argc,argv)
  int argc;
  char **argv;
{
  char *display = NULL;

  Display *dpy1 = NULL;
  Display *dpy2 = NULL;
  Window  win1;
  Window  win2;

  XWMHints hints={(InputHint|StateHint), True, NormalState,
    0, 0, 0, 0, 0, 0};

  if (argc != 3) {
    printf("usage: %s remote_windowid remote_displayname\n",argv[0]);
    printf("example: %s 0x500000e 10.99.99.99:0\n",argv[0]);
    exit(1);
  }

  /* read input */
  sscanf(argv[1], "0x%lx", &win1); /* 1st arg is remote windowid */
  display=argv[2]; /* 2nd arg is remote display name */

  /* open displays */
  if ((dpy1 = XOpenDisplay(NULL)) == NULL) { /* Open local display */
    fprintf(stderr, "Failed: open display %s\n", XDisplayName(NULL));
    exit(1);
  }
  if ((dpy2 = XOpenDisplay(display)) == NULL) { /* Open remote display */
    fprintf(stderr, "Failed: open display %s\n", XDisplayName(display));
    exit(1);
  }

  /* create local window */
  win1 = XCreateSimpleWindow(dpy1, DefaultRootWindow(dpy1),
    0, 0, 100, 100, 1, WhitePixel(dpy1, DefaultScreen(dpy1)),
    BlackPixel(dpy1, DefaultScreen(dpy1)));

  /* set window manager properties */
  XSetStandardProperties(dpy1, win1, "xtester", "xtester", None, NULL, 0, NULL);

  XSetWMHints(dpy1, win1, &hints);

  XSelectInput(dpy1, win1, FocusChangeMask|KeyPressMask|KeyReleaseMask);

  XMapWindow(dpy1, win1); /* make windows visible */

  while (1) { /* main x loop */
    XEvent event;
    XKeyEvent * e; 
    e=(XKeyEvent *)&event;
	
    XNextEvent(dpy1, &event); /* get an event */

    /* forward key events */
    if (event.type == KeyPress){
      XTestFakeKeyEvent (dpy2, e->keycode, 1, 0);
    }
    if (event.type == KeyRelease){
      XTestFakeKeyEvent (dpy2, e->keycode, 0, 0);
      XSetInputFocus(dpy2, win2, 1, CurrentTime);
      XSetInputFocus(dpy1, win1, 1, CurrentTime);
      XFlush(dpy2);
    }
  }
}
