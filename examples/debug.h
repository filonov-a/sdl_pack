//
// Created by aef
//

#ifndef DEBUG_H
#define DEBUG_H

extern int debugActive;

void debug(const char *format, ...);

void error(const char *format, ...);

void hexDump(const char *, void *, unsigned int);

#endif 
