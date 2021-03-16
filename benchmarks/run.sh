#!/bin/bash

sudo cpupower frequency-set --governor performance > /dev/null
./benchmark_polynomial_add
sudo cpupower frequency-set --governor powersave > /dev/null
