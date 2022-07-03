---
title: Debugging X11 with tcpdump
date: 2020-02-24 09:18:03
tags: Window System
categories: graphics
---

X11 is designed as client-server mode. The communication between the X client and server complies with TCP protocol. Recently I have a Windows X server VcXsrv installed on my Windows 10 and I debug an OpenGL demo glxgears on the WSL2 with tcpdump.

## Environment
WSL2 is equipped with its own networking interface like a virtual machine. 

``` bash
$ ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 172.28.233.193  netmask 255.255.240.0  broadcast 172.28.239.255
        inet6 fe80::215:5dff:fe27:9562  prefixlen 64  scopeid 0x20<link>
        ether 00:15:5d:27:95:62  txqueuelen 1000  (Ethernet)
        RX packets 221579  bytes 15160108 (15.1 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 306691  bytes 15714800233 (15.7 GB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 392  bytes 43340 (43.3 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 392  bytes 43340 (43.3 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

```

``` bash
$ /mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe -C "ipconfig"

Windows IP Configuration


Ethernet adapter vEthernet (WSL):

   Connection-specific DNS Suffix  . : 
   Link-local IPv6 Address . . . . . : fe80::412b:ad3:5548:67ba%38
   IPv4 Address. . . . . . . . . . . : 172.28.224.1
   Subnet Mask . . . . . . . . . . . : 255.255.240.0
   Default Gateway . . . . . . . . . : 

Wireless LAN adapter ±¾µØÁ¬½Ó* 1:

   Media State . . . . . . . . . . . : Media disconnected
   Connection-specific DNS Suffix  . : 

Wireless LAN adapter ±¾µØÁ¬½Ó* 10:

   Media State . . . . . . . . . . . : Media disconnected
   Connection-specific DNS Suffix  . : 

Wireless LAN adapter WLAN:

   Connection-specific DNS Suffix  . : DHCP HOST
   Link-local IPv6 Address . . . . . : fe80::954b:e66f:4065:20af%20
   IPv4 Address. . . . . . . . . . . : 192.168.2.100
   Subnet Mask . . . . . . . . . . . : 255.255.255.0
   Default Gateway . . . . . . . . . : 192.168.2.1
```

After starting up the X server VcXsrv, you need to export the environmnet variable **`DISPLAY`** on the WSL. In case of the vEthernet configuration changed after rebooting you'd better do it like this:

``` bash
export DISPLAY=$(grep 'nameserver' /etc/resolv.conf | awk '{print $2}'):0
```

NOTE: The Firewall between the host and WSL2 must be disabled or your X client can not connect VcXsrv.

## Debugging

I trace the demo glxgears using gdb and tcpdump at the same time.

- gdb

``` shell
gdb -q -tui glxgears
```
- tcpdump

``` shell
sudo tcpdump -vvX not icmp and not arp and not udp and portrange 37900-37999 -w x110224.pcap
```
- vv: verboser than -v
- X: show the packet's content
- not icmp: filter out icmp packets
- not arp: filter out arp packets
- not udp: filter out udp packets
- portrange 37900-37999: listening on the ports from 37900 to 37999
- w x110224.pcap: save the packet captures into the file

``` shell
tcpdump -X -r x110224.pcap
```

