#include <stdio.h>
#include <stdlib.h>

/*
 *	char* getenv(char* name):获取环境变量
 * 	int setenv(char* name,char* value,int overwrite):改变或增加环境变量
 * 	int unsetenv(char* name):清空环境变量的值
 * */

int main(){
	char *p;
	if((p=getenv("USER"))){
		printf("USER:%s\n",p);
	}
	setenv("TEST","test",1);
	printf("TEST:%s\n",getenv("TEST"));
	unsetenv("TEST");
	printf("TEST:%s\n",getenv("TEST"));
	return 0;
}
