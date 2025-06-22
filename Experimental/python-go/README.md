# python-go

My goal is to have a Python script execute Go functions in an efficient & nice way.
Naturally, they need to talk to each other. There are many ways of achiving this via a
client-server architecture. However, that is not without it's overhead (runtime & development).

Here, I will try to evalaute the following methods of achiving that goal:

- Embedded Python in Go via CPython and cgo
- gRPC between Go and Python processes
