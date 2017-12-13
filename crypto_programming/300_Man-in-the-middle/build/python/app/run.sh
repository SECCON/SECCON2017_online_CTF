#!/bin/sh

socat TCP-LISTEN:8000,reuseaddr,fork EXEC:"/usr/src/app/code.py"

