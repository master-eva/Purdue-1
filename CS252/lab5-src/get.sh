#!/bin/sh
# Usage: ./get.sh <server> <port> <path>
echo -e "GET $3 HTTP/1.0\r\n\r\n" | nc $1 $2

