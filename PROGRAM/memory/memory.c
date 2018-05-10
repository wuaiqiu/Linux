#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * 	void* malloc(size_t size):配置内存空间
 * 	void* calloc(size_t size,size_t num):配置size*num内存空间
 * 	void free(void* p):释放内存
 *	int memcmp(void *s1,void *s2,size_t n):比较s1和s2所指的内存区间前n个字符，返回差值
 *	void memcpy(void *dest,void *src ,int n):拷贝src所指的内存内容前n个字节到dest所指
 *	的地址
 *	void * memmove(void *dest,const void *src,size_t n):与memcpy相同，但解决了内存重叠
 *	问题
 *	void * memset(void *s ,int c, size_t n):参数s所指的内存区域前n个字节以参数c填入，然
 *	后返回指向s的指针
 *
 * 	size_t ==> long unsigned int
 * */

struct co{
	int index;
	char name[8];
};

int main(){
	struct co* p1=(struct co*)malloc(sizeof(struct co));
	struct co* p2=(struct co*)calloc(sizeof(struct co),1);
	memcpy(p1->name,"aa" , 2);
	memset(p2->name,(int)'b',2);
	printf("%s\n",p1->name);
	printf("%s",p2->name);
	free(p1);
	free(p2);
	return 0;
}

/*
 * 内存重叠：拷贝的目的地址在源地址范围内。所谓内存重叠就是拷贝的目的地址和源地址有重叠。
 * */