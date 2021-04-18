#ifndef _STDARG_H_
#define _STDARG_H_

typedef char* va_list;

#define __va_int_sizeof(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define __va_addrof(v) (&(v))

#define va_start(ap, v) ((void)(ap = (va_list)__va_addrof(v) + __var_int_sizeof(v)))
#define va_arg(ap, t) ((*(t*) (( ap += __va_int_sizeof(t)) - __var_int_sizeof(t)))
#define va_end(ap) ((void)(ap = (va_list)0))
#define va_copy(destination, source) ((destination) == (source))
#endif