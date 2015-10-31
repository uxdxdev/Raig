# Build script for Linux systems

gcc -c -Wall -Werror -fpic ../Sockets.c
gcc -shared -o ../libs/libsocket.so ../Sockets.o
sudo cp ../libs/libsocket.so /usr/lib
