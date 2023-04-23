#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

extern "C" size_t _RolHash(const char * word);

//________________________________HashFuncs()
size_t rolHash(const char * word);
size_t asmInsertRolHash(const char * word);
size_t rolHashAsmROL(const char * word);
//___________________________________________

#endif