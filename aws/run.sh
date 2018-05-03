#!/bin/sh

# get latest version from git
git pull

# run server with steps=100, max_iterations=1000
cd ~/airtnt/server
make
LD_LIBRARY_PATH=./sample_libcrypto ./server 8000 100 1000