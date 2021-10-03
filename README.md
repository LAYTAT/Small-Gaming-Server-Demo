# Server Architec Demo
###### create by lay on 2020/12/2

This is a small and somewhat scalable gaming server.
Personal project, take it if needed.

## Key Designs
1. updated buffer manager for socket connections, with less system overhead(less copy).
2. Login servers
3. Gate servers
4. Game server2
5. DB server
6. A Buffer manager with less system overhead.

## Others
1. Use google protobuf for btw-server-comms.
2. Multiple Logically Seperate Servers, from login servers to gate servers, master servers, db servers and servers to be added.

## TODO
1. A Master server
