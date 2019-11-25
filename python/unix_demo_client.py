#!/usr/bin/env python2

import sys
import socket

#
# similar to unix_demo_client.c, but written in Python
#
# This program attaches to a UNIX domain socket given on the command line.
# Anything typed on standard input is then send to the server.
#

if len(sys.argv) != 2:
    print >> stderr, "Expecting the name of the socket on command line"
    sys.exit(1)


# Create socket
try:
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
except OSError, msg:
    print >> stderr, "Error opening stream socket:", msg
    sys.exit(1)


# Connect socket using name specified by command line.
try:
    sock.connect(sys.argv[1])
except OSError, msg:
    print >> stderr, "Error connecting stream socket:", msg
    sys.exit(1)



# loop until we are told to quit (or the input closes)
print "Now copying stdin into the socket.  Type \"quit\" to stop"

line = sys.stdin.readline()
while len(line) > 0:
    # write the data to the server
    try:
        sock.sendall(line)
    except OSError, msg:
        print >> stderr, "Error writing data to server:", msg
        sys.exit(1)

    # check if we are done
    if line == "quit\n":
        break

    line = sys.stdin.readline()

sock.close()

