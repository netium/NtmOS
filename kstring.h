#ifndef _KERNEL_FUNCTIONS_C_H_
#define _KERNEL_FUNCTIONS_C_H_

typedef unsigned int size_t;

void* k_memcpy(void* destination, const void* source, size_t num);

void* k_memmove(void* destination, const void* source, size_t num);

void* k_memset(void* ptr, int value, size_t num);

int k_memcmp(const void* ptr1, const void* ptr2, size_t num);

const void* k_memchr(const void* ptr, int value, size_t num);

char* k_strcat(char* destination, const char* source);

const char* k_strchr(const char* str, int character);

int k_strcmp(const char* str1, const char* str2);

size_t k_strlen(const char* str);

char* k_strcpy(char* destination, const char* source);

void k_rev8(char* pbyte1, char* pbyte2);

void k_rev16(short* pword1, short* pword2);

void k_rev32(int* pdword1, int* pdword2);

void k_rev64(long long* pqword1, long long* pqword2);

void * k_memrev(void *ptr, size_t num);

#endif

