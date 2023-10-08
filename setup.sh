#!/bin/bash


sudo apt update

sudo apt install cmake build-essential

echo "Install secp256k1"
git clone https://github.com/bitcoin-core/secp256k1.git
cd secp256k1/
mkdir build && cd build
cmake ..
make
# make check # ci mette un pò
sudo make install
cd ..
cd ..