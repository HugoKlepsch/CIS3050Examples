#!/usr/bin/env python2

import socket
import signal
import getpass
import sys
import os

#
# similar to unix_demo_client.c, but written in Python
#
# This program creates a socket and then begins an infinite loop.  Each time
# through the loop it accepts a connection and prints out messages from it.
# When the connection breaks, or a termination message comes through, the
# program accepts a new connection.
#
# To generate the messages for this server, use the code in unix_demo_client.c


# this variable is to be shared with the signal handler */
sSocketPath = None

# remove the socket and exit
def signalHandler(sig, code):
    print >> sys.stderr, "Cleaning up on signal"
    os.unlink(sSocketPath)
    sys.exit(1)


# get us a name to use for the socket.  While there is mkstemp()
# for Python, there is no interface that won't actually create
# the file, and we want a socket
sSocketPath = "/tmp/myUDS.%s.%d" % (getpass.getuser(), os.getpid())

# register the signal handler for HUP, INTR, and TERM
signal.signal(signal.SIGHUP, signalHandler) # signal 1 */
signal.signal(signal.SIGINT, signalHandler) # signal 2 */
signal.signal(signal.SIGTERM, signalHandler) # signal 15 */

# Create socket
try:
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
except OSError, msg:
    print >> sys.stderr, "Error opening stream socket:", msg
    sys.exit(1)

# bind the socket to the filesystem
try:
    sock.bind(sSocketPath)
except OSError, msg:
    print >> sys.stderr, "Error binding stream socket:", msg
    sys.exit(1)

print "Socket has name", sSocketPath


# Start accepting connections
sock.listen(5)

# loop until told to "quit"
keepGoing = True
while keepGoing:
    # we get a _second_ socket to read the message from
    try:
        msgsock, clientAddress = sock.accept()
    except OSError, msg:
        print >> sys.stderr, "Error from accept on stream socket:", msg
        sys.exit(1)

    message = True
    while message and keepGoing:

        # the 1024 here is simply the size of message section
        # we read in one go - much like read() we can read a
        # message in pieces, just like we read a file.  Here
        # we are implicitly expecting that none of the lines
        # send are more than 1024 bytes, as we are printing
        # them out one by one.
        try:
            message = msgsock.recv(1024)
        except OSError, msg:
            print >> sys.stderr, "Error reading stream message:", msg
            sys.exit(1)

        if message:
            sys.stdout.write("got -->%s" % message)

            if message == "quit\n":
                keepGoing = False
        else:
            print "Ending connection"


    # close the message socket and go back to waiting on the stream socket
    msgsock.close()


print "Told to quit, so cleaning up..."
sock.close()
os.unlink(sSocketPath)

sys.exit(0)

