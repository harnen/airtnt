#!/bin/sh

# update, upgrade, install dependencies
sudo apt update
sudo apt -y upgrade
sudo apt -y autoremove
sudo apt -y install build-essential g++ python-dev autotools-dev libicu-dev build-essential libbz2-dev libboost-all-dev

# install lib ssl
sudo apt -y install libssl-dev

# install lib-boost
wget https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.tar.gz
tar xvzf boost_1_67_0.tar.gz
cd boost_1_67_0/
./bootstrap.sh
./b2
sudo ./b2 install
cd

# install airtnt
git clone https://github.com/harnen/airtnt.git
cd airtnt/server
make

# run server with steps=100, max_iterations=1000
LD_LIBRARY_PATH=./sample_libcrypto ./server 8000 100 1000
