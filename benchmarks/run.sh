#!/bin/bash

sudo cpupower frequency-set --governor performance > /dev/null
./$1
sudo cpupower frequency-set --governor powersave > /dev/null
