#include "ReprFormatBuilder.h"
#include <string.h>

char* ReprFormatBuilder::Build(int num, char* type)
{
	char * buff = new char[64];
	buff[0] = '\0';
	for(int i=0; i<num ; num++)
	{
		strcat(buff,type);
	}
	return buff;
}
