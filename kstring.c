#include "kstring.h"
#include <stddef.h>

void* k_memcpy(void* destination, const void* source, size_t num) {
	char* d = destination;
	const char * s = source;
	for (int i = 0; i < num; i++) { d[i] = s[i]; }
	return destination;
}

void* k_memmove(void* destination, const void* source, size_t num) {
	char *d = destination;
	const char *s = source;
	if (source <= destination) { // copy in ascent order
		for (size_t i = 0; i < num; i++) d[i] = s[i];
	}
	else {	// copy in descent order
		for (size_t i = num; i > 0; i--) d[num - 1] = s[num - 1];
	}
	return destination;
}

void* k_memset(void* ptr, int value, size_t num) {
	// slow version, shall use stosb/stosw/stosd instead
	char* d = ptr;
	for (size_t i = 0; i < num; i++) d[i] = value;

	return ptr;
}

int k_memcmp(const void* ptr1, const void* ptr2, size_t num) {
	const char* d = ptr1, * s = ptr2;
	for (size_t i = 0; i < num; i++) {
		int diff = d[i] - s[i];
		if (diff != 0) return diff;
	}
	return 0;
}

const void* k_memchr(const void* ptr, int value, size_t num) {
	const char* d = ptr;
	for (size_t i = 0; i < num; i++) if (d[i] == value) return d + i;
	return 0;
}

char* k_strcat(char* destination, const char* source) {
	char* d = destination;
	while (*d != 0) d++;
	while (*source != 0) { *d = *source; d++, source++; }
	*d = 0;
	return destination;
}

const char* k_strchr(const char* str, int character) {
	while (*str != 0) {
		if (*str == character) return str;
		str++;
	}
	return 0;
}

int k_strcmp(const char* str1, const char* str2) {
	while (*str1 != 0 && *str2 != 0) {
		int diff = *str1 - *str2;
		if (diff != 0) return diff;
		str1++, str2++;
	}

	return *str1 - *str2;
}

size_t k_strlen(const char* str) {
	const char *t = str;
	while (*t != 0) t++;
	return t - str;
}

char* k_strcpy(char* destination, const char* source) {
	char* d = destination;
	while ((*d++ = *source++) != 0);
	return destination;
}

char* k_strrev(char *str) {
	size_t l = k_strlen(str);
	k_memrev(str, l);
	return str;
}

void k_rev8(char* pbyte1, char* pbyte2) {
	char t = *pbyte1;
	*pbyte1 = *pbyte2;
	*pbyte2 = t;
}

void k_rev16(short* pword1, short* pword2) {
	short t = *pword1;
	*pword1 = *pword2;
	*pword2 = t;
}

void k_rev32(int* pdword1, int* pdword2) {
	int t = *pdword1;
	*pdword1 = *pdword2;
	*pdword2 = t;
}

void k_rev64(long long* pqword1, long long* pqword2) {
	long long t = *pqword1;
	*pqword1 = *pqword2;
	*pqword2 = t;
}

void * k_memrev(void *ptr, size_t num) {
	char *ph = ptr, *pt = (ph + num) - 1;	

	while (ph < pt) {
		k_rev8(ph, pt);
		ph++, pt--;
	}

	return ptr;
}

static const char NUM_MAP[] = "0123456789ABCDEF";

void *k_utoa(unsigned num, char* str, int base) {
	if (NULL == str) return NULL;
	if (base > 16) base = 16;

	size_t i = 0;

	if (num == 0) {
		str[i++] = NUM_MAP[num];
		str[i] = NULL;
		return str;
	}

	while (num != 0) {
		int r = num % base;
		str[i++] = NUM_MAP[r];
		num /= base;
	}

	str[i] = NULL;

	k_memrev(str, i);

	return str;
}

void *k_itoa(int num, char* str, int base){
	if (NULL == str) return NULL;
	if (base > 16) base = 16;

	size_t i = 0;
	int is_neg = 0;

	if (num == 0) {
		str[i++] = '0';
		str[i] = 0x0;
		return str;
	}

	if (num < 0 && base == 10) {
		is_neg = 1;
		num = -num;
	}

	while (num != 0) {
		int r = num % base;
		str[i++] = NUM_MAP[r];
		num /= base;
	}

	if (is_neg) str[i++] = '-';

	str[i] = NULL;

	k_memrev(str, i);

	return str;
}

int k_sprintf(char *str, const char *fmt, ...) {
	const char * sp = (char*)&fmt;
	sp += sizeof(const char*);

	int int_temp;
	char char_temp;
	char *string_temp;
	double double_temp;

	char ch;
	int l = 0;

	char buffer[256];

	while ( ch = *fmt++ ) {
		if ('%' == ch) {
			switch (ch = *fmt++) {
				case '%':
					str[l++] = ch;
					break;
				case 's':
					string_temp = *(char **)sp;
					k_strcat(str, string_temp);
					l += k_strlen(string_temp);
					sp += sizeof (char *);
					break;
				case 'd':
					int_temp = *(int *)sp;
					k_itoa(int_temp, buffer, 10);
					k_strcat(str, buffer);
					l += k_strlen(buffer);
					sp += sizeof (int);
					break;
				case 'x':
					int_temp = *(int *)sp;
					k_itoa(int_temp, buffer, 16);
					k_strcat(str, buffer);
					l += k_strlen(buffer);
					sp += sizeof (int);
					break;
				case 0:	// Reach out to the end of the format string
					goto end;
			}
		}
		else {
			str[l++] = ch;
		}
	}
end:
	str[l] = 0x0;
	return l;
}