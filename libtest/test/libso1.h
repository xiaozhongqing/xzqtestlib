#ifndef _H_LIBSO1_SO_H_
#define _H_LIBSO1_SO_H_

#ifdef __cplusplus
extern "C"
{
#endif


struct context
{
	int a;
	int b;
	int result;
}
int add(void* param); 

#ifdef __cplusplus
}
#endif

#endif

