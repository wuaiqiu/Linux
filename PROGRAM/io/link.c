#include <stdio.h>
#include <unistd.h>


/*
 * int symlink(char* oldpath,char* newpath):创建软链接
 * int link(char* oldpath,char* newpath):创建硬链接
 * */

int main(){
	symlink("/home/wu/wifi","/home/wu/symlink");
	link("/home/wu/wifi","/home/wu/link");
	return 0;
}
