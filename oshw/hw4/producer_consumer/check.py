#!/usr/bin/env python3

import subprocess
import sys

from py_essentials import hashing as hs


def checksum(target_id):
    src_file = './input/%d' % target_id
    output_file = './output/%d' % target_id

    hash1 = hs.fileChecksum(src_file, 'sha256')
    hash2 = hs.fileChecksum(output_file, 'sha256')

    if hash1 != hash2:
        return False
    else:
        return True

for i in range(1, 6):


    ret = subprocess.call('./client %d' % i, shell=True)
    # check return code
    if ret != 0:
        print('Client failed\n')
        sys.exit(1)

    if checksum(i) == False:
        print('Checksum is not equal with %dth test data\n' % i)
        sys.exit(1)
    else:
        print('Pass checksum %d' % i)


