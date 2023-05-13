#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>

//____________________________asm_HashFuncs()
extern "C" size_t _RolHash(const char * word);
extern "C" size_t _FAQ6(const char * word);
extern "C" size_t __attribute__ ((noinline)) _crc32(const char* word);
//___________________________________________

//______________________________C_HashFuncs()
size_t rolHash(const char * word);
size_t asmInsertRolHash(const char * word);
size_t rolHashAsmROL(const char * word);
size_t FAQ6Hash(const char * word);
//___________________________________________
#endif