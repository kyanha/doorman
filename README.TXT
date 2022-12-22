
KNOCK.EXE, Version 0.9


Bruce Ward
Saint John, NB, Canada April 18, 2006


This is the 'Readme' file for 'knock.exe'.   Knock is the client half of
of a client-server pair, the other half being 'doormand', the doorman
daemon.   The doorman is used to help secure Unix/GNU-Linux servers
on the wild and wooly Internet by allowing them to run with all TCP
ports closed by a firewall.

Knock is used to persuade the doorman to insert a firewall rule which
will allow you to make a TCP connection for a particular service. The
rule will be kept for a configured time period, and automatically
removed if no TCP connection made.  If one is made, the connection
may be kept indefinitely; when finally broken, the firewall rule is
removed. 

knock will typically be used like this:

C:\whatver> knock -r"putty myname@%H%"  x.myserver.org  22
C:\whatver> 


---Installing----------------------------------------------------------

To install knock.exe, simply drop it in any directory which occurs in
your PATH variable, then drop knock.cfg in your "profile" directory.

This directory will be:
C:\Documents and Settings\<username>\  under WinXP & Win2K, etc.
C:\WINNT\Profiles\<username>\          under WinNT4
C:\                                    under Win95 & 98

Read 'knock.txt' and 'knockcfg.txt'; then edit knock.cfg to match the
way the server's doorman is configured.  You should now be ready to go.


--- Running-new feature ----------------------------------------------



---If you are building for windows------------------------------------

You must first download the 'doorman-X.X.X.tgz' package.
Only the 'knock' client may be built for windows.
Knock.c can be compiled with Microsoft Visual C++; double-click
on 'WINNOCK.DSW', and build as normal.


---Please report bugs--------------------------------------------------

It should be noted that this is 'beta' software; if you find bugs,
(which is altogether likely) please, PLEASE report them to:
<bward2@users.sourceforge.net>

This includes anything in the documentation which is unclear, dumb,
or just plain wrong, and things which don't work the way you think
they should.


