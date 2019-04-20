#!/usr/bin/env python3
"""This is the hw3 of OS in spring 2019"""
import random
import string
import hashlib
from socketserver import TCPServer
from threading import Thread

def server_work(message, pow_diff=3):
    """genetrate POW string"""
    pow_check = pow_diff * '0'
    seed = id_generator()
    hash_value = create_hashvalue(seed+message)
    while hash_value[:pow_diff] > pow_check:
        seed = id_generator()
        hash_value = create_hashvalue(seed+message)

    print(hash_value)
    print(hash_value[:3])
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
    hash_obj.update(message.encode("utf-8"))
    return hash_obj.hexdigest()

if __name__ == '__main__':
    print(server_work('70NVO'))



