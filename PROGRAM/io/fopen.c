#include <stdio.h>

/*
 * 	带缓冲的I/O是指进程对输入输出流进行了改进，提供了一个流缓冲，当用fwrite函数网磁盘写数
 *据时，先把数据写入流缓冲区中，当达到一定条件，比如流缓冲区满了，或刷新流缓冲，这时候才会
 *再经系统调用(read/write)写入磁盘,因此，带缓冲的I/O在往往磁盘写入相同的数据量时，会比不带
 *缓冲的I/O调用系统调用的次数要少
 *
 *	FILE* fopen(char* path,char* mode):打开或创建文件
 *	int fgetc(FILE* stream):从文件读取一个字符,stdin为控制台输入流
 *	int fputc(int c,FILE* stream):将一字符写入文件,stdout为控制台输出流
 *	char* fgets(char* s,int size,FILE* stream):从文件读取一个字符串
 *	int fputs(char* s,FILE* stream):将一字符串写入文件
 *	size_t fread(void* ptr,size_t size,size_t count,FILE* stream):从文件流读取数据(count条，每条size个字节)
 *	size_t fwrite(void* ptr,size_t size,size_t count,FILE* stream):将数据写入文件(count条，每条size个字节)
 *	int fseek(FILE* stream,long offset,int whence):移动文件流读写位置
 *	int ftell(FILE* stream):返回当前位置
 *	void rewind(FILE* stream):设置读写指针为文件头
 *	int fclose(FILE* stream):关闭文件
 * */

/*
 * fgetc与fputc
 * */

void fun1(){
	FILE* stream=fopen("/home/wu/wifi","a+");
	fputc(fgetc(stdin),stream);
	fclose(stream);
}

/*
 * fgets与fputs
 * */

void fun2(){
	FILE* stream=fopen("/home/wu/wifi","a+");
	char ch[12];
	fputs(fgets(ch,10,stdin),stream);
	fclose(stream);
}

/*
 * fwrite与fread
 * */
void fun3(){
	FILE* stream=fopen("/home/wu/wifi","a+");
	char ch[10];
	fread(ch,10,1,stdin);
	fwrite(ch,10,1,stream);
	fclose(stream);
}

/*
 * fseek与ftell与rewind
 * */

void fun4(){
	FILE* stream=fopen("/etc/passwd","r");
	fseek(stream,10,SEEK_SET);
	printf("文件流的偏移量: %d\n",ftell(stream));
	rewind(stream);
	printf("文件流的偏移量: %d\n",ftell(stream));
	fclose(stream);
}
int main(){
	fun1();
	fun2();
	fun3();
	fun4();
	return 0;
}

/*
 * mode:
 * 	  r:只读,该文件必须存在
 * 	  r+:读写,该文件必须存在
 * 	  w:只写
 * 	  w+:读写
 * 	  a:追加只写
 * 	  a+:追加读写
 *
 * whence:
 * 	SEEK_SET:以文件头为基准(正数)
 * 	SEEK_CUR:以目前指针为基准(正数与负数)
 * 	SEEK_END:以文件尾为基准(负数)
 * */
