# Build script for OSX systems

gcc -dynamiclib ../Sockets.c -o ../libs/libsocket.dylib
sudo cp ../libs/libsocket.dylib /usr/lib
