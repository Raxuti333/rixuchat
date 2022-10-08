# Build and install rixuchat

if you want to install all both client and server just run:

```
make all
```
if you want just the client
```
make client
```
or for server
```
make server
```

# Using rixuchat-client

to run rixuchat client you need to know the server address and port
```
rhat <name>@<address>:<port>
```

# Using rixuchat-server

currently rixuchat only supports IPv4.

to start a rixuhchat-server you need to specify the a ip the and the port
```
rhat-server <ipv4>:<port>
```

# Notes

Rixuchat has a lot few know bugs and mostlikely a lot of unknow bugs. Rixuchat is a simple test project writen to test how to do UDP client and server. Not intended for use