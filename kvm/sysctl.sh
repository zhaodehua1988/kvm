#!/bin/sh
sysctl -w net.core.rmem_max=1204000
sysctl -w net.core.wmem_max=1024000
sysctl -w net.core.wmem_default=1024000
sysctl -w net.core.rmem_default=1024000
sysctl -w net.ipv4.udp_mem="1024000 1024000 1024000"
sysctl -w net.ipv4.udp_rmem_min=1024000
sysctl -w net.ipv4.udp_wmem_min=1024000






