#!/usr/bin/python

from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler
import getopt
import sys
import threading
import time
import logging
import signal

PORT = 8080
SERVER = "localhost"

SERIAL_LOC = "/dev/ttyACM0"
SERIAL_BR = 9600

ON_CMD = "O0=1\r"
OFF_CMD = "O0=0\r"

UPTIME = 60 * 5

LOG_LOC = "/var/log/vbts_pa_controller.log"
LOG_LEVEL = logging.DEBUG
LOG_FORMAT = "%(asctime)s - %(name)s - %(levelname)s - %(message)s"

#used variables
fake = False
#fake = True

PA_On = False
PA_Time = 0
PA_Lock = threading.Lock()

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

logging.basicConfig(filename=LOG_LOC, 
                    level=LOG_LEVEL, 
                    format=LOG_FORMAT)

if (not fake):
    import serial

logger = logging.getLogger("VBTS_PA_Controller")

class RequestHandler(SimpleXMLRPCRequestHandler):
    rpc_paths = ('/RPC2',)

server = None
serial_con = None

try:
    server = SimpleXMLRPCServer((SERVER, PORT),
                                requestHandler=RequestHandler,
                                allow_none=True)
    server.register_introspection_functions()
    logger.info("RPC Server Started w/ time " + str(UPTIME))
except:
    logger.warn("RPC Server failed to start")
    print ("Server failed to open")
    exit(1)

if (not fake):
    try:
        serial_con = serial.Serial(SERIAL_LOC)
        logger.info("Serial connection established")
    except:
        logger.warn("Serial port failed to open")
        print ("Serial port failed to open")
        exit(1)

#update functions
def on():
    logger.info("Turning PA on")
    print ("Turning PA on")
    serial_con.write(ON_CMD)
    serial_con.flush()

def fake_on():
    logger.info("Fake turning PA on")
    print ("Fake turning on")

def off():
    logger.info("Turning PA off")
    print ("Turning PA off")
    serial_con.write(OFF_CMD)
    serial_con.flush()

def fake_off():
    logger.info("Fake turning PA off")
    print ("Fake turning off")

def update_pa_on_reason(reason):
    print (reason)
    update_pa_on()

#eventually add locks to these
def update_pa_on():
    global PA_On
    global PA_Time
    global PA_Lock
    PA_Lock.acquire()
    PA_On = True
    PA_Time = time.time()
    if (fake):
        fake_on()
    else:
        on()
    PA_Lock.release()
    
def update_pa_off_reason(reason):
    print (reason)
    update_pa_off()

def update_pa_off():
    global PA_On
    global PA_Time
    global PA_Lock
    PA_Lock.acquire()
    PA_On = False
    if (fake):
        fake_off()
    else:
        off()
    PA_Lock.release()

class off_thread(threading.Thread):
    
    def __init__(self):
        threading.Thread.__init__(self)
        self.logger = logging.getLogger("VBTS_PA_Controller.off_thread")
        self.logger.info("Starting off thread")
        self.daemon = True

    def run(self):
        global PA_On
        global PA_Time
        global PA_Lock
        self.logger.info("Running off thread")
        while(True):
            #little bit ugly, but ducks race condition
            PA_Lock.acquire()
            if (PA_On and PA_Time + UPTIME < time.time()):
                self.logger.info("PA Timeout")
                PA_On = False
                if (fake):
                    fake_off()
                else:
                    off()
            PA_Lock.release()
            time.sleep(1)

#register functions
server.register_function(update_pa_on, 'on')
server.register_function(update_pa_on_reason, 'onWithReason')
server.register_function(update_pa_off, 'off')
server.register_function(update_pa_off_reason, 'offWithReason')

#lastly, catch exit function
def quit_signal():
    logger.info("Shutting down")
    serial_con.close()
    server.shutdown()

signal.signal(signal.SIGTERM, quit_signal)

t = off_thread()
t.start()

server.serve_forever()
