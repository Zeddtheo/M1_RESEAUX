#!/bin/bash
# configure-route.sh
zero="0"
ip route add $1 via $2$zero dev $3