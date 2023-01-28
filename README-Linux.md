# All of the Linux Commands I Personally Found to be Useful

## System
```console
$ cat /proc/cpuinfo # Returns CPU info
$ tty # Returns the filename of the terminal connected to standard input
```

## Service
```console
$ systemctl --type=service --type=<ACTIVE|RUNNING|etc.> # Returns all services

$ systemctl enable|disable <SERVICE_NAME> # Enables/disables a service

$ systemctl start <SERVICE_NAME> # Starts a service
$ systemctl restart <SERVICE_NAME> # Restarts a service
$ systemctl stop <SERVICE_NAME> # Stops a service
$ systemctl status <SERVICE_NAME> # Returns the status of a service
```

## Network
```console
$ ip addr|a # Returns IP addresses and property information
$ ip addr|a show dev <DEV_NAME> # Returns info belonging to <DEVICE_INFO>

$ ip addr|a add <IP_ADDR>/<NETMASK> dev <DEV_NAME> # Adds an IP address
# ip addr add 192.168.1.1/24 dev eth02

$ ip addr|a del <IP_ADDR>/<NETMASK> dev <DEV_NAME> # Deletes an IP address
# ip addr del 192.168.1.1/24 dev eth02

$ ip link|l set <DEV_NAME> up|down # Bring device (interface) up or down
# ip link set eth02 up

$ ip route|r # Returns routing tables

$ ip route|r add default|<IP_ADDR> via <IP_ADDR> dev <DEV_NAME> # Add to routing table
#
# If you want to add default gateway then use 'default
# If you want to route a specific IP address then use '<IP_ADDR>'
# You can choose not to use 'dev <DEV_NAME>' for routing a specific IP address
#
# ip route add default via 192.168.1.1 dev eth02
# ip route add 192.168.1.0/24 via 192.168.1.1

$ ip route|r delete default|<IP_ADDR> via <IP_ADDR> # Deletes a route
# ip route delete 192.168.1.0/24 via 192.168.1.1

$ ip route get <IP_ADDR> # Returns the route taken for <IP_ADDR>

$ ss -antu # Returns LISTENING | ESTABLISHED | etc. ports
```

# Administration
```console
$ cat /etc/passwd # Returns all users and their default shell
```

## Utility
```console
$ find <DIR> -name <REGEX> # Searches the <DIR> for any mathching <REGEX>
# find /etc -name *.conf
```
