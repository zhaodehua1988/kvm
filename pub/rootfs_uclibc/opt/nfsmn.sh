#!/bin/sh
#./hello
mount -t nfs -o nolock -o tcp -o rsize=32768,wsize=32768 192.168.15.8:/workroom/Hi3798M  /mnt
