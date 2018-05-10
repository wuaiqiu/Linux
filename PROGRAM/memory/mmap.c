#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

/*
 * 	void* mmap(void* start,size_t length,int prot,int flags,int fd,off_t offsize):将某
 * 	个文件所占的物理块大小的内容映射到以页的整数倍内存空间中，对该内存区域的读写操作会异步写
 * 	入磁盘文件
 * 	void* munmap(void* start,size_t length):解除映射关系
 * 	size_t getpagesize():获取页大小
 *	int msync(void *start, size_t length, int flags):同步写入映射文件(close映射文件也
 *	可以写入)
 *
 * 	start:映射内存的首地址
 * 	length:映射文件的大小(映射内存为页的整数倍，当写入内存超过文件的大小将被忽略)
 * 	prot:
 * 		PROT_EXEC:映射内存可被执行
 * 		PROT_READ:映射内存可被读取
 * 		PROT_WRITE:映射内存可被写入
 * 		PROT_NONE:映射内存不空读写
 *  flags:
 *  	MAP_SHARED:映射文件可被修改，以此可以实现进程共享
 *  	MAP_PRIVATE:映射文件不可被修改
 *  fd:文件描述符
 *  offsize:映射文件的偏移量，0表示文件头
 * */

int main(){
	int fd=open("/home/wu/hello",O_RDWR);
	struct stat sb;
	fstat(fd,&sb);
	printf("%d\n",sb.st_size);
	void* start=mmap(NULL,sb.st_size,PROT_WRITE,MAP_SHARED,fd,0);
	if(start==MAP_FAILED)return 0;
	printf("%s\n",start);
	msync();
	munmap(start,sb.st_size);
	close(fd);
	return 0;
}


/*
 * 	1.用户空间就是用户进程所在的内存区域
 * 	2.系统空间就是操作系统占据的内存区域
 * 	3.处于用户态的程序只能访问用户空间，而处于内核态的程序可以访问用户空间和内核空间
 * 	4.CPU执行内核代码和执行用户程序代码没什么区别；但是注意到，内核代码对用户参数是充
 * 	分的不信任,每次都要进行检查
 * 	5.用户态切换到内核态的3种方式:
 * 		a. 系统调用:这是用户态进程主动要求切换到内核态的一种方式，用户态进程通过系统
 * 		调用申请使用操作系统提供的服务程序完成工作，比如fork()实际上就是执行了一个创
 * 		建新进程的系统调用。(进程控制,文件系统控制,内存管理,网络管理,用户管理)
 *
 * 		b. 异常:当CPU在执行运行在用户态下的程序时，发生了某些事先不可知的异常，这时
 * 		会触发由当前运行进程切换到处理此异常的内核相关程序中，也就转到了内核态，比如
 * 		缺页异常.(物理页不可写或不存在，需要内核态进行分配)
 *
 * 		c. 外围设备的中断:当外围设备完成用户请求的操作后，会向CPU发出相应的中断信号，
 * 		这时CPU会暂停执行下一条即将要执行的指令转而去执行与中断信号对应的处理程序，
 * 		如果先前执行的指令是用户态下的程序，那么这个转换的过程自然也就发生了由用户态
 * 		到内核态的切换。比如硬盘读写操作完成，系统会切换到硬盘读写的中断处理程序中执
 * 		行后续操作等。
 * */
