![image](https://user-images.githubusercontent.com/3397660/219311144-140b2f85-ab10-48d4-bcab-94f5cedc2263.png)

# R.A.I.G Server Application
Remote Artificial Intelligence for Games is a server application that processes AI requests from game clients using the RAIG Client API https://github.com/damorton/libraig.git. RAIG uses the BSD Sockets API for process communication over networks. 

## Instructions

### Ubunutu

- Download the repo
- Extract contents of the .zip file
- `cd raig-master/`
- Build
```
$ mkdir linux-build && cd linux-build
$ cmake ..
$ make
```
- Run `../bin/raig`

### Windows 

Coming soon...

## Dependencies

- libsocket   https://github.com/damorton/libsocket.git

Libraries used in the project are built and installed using `cmake` into the `external` directory.
