#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct context
{
	int a;
	int b;
	int result;
};
int add(void* param)
{
	if(!param)
	{
		return -2;		
	}
	struct context* p = (struct context*)param;
	p->result = p->a + p->b;
	return 0;
}

int lib_get_version(void)
{
	return 0x10001000;
}
int lib_get_count(void)
{
	return 1;
}
char* lib_get_help(int at)
{
	if(at == 0)
	{
		return (char*)"\nstruct context\n{\n\tint a;\n\tint b;\n\tint result;\n}\nint add(void* param);";
	}
}


#ifdef __cplusplus
}
#endif
