#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>


//________________________________HashFuncs()
size_t OneHash(const char * word);
size_t AsciiHash(const char * word);
size_t StrlenHash(const char * word);
size_t AsciiSumHash(const char * word);
size_t rolHash(const char * word);
size_t rorHash(const char * word);
size_t FAQ6Hash(const char * word);
//___________________________________________

#endif