#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

//________________________________HashFuncs()
size_t rolHash(const char * word);
size_t asmInsertRolHash(const char* word);
size_t rolHashAsmROL(const char * word);
//___________________________________________

#endif