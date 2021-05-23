#!/bin/bash

sudo nmap -sn 192.168.0.0/24 | grep -i "Raspberry" -A 3 -B 3 > findrpi.txt
sleep 1
cat findrpi.txt


