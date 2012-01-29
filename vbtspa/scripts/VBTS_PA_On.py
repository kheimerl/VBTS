#!/usr/bin/python

import xmlrpclib

PORT = 8080
SERVER = "localhost"

s = xmlrpclib.ServerProxy('http://' + SERVER + ":" + str(PORT))

s.on()
