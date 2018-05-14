#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>

/*
 * int dup (int fd):复制文件标识符,而fork出的子进程的文件标识符为同一个
 * int flock(int fd,int operation):锁定文件或解除锁定
 * 	 1.只能对整个文件上锁，而不能对文件的某一部分上锁
 *	 2.子进程继承父进程的锁(通过frok与dup复制文件)
 *	 3.使用open两次打开同一个文件，得到的两个fd是独立的,无法同时上锁
 *	 4.flock锁只支持本地，不能再NFS文件系统上使用
 *	 5.针对文件为基础
 * int fcntl(int fd,int cmd,struct flock *lock):锁定文件或解除锁定
 * 	 1.针对进程为基础
 * */

/*
 *flock父子进程继承锁
 * */

void fun1(){
	int ret;
	int fd1 = open("/home/wu/wifi",O_RDWR);
	int fd2 = dup(fd1);
	int fd3 = open("/home/wu/wifi",O_RDWR);
	printf("fd1: %d, fd2: %d, fd3: %d\n", fd1, fd2,fd3);
	ret = flock(fd1,LOCK_EX);
	printf("get lock1, ret: %d\n", ret);
	ret = flock(fd2,LOCK_EX);
	printf("get lock2, ret: %d\n", ret);
	ret =flock(fd3,LOCK_EX);
	printf("get lock3, ret: %d\n", ret);
}

/*
 * 可以加上多个建议锁
 * */
void fun2(){
		int ret;
		int fd1 = open("/home/wu/wifi",O_RDWR);
		int fd2 = open("/home/wu/wifi",O_RDWR);
		printf("fd1: %d, fd2: %d\n", fd1, fd2);
		ret = flock(fd1,LOCK_SH);
		printf("get lock1, ret: %d\n", ret);
		char* buf="Hello";
		write(fd1,buf,5);
		ret = flock(fd2,LOCK_SH);
		printf("get lock2, ret: %d\n", ret);
		char* buf2="Hello2";
		write(fd2,buf2,6);
}

/*
 * fcntl的使用
 * */
void fun3(){
	struct flock lock;
	lock.l_whence=SEEK_SET;
	lock.l_start=0;
	lock.l_len=0;
	lock.l_type=F_WRLCK;
	int ret;
	int fd=open("/home/wu/wifi",O_RDWR);
	ret=fcntl(fd,F_SETLK,&lock);
	printf("get lock, ret: %d\n", ret);
}

int main(){
	//fun1();
	//fun2();
	fun3();
	return 0;
}

/*
 * 	 建议性锁:就是说它不具备强制性，只是作为程序员之间的约定，如果你愿意，仍然可以直接去
 * 对一个上锁的文件进行操作。可以加多个
 * 	 强制性锁:当一个文件被加上强制性锁后，内核将阻止其他进程对其进行读写操作。只能加一个
 *
 *
 * operation:
 * 	LOCK_SH ：建议性锁
 * 	LOCK_EX ：强制性锁
 * 	LOCK_UN : 解锁
 * 	LOCK_NB：非阻塞（与以上三种操作一起使用）
 *
 * cmd:
 * 	F_SETLK:给文件加上进程锁,非阻塞
 * 	F_SETLKW:给文件加上进程锁,阻塞
 *
 * struct flock{
 * 		short l_type; //加锁类型(F_RDLCK建议性锁,F_WRLCK强制性锁,F_UNLCK解锁)
 * 		short l_whence; //对l_start的解释(SEEK_SET,SEEK_CUR,SEEK_END)
 * 		off_t l_start;//指明锁定的内容位置
 * 		off_t l_len;//加锁的长度，0表示全文
 * 		pid_t l_pid;//加锁的进程id
 * };
 * */
