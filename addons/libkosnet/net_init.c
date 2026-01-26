/* KallistiOS ##version##

   addons/kosnet/init.c
   Copyright (C) 2026 Eric Fradella
*/

#include <stdint.h>

#include <kos/dbgio.h>
#include <kos/dbglog.h>
#include <kos/init.h>

#include <dc/fs_dcload.h>

#include <kosnet/net.h>
#include <kosnet/netif/broadband_adapter.h>
#include <kosnet/netif/lan_adapter.h>
#include <kosnet/netif/w5500_adapter.h>
#include <kosnet/fs_dclsocket.h>

uint32_t _fs_dclsocket_get_ip(void);

void arch_init_net_dcload_ip(void) {
    union {
        uint32_t ipl;
        uint8_t ipb[4];
    } ip = { 0 };

    if(dcload_type == DCLOAD_TYPE_IP) {
        /* Grab the IP address from dcload before we disable dbgio... */
        ip.ipl = _fs_dclsocket_get_ip();
        dbglog(DBG_INFO, "dc-load says our IP is %d.%d.%d.%d\n", ip.ipb[3],
            ip.ipb[2], ip.ipb[1], ip.ipb[0]);
        dbgio_disable();
    }

    net_init(ip.ipl);     /* Enable networking (and drivers) */

    if(dcload_type == DCLOAD_TYPE_IP) {
        fs_dclsocket_init_console();

        if(!fs_dclsocket_init()) {
            dbgio_dev_select("fs_dclsocket");
            dbgio_enable();
            dbglog(DBG_INFO, "fs_dclsocket console support enabled\n");
        }
    }
}

void arch_init_net_no_dcload(void) {
    net_init(0);
}

KOS_INIT_FLAG_WEAK(arch_init_net_dcload_ip, true);
KOS_INIT_FLAG_WEAK(arch_init_net_no_dcload, false);
KOS_INIT_FLAG_WEAK(fs_dclsocket_shutdown, true);

void eth_init(void) {
    bba_init();
    la_init();
    w5500_adapter_init(NULL, true);
}

void eth_shutdown(void) {
    la_shutdown();
    bba_shutdown();
    w5500_adapter_shutdown();
}

#include <kos/thread.h>

/* Weak symbol allows overriding by end user applications */
void __weak_symbol arch_net_init_custom(void) {
    /* Install the dbgio handler for dcload socket */
    dbgio_add_handler(&dbgio_dcls);

    /* Initialize our network hardware */
    eth_init();

    KOS_INIT_FLAG_CALL(arch_init_net_dcload_ip);
    KOS_INIT_FLAG_CALL(arch_init_net_no_dcload);

    thd_sleep(250);
}

/* Weak symbol allows overriding by end user applications */
void __weak_symbol arch_net_shutdown_custom(void) {
    KOS_INIT_FLAG_CALL(fs_dclsocket_shutdown);

    net_shutdown();
    eth_shutdown();
}

void arch_net_init(void) {
    arch_net_init_custom();
}

void arch_net_shutdown(void) {
    arch_net_shutdown_custom();
}
