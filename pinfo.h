#ifndef PINFO_H
#define PINFO_H

struct pinfo {
    int pid;
    int ppid;
    int state;
    int sz;
    char name[16];
};

#endif