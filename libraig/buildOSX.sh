# Build script for OSX systems
# pwd scripts/

g++ -dynamiclib raig.cc -L../libsocket/libs -o libs/libraig.dylib -lsocket
sudo cp libs/libraig.dylib /usr/lib
