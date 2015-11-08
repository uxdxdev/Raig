# Build script for Linux systems

g++ -std=c++11 -c -Wall -Werror -fpic raig.cc
g++ -std=c++11 -shared -o libs/libraig.so raig.o -L../libsocket/libs -lsocket
sudo cp libs/libraig.so /usr/lib
