#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

/*
 * 	DIR* opendir(char* paht):打开目录
 * 	struct dirent* readdir(DIR* dir):读取目录
 * 	int closedir(DIR* dir):关闭目录
 * */

int main(){
	DIR* dir=opendir("/home/wu");
	struct dirent* ptr;
	while((ptr=readdir(dir))!=NULL){
		printf("%s\n",ptr->d_name);
	}
	closedir(dir);
	return 0;
}
