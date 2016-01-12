# R.A.I.G Server Application
Remote Artificial Intelligence for Games is a server application that processes AI requests from clients using the RAIG Client API https://github.com/damorton/libraig.git.

## Instructions
- Download the repo
- Extract contents of the .zip file
- `cd raig-folder/`
- Build
```
$ cmake .
$ make
```
- Run `$ ./raig`

## Dependencies

- libsocket   https://github.com/damorton/libsocket.git

Libraries used in the project are built and installed using `cmake`.