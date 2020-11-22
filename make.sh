#!/bin/sh

gcc -Wall -std=c99 -O4 -I. main.c harmonic.c    -o h -lm -static

gcc -Wall -std=c99 -O4 -I. main.c double_wall.c -o d -lm -static