```
reading from file /home/luc/github/x110224.pcap, link-type EN10MB (Ethernet)
09:11:56.659816 IP 172.28.233.193.37950 > 172.28.224.1.x11: Flags [P.], seq 2796598646:2796598658, ack 1053772981, win 259, length 12
	0x0000:  4500 0034 1d84 4000 4006 fb43 ac1c e9c1  E..4..@.@..C....
	0x0010:  ac1c e001 943e 1770 a6b0 b576 3ecf 4cb5  .....>.p...v>.L.
	0x0020:  5018 0103 2223 0000 6200 0300 0400 0000  P..."#..b.......
	0x0030:  4452 4932                                DRI2
09:11:56.660218 IP 172.28.224.1.x11 > 172.28.233.193.37950: Flags [P.], seq 1:33, ack 12, win 8211, length 32
	0x0000:  4500 0048 d9d7 4000 8006 fedb ac1c e001  E..H..@.........
	0x0010:  ac1c e9c1 1770 943e 3ecf 4cb5 a6b0 b582  .....p.>>.L.....
	0x0020:  5018 2013 cf36 0000 0100 0a00 0000 0000  P....6..........
	0x0030:  0000 0000 0000 0000 0000 0000 0000 0000  ................
	0x0040:  0000 0000 0000 0000                      ........
09:11:56.660232 IP 172.28.233.193.37950 > 172.28.224.1.x11: Flags [.], ack 33, win 259, length 0
	0x0000:  4500 0028 1d85 4000 4006 fb4e ac1c e9c1  E..(..@.@..N....
	0x0010:  ac1c e001 943e 1770 a6b0 b582 3ecf 4cd5  .....>.p....>.L.
	0x0020:  5010 0103 2217 0000                      P..."...

```

what codes sends and receives these packets? The first two twenty-byted segments are IP header (20 bytes without option) and TCP header (20 bytes without option) separately in these packets.

The source code snippet:
``` c
Bool
XQueryExtension(
    register Display *dpy,
    _Xconst char *name,
    int *major_opcode,  /* RETURN */
    int *first_event,   /* RETURN */
    int *first_error)	/* RETURN */
{
    xQueryExtensionReply rep;
    register xQueryExtensionReq *req;

    LockDisplay(dpy);
    GetReq(QueryExtension, req);
    req->nbytes = name ? strlen(name) : 0;
    req->length += (req->nbytes+(unsigned)3)>>2;
    _XSend(dpy, name, (long)req->nbytes);
    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *major_opcode = rep.major_opcode;
    *first_event = rep.first_event;
    *first_error = rep.first_error;
    UnlockDisplay(dpy);
    SyncHandle();
    return (rep.present);
}
```

The gdb log:
```
Starting program: /mnt/c/Users/lulu/Documents/github/demos/src/xdemos/glxgears 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, XextAddDisplay (extinfo=0x7ffff7e77380 <_dri2Info_data>, dpy=0x55555555d2a0, 
    ext_name=0x7ffff7e76780 <dri2ExtensionName> "DRI2", hooks=0x7ffff7e767a0 <dri2ExtensionHooks>, nevents=0, data=0x0)
    at extutil.c:103
XInitExtension (dpy=dpy@entry=0x55555555d2a0, name=name@entry=0x7ffff7e76780 <dri2ExtensionName> "DRI2")
    at InitExt.c:44
XQueryExtension (dpy=dpy@entry=0x55555555d2a0, name=name@entry=0x7ffff7e76780 <dri2ExtensionName> "DRI2", 
    major_opcode=major_opcode@entry=0x7fffffffd984, first_event=first_event@entry=0x7fffffffd988, 
    first_error=first_error@entry=0x7fffffffd98c) at QuExt.c:39
$2 = {
  reqType = 0x62, 
  pad = 0x0, 
  length = 0x3, 
  nbytes = 0x4, 
  pad1 = 0x0, 
  pad2 = 0x0
}
$3 = {
  type = 0x1, 
  pad1 = 0x0, 
  sequenceNumber = 0xa, 
  length = 0x0, 
  present = 0x0, 
  major_opcode = 0x0, 
  first_event = 0x0, 
  first_error = 0x0, 
  pad3 = 0x0, 
  pad4 = 0x0, 
  pad5 = 0x0, 
  pad6 = 0x0, 
  pad7 = 0x0
}
```

`$2` is **Request** packet content to VcXsrv, `$3` is **Reply** packet content from VcXsrv. Even that we can notice the three-way handshake of TCP from the zero-lengthed packet in x110224.pcap. 
