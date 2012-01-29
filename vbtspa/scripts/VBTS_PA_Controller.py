#!/usr/bin/python

from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import getopt
import sys

PORT = 8080
SERVER = "localhost"

SERIAL_LOC = "/dev/ttyACM0"
SERIAL_BR = 9600

ON_CMD = "O0=1"
OFF_CMF = "O0=0"

fake = False

opts, args = getopt.getopt(sys.argv[1:],
                           "fh", ["fake", "help"])

def usage():
    print ("The VBTS Power Amplifier Controller")
    print ("-f | --fake Fake the serial line")
    print ("-h | --help Display this help message")
    exit(2)

for o,a in opts:
    if o in ("-f", "--fake"):
        fake = True
    else:
        usage()

import serial

class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

server = None
serial_con = None

try:
    server = SimpleXMLRPCServer((SERVER, PORT),
                                requestHandler=RequestHandler,
                                allow_none=True)
    server.register_introspection_functions()
except:
    print ("Server failed to open")
    exit(1)

if (not fake):
    try:
        serial_con = serial.Serial(SERIAL_LOC, SERIAL_BR)
    except:
        print ("Serial port failed to open")
        exit(1)

#update functions
def on():
    serial_con.write(ON_CMD)

def fake_on():
    print ("Turning on")

def off():
    serial_con.write(OFF_CMD)

def fake_off():
    print ("Turning off")

#register functions
if (not fake):
    server.register_function(on, 'on')
    server.register_function(off, 'off')
else:
    server.register_function(fake_on, 'on')
    server.register_function(fake_off, 'off')

server.serve_forever()
