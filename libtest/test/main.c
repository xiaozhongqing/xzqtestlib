#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define RN	"\n"
typedef int	(*pfn_get_int)(void);
typedef char*	(*pfn_get_help)(int at);
typedef int	(*pfn_get_func)(void* param);
struct context
{
	int a;
	int b;
	int r;
};
int main(int argc, char** argv)
{
	void* plib;
	int i, count;
	pfn_get_int 	fn_version;
	pfn_get_int 	fn_count;
	pfn_get_help 	fn_help;
	pfn_get_func 	fn_add;
	char filename[64] = {0};
	if(argc < 2)
	{
		printf("INFO: ./test libXXX.so  or  ./test libXXX.so Y\n");
		return -1;
	}
	sprintf(filename, "./%s", argv[1]);
	plib = dlopen(filename, RTLD_NOW | RTLD_GLOBAL);
	if(!plib)
	{
		printf("ERROR: dlopen (%s) failed\n", filename);
		return -1;
	}
	fn_version = (pfn_get_int)dlsym(plib, "lib_get_version");
	if(!fn_version)
	{
		printf("ERROR: dlsym (%s) failed\n", "lib_get_version");
		dlclose(plib);
		return -1;
	}
	printf("INFO: %s, version: %08X\n", filename, fn_version());
	
	fn_count = (pfn_get_int)dlsym(plib, "lib_get_count");
	if(!fn_count)
	{
		printf("ERROR: dlsym (%s) failed\n", "lib_get_count");
		dlclose(plib);
		return -1;
	}
	count = fn_count();
	printf("INFO: %s, count: %d\n", filename, count);

	fn_help = (pfn_get_help)dlsym(plib, "lib_get_help");
	if(!fn_help)
	{
		printf("ERROR: dlsym (%s) failed\n", "lib_get_help");
		dlclose(plib);
		return -1;
	}
	for(i = 0; i< count; i++)
	{
		printf("INFO: %s, function: %s\n", filename, fn_help(i));
	}
	fn_add = (pfn_get_func)dlsym(plib, "add");
	struct context c = {0};
	c.a = 3;
	c.b = 4;
	if(fn_add(&c) == 0)
	{
		printf("add result: %d\n", c.r);
	}
	else
	{
		printf("add function failed\n");
	}
	if(argc >= 3 && (argv[2][0] == 'y' || argv[2][0] == 'Y'))
	{
		FILE* file;
		int len = (int)strlen(filename);
		for(i = len - 1; i > 0; i--)
		{
			if(filename[i] == '.')
			{
				filename[i + 1] = 'h';
				break;
			}
			else
			{
				filename[i] = 0;
			}
		}
		file = fopen(filename, "w+");
		if(!file)
		{
			dlclose(plib);
			printf("ERROR: fopen(%s) failed\n", filename);
			return -1;
		}
		sprintf(filename, "%s", argv[1]);
		char msg[1024] = {0};
		len = (int)strlen(filename);
		for(i = 0; i < len; i++)
		{
			if(filename[i] == '.')
			{
				filename[i] = '_';
			}
			if(filename[i] >= 'a' && filename[i] <= 'z')
			{
				filename[i] = filename[i] - 'a' + 'A';
			}
		}
		sprintf(msg, "#ifndef _H_%s_H_%s", filename, RN);
		fwrite(msg, 1, strlen(msg), file);
		sprintf(msg, "#define _H_%s_H_%s%s", filename, RN, RN);
		fwrite(msg, 1, strlen(msg), file);
		sprintf(msg, "#ifdef __cplusplus%sextern \"C\"%s{\%s#endif%s%s", RN, RN, RN, RN, RN);
		fwrite(msg, 1, strlen(msg), file);
		for(i = 0; i < count; i++)
		{
			sprintf(msg, "%s %s", fn_help(i), RN);
			fwrite(msg, 1, strlen(msg), file);
		}
		sprintf(msg, "%s#ifdef __cplusplus%s}%s#endif%s%s", RN, RN, RN, RN, RN);
		fwrite(msg, 1, strlen(msg), file);
		sprintf(msg, "#endif%s%s", RN, RN);
		fwrite(msg, 1, strlen(msg), file);
		fflush(file);
		fclose(file);		
	}
	dlclose(plib);

	return 0;
}

