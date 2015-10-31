# Build script for OSX systems
# pwd scripts/

g++ -dynamiclib Raig.cpp -L../libsocket/libs -o libs/libraig.dylib -lsocket
sudo cp libs/libraig.dylib /usr/lib
