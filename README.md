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
