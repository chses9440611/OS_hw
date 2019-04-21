#!/usr/bin/env python3
"""This is the hw3 of OS in spring 2019"""
import random
import string
import hashlib
from threading import Thread
from socketserver import BaseRequestHandler, TCPServer

def server_work(message, pow_diff=3):
    """genetrate POW string"""
    pow_check = pow_diff * '0' # decide the # of 0
    seed = id_generator() # create the seed
    hash_value = create_hashvalue(seed+message) # create the hash value
    while hash_value[:pow_diff] > pow_check: # while create failure, redo
        seed = id_generator()
        hash_value = create_hashvalue(seed+message)

    return message + ',' + seed + ',' + hash_value + '\n'

def id_generator(size=5, chars=string.ascii_letters + string.digits):
    """Generating seed"""
    value = ''
    for _ in range(size):
        value += random.choice(chars)

    return value

def create_hashvalue(message):
    """Create hash value"""
    hash_obj = hashlib.sha256()
    hash_obj.update(message.encode("utf-8")) # change message from string to bytes
    return hash_obj.hexdigest()


class EchoHandler(BaseRequestHandler):
    """Process request"""
    def handle(self):
        """ The function to recv and send result"""
        print('Got connection from: ', self.client_address)
        while True:
            msg = self.request.recv(8192)
            if not msg:
                break
            msg = (msg.decode()).replace('\n', '') #change message from bytes to string and remove '\n'
            #print("Message: ", msg)
            msg2 = server_work(msg)
            self.request.send(msg2.encode()) # change msg2 from string to bytes

if __name__ == '__main__':
    try:
        NUMTHREADING = 100
        SERV = TCPServer(('', 20000), EchoHandler)
        for n in range(NUMTHREADING):
            tSocket = Thread(target=SERV.serve_forever)
            tSocket.daemon = True
            tSocket.start()
        SERV.serve_forever()
    except KeyboardInterrupt:
            print("ByeBye!")
            SERV.server_close()

