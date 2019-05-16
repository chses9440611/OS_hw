""" Test Client"""
from socket import socket, AF_INET, SOCK_STREAM
import time

s = socket(AF_INET, SOCK_STREAM)
s.connect(('localhost', 20000))
for _ in range(5):
    s.send(b'Hello')
    time.sleep(1)
