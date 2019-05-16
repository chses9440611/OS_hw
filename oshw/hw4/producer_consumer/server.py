import socket
import subprocess
import struct
import time

Host = '127.0.0.1'
Port = 7777

def handle_request(conn):
    req = conn.recv(5)
    target = struct.unpack('<I', req)
    filename = './input/%d' % target
    print('Seding %s file' % filename)
    with open(filename, 'rb') as f:
        while True:
            chunk = f.read(4096)
            if chunk:
                conn.send(chunk)
            else:
                break
    conn.close()

if __name__ == '__main__':

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((Host, Port))
    sock.listen(5)

    print('Server started, waitng for connection')

    while True:
        conn, addr = sock.accept()
        handle_request(conn)

