#ifndef _K_VARARGS_H_
#define _K_VARARGS_H_

typedef char* va_list;

#define _K_ADDRESSOF(v)	(&(v))
#define _K_INTSIZEOF(n)	((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define va_start(ap, v)	((void)(ap = (va_list)_K_ADDRESSOF(v) + _K_INTSIZEOF(v)))
#define va_arg(ap, t)	(*(t*)((ap += _K_INTSIZEOF(t)) - _K_INTSIZEOF(t)))
#define va_end(ap)	((void)(ap = (va_list)0))
#endif