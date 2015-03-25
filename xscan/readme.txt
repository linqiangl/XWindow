
XSCAN for unix. 
Tested under SunOS 4.1.4 and Linux.

What does this do?

	This proggie scans hosts or subnets looking for unsecured X clients.
If it finds one it starts logging all keystrokes to that session. Very simple.

Usage:

	xscan <host> | <subnet> [[<host>] [<subnet>] ...]

Examples:

	Scan a single host:

		xscan wsx-nc1-14.ix.netcom.com

	Scan multiple hosts:

		xscan netcom6.netcom.com www.escape.com

	Scan a subnet:

		xscan 129.23.7

	Mixture:

		xscan 129.237.4 netcom6.netcom.com www.escape.com 128.45.4
		

contact:

pendleto@math.ukans.edu

THIS PROGRAM IS NOT DISTRIBUTED FOR EDUCATIONAL PURPOSES ONLY.
