// A struct to be shared by both user and kernel to print the processes list.

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