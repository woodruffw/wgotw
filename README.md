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
