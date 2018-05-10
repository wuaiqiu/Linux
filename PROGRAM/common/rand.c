#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
 * 利用函数rand产生10个(1-10)的随机数
 *
 * 		int rand(void)
 * 		void srand(unsigned int seed)
 * */


int main(){
	int j;
	//设置随机种子，如果srand每次输入的数值一样，那么运行产生的随机数也是一样的
	srand((unsigned)time(0));
	for(int i=0;i<10;i++){
		j=rand()%10+1;
		printf("%d ",j);
	}
	return 0;
}