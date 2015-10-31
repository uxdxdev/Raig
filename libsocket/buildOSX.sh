# Build script for OSX systems
# pwd scripts/

gcc -dynamiclib Sockets.c -o libs/libsocket.dylib
sudo cp libs/libsocket.dylib /usr/lib