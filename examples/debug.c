//
// Created by aef
//

#include "debug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int debugActive = 1;

void debug(const char *format, ...) {
    if (!debugActive)
        return;
    va_list list;
    va_start(list, format);
    vfprintf(stderr, format, list);
    va_end(list);
}

void error(const char *format, ...) {
    va_list list;
    va_start(list, format);
    vfprintf(stderr, format, list);
    va_end(list);
}

void hexDump(char const *desc, void *addr, unsigned int len) {
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char *) addr;

    if (desc != NULL)
        debug("%s: %u (%p)\n", desc, len, addr);
    if (addr == NULL) {
        debug("\t NULLPTR\n", desc, len);
    }
    if (len <= 0) {
        debug("  <Empty data>\n");
        return;
    }
    for (i = 0; i < len; i++) {
        if ((i % 16) == 0) {
            if (i != 0)
                debug("  %s\n", buff);
            debug("  %04x ", i);
        }
        debug(" %02x", pc[i]);

        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
            buff[i % 16] = '.';
        else
            buff[i % 16] = pc[i];

        buff[(i % 16) + 1] = '\0';
    }
    while ((i++ % 16) != 0) {
        debug("   ");
    }

    // And print the final ASCII bit.
    debug("  %s\n", buff);
}
