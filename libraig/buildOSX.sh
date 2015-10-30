# Build script for OSX systems

g++ -dynamiclib Raig.cpp -o libs/libraig.dylib
sudo cp libs/libraig.dylib /usr/lib
