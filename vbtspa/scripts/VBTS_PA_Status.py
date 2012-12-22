#!/usr/bin/python

import xmlrpclib
import argparse

parser = argparse.ArgumentParser(description='Turn off a VBTS PA')
parser.add_argument("-p", "--port", dest="port", 
                    default="8080", help="The port of the server")
parser.add_argument("-d", "--dest", dest="dest", 
                    default="localhost", help="The IP or hostname of the server")

args = parser.parse_args()

s = xmlrpclib.ServerProxy('http://' + args.dest + ":" + args.port)

if (s.status()):
    print ("PA On")
else:
    print ("PA Off")
