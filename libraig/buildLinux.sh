# Build script for Linux systems

gcc -c -Wall -Werror -fpic raig.cc
gcc -shared -o libraig.so raig.o
sudo cp libs/libraig.so /usr/lib
