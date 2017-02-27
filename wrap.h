#ifndef WRAP_H
#define WRAP_H

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>


using namespace std;

int		Getopt(int argc, char * const argv[], const char *optstring);
int 	Open(const char *pathname, int oflag, ...);
void	Write(int fd, const void *buf, size_t count);
void	Close(int fd);
char *  itoa(int num, char * str, int radix);
FILE * 	Fopen(const char *pathname, const char *mode);
void 	Fclose(FILE *fp);
ssize_t Getline(char **lineptr, size_t *n, FILE *stream);
void * 	Malloc(size_t size);
int     myrand(int start, int end);



#endif