
#
# simple test program
#
# $Id$
#

import ex1
import async
import socket

def cb(err,res):
    print "err=", err, "& res=", res

#
# make call a function, so that way we can test the refcounting on 
# args to arpc.call calls.  if we called from main, then we would
# never see f being dealloced (since main will always have a reference
# to it)
#
def call(cli):
    f = ex1.foo_t ()
    f.x = 'this is a test string'
    f.xx = 1010
    cli.call (ex1.FOO_FUNC, f, cb)


sock = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
sock.connect (('127.0.0.1', 3000))
fd = sock.fileno ()

print "file descriptor is", fd

x = async.arpc.axprt_stream (fd)
cli = async.arpc.aclnt (x, ex1.foo_prog_1 ())

call (cli);


async.core.amain ()
