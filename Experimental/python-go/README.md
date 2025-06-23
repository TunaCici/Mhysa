# python-go

My goal is to have a Python script execute Go functions in an efficient & nice way.
Naturally, they need to talk to each other. There are many ways of achiving this via a
classical client-server architecture. However, that is not without it's overhead (runtime & development).

Another way is to execute the Python script within the server application.
The most ideal and battle-tested method of doing that is via CPython.
However, my goal is to do it within Go (not C) And sadly there is no Go idiomatic way of doing it.

Here, I will try to evalaute those followings. In other words:

- Embedd Python in Go via CPython and cgo
- gRPC between Go and Python processes
