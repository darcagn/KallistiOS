# libkosnet Makefile
#

TARGET = libkosnet.a

OBJS  = fs_dclsocket.o fs_socket.o

OBJS += getaddrinfo.o gethostbyname.o

OBJS += inet_addr.o inet_aton.o inet_ntoa.o inet_ntop.o inet_pton.o

OBJS += net_init.o net_core.o net_arp.o net_input.o net_icmp.o net_ipv4.o
OBJS += net_udp.o net_dhcp.o net_ipv4_frag.o net_thd.o net_ipv6.o net_icmp6.o
OBJS += net_crc.o net_ndp.o net_multicast.o net_tcp.o

OBJS += netif/broadband_adapter.o netif/lan_adapter.o netif/w5500_adapter.o

include $(KOS_BASE)/addons/Makefile.prefab
