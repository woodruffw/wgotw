wgotw
=====

**Work in progress.**

WGOtW is a program traffic inspection tool.

Unlike system-wide traffic inspection tools like Wireshark, WGOtW only
seeks to capture the traffic for a single program.

It also doesn't attempt to capture underlying IP or Ethernet packets - only
the buffers sent and received through the `send(2)` and `recv(2)` family
of functions.

### Building

```bash
$ make
```

### Usage

You can use the included wrapper script:

```
$ ./wgotw --help
```

Or just `LD_PRELOAD` the library directly:

```bash
$ LD_PRELOAD=./libwgotw.so <cmd [args ...]>
```

### Example

```
$ ./wgotw -Dd 'curl example.com'
[ ... snip ... ]

$ ls -R wgotw-934/
wgotw-934/:
127.0.1.1  93.184.216.34

wgotw-934/127.0.1.1:
53

wgotw-934/127.0.1.1/53:
inbound.0  inbound.1

wgotw-934/93.184.216.34:
80

wgotw-934/93.184.216.34/80:
inbound.1  outbound.0

$ cat wgotw-934/93.184.216.34/80/outbound.0
GET / HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 Gecko
Accept: */*

$ cat wgotw-934/93.184.216.34/80/inbound.1
HTTP/1.1 200 OK
Cache-Control: max-age=604800
Content-Type: text/html
Date: Fri, 05 May 2017 01:12:41 GMT
Etag: "359670651+ident"
Expires: Fri, 12 May 2017 01:12:41 GMT
Last-Modified: Fri, 09 Aug 2013 23:54:35 GMT
Server: ECS (ewr/15BD)
Vary: Accept-Encoding
X-Cache: HIT
Content-Length: 1270

<!doctype html>
<html>
<head>

[ ... snip ... ]
```
