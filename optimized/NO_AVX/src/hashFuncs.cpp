#include "./include/hashFuncs.hpp"

extern "C" size_t _AsmRolFunc(size_t num);
size_t RolFunc (size_t num);

size_t rolHash(const char * word) {

    size_t hash = 0;

    for (; *word; word++) {
        hash = RolFunc(hash) ^ (*word);
    }
    return hash;
}

size_t rolHashAsmROL(const char * word) {

    size_t hash = 0;

    for (; *word; word++) {
        hash = _AsmRolFunc(hash) ^ (*word);
    }
    return hash;
}

size_t asmInsertRolHash(const char* word) {
    size_t hash = 0;

    for (; *word; word++) {
        asm (
            ".intel_syntax noprefix\n\t"
            "mov rax, %1 \n\t"
            "rol rax\n\t"
            "movsx rbx, BYTE PTR[%2]\n\t"
            "xor rax, rbx\n\t"
            "mov %0, rax\n\t"
            ".att_syntax prefix\n\t" 
            : "=r"(hash)
            : "r"(hash), "r"(word)
            : "%rax", "%rbx"    
        );
    }

    return hash;
}

size_t RolFunc (size_t num) {                                 // 10011000 ----> 0011001
    return (num << 1) | (num >> (sizeof(num) * 8 - 1));
}

size_t FAQ6Hash(const char * word) {

    size_t hash = 0;

    for (; *word; word++)
    {
        hash += (unsigned char)(*word);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;

}

