#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

int add(int a, int b)
{
	return a + b;
}

int lib_get_version(void)
{
	return 0x10001000;
}
int lib_get_count(void)
{
	return 1;
}
char* lib_get_function(int at)
{
	if(at == 0)
	{
		return (char*)"int add(int a, int b)";
	}
}


#ifdef __cplusplus
}
#endif
