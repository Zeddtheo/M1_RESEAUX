#!/bin/bash
ip link set tun0 up
p addr add fc00:1234:ffff::1/64 dev tun0