#!/bin/bash

sudo make

sudo insmod procdriver.ko

dmesg --color=always | tail -n8

sudo cat /proc/procdriver

echo "1" | sudo tee /proc/procdriver

dmesg --color=always | tail -n8

sudo rmmod procdriver



