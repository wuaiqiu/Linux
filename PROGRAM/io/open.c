#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

/*
 * 不带缓存文件的IO操作:
 * 		不带缓冲的I/O是指进程不提供缓冲功能（但内核还是提供缓冲的）。每调用一次write或read
 * 函数，直接系统调用
 *
 *	  int creat(char* pathname,mode_t mode):创建文件并设置权限
 *	  int open(char* pathname,int flags):打开或创建文件,并使用指定权限访问
 *	  int open(char* pathname,int flags,mode_t mode):打开或创建文件,并使用
 *	  指定权限访问,并指定新增文件权限
 *	  ssize_t read(int fd,void* buf,size_t count):读取文件，STDIN_FILENO为控制台输入流
 *	  ssize_t write(int fd,void* buf,size_t count):写入文件,count与buf需要相同的字数，STDOUT_FILENO为控制台输出流
 *	  _off_t lseek(int fd,_off_t offset ,int whence):移动读/写指针offset个字节
 *	  int close(int fd):关闭文件
 * */

int main(){
	int dest=open("/home/wu/a.txt",O_RDWR|O_CREAT|O_EXCL,S_IWUSR|S_IRUSR);
	int src=open("/home/wu/wifi",O_RDONLY);
	char buf[10];
	int count;
	lseek(src,2,SEEK_SET);
	while((count=read(src,buf,10))>0){
		write(dest,buf,count);
	}
	close(src);
	close(dest);
	return 0;
}


/*
 * mode_t:
 * 	S_IRUSR:所有者具有读取权限
 * 	S_IWUSR:所有者具有写入权限
 * 	S_IXUSR:所有者具有执行权限
 * 	S_IRGRP:组具有读取权限
 * 	S_IWGRP:组具有写入权限
 * 	S_IXGRP:组具有执行权限
 * 	S_IROTH:其他用户具有读取权限
 * 	S_IWOTH:其他用户具有写入权限
 * 	S_IXOTH:其他用户具有执行权限
 *
 * flags:
 * 	O_RDONLY:以只读方式打开
 * 	O_WRONLY:以写入方式打开
 * 	O_RDWR:以读写方式打开
 * 	O_APPEND:以追加方式打开
 * 	O_TRUNG:清空文件
 * 	O_NONBLOCK:非阻塞读取/写入文件,报错(需要轮询查看)
 * 	O_CREAT:开启mode参数
 * 	O_EXCL:与O_CREAT一起使用，当文件存在创建失败
 *
 * ssize_t:long int
 *
 * _off_t: long int
 *
 * whence:
 * 	SEEK_SET:以文件头为基准(正数)
 * 	SEEK_CUR:以目前指针为基准(正数与负数)
 * 	SEEK_END:以文件尾为基准(负数)
 * */
