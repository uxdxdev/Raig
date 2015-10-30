# Build script for OSX systems

gcc -dynamiclib Sockets.c -o libsocket.dylib
sudo cp libsocket.dylib /usr/lib
