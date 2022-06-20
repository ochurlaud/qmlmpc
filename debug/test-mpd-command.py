#!/usr/bin/env python3

import socket
import sys

host = sys.argv[1]
command = sys.argv[2]

print(command)
s = socket.socket()
s.connect((host, 6600))
s.send(f'{command} \n'.encode('utf8'))
s.recv(5000)
print(s.recv(5000).decode('utf8'))
s.close()

