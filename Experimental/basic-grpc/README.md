# Basic-gRPC

Basic Go CLI tool that shows you the weather, stocks and other basic info.
It uses gRPC to communicate with a server applicaton.

# How to build

1. Generate interfaces /w `protoc`

```sh
cd Experimental/basic-grpc/proto
protoc --go_out=. --go_opt=paths=source_relative \
    --go-grpc_out=. --go-grpc_opt=paths=source_relative \
    basic-grpc.proto
```

2. Run the server in a shell

```sh
cd Experimental/basic-grpc/server
go run server.go
```

3. Run the client (CLI) in another shell

```sh
cd Experimental/basic-grpc/cli
go run client.go
```
