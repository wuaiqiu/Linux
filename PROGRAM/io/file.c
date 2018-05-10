#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * int stat(char* fileName,struct stat* stat):获取文件属性
 * int lstat(char* fileName,struct stat* stat):与stat的区别是，lstat返回为链接文件本
 * 身，stat返回文件实体
 * int fstat(int fd, struct stat *stat):获取文件属性
 * */

int main(){
	struct stat s1,s2,s3;
	stat("/home/wu/hello",&s1);
	lstat("/home/wu/hello",&s2);
	int fd=open("/home/wu/hello",O_RDWR);
	fstat(fd,&s3);
	printf("%d\n%d\n%d",s1.st_size,s2.st_size,s3.st_size);
	return 0;
}

/*
 * struct stat{
 * 		dev_t st_dev;	//文件所在设备ID
 * 		ino_t st_ino;	//索引节点号
 * 		mode_t st_mode; //文件保护模式
 * 		nlink_t st_nlink; //文件硬链接数
 * 		uid_t st_uid; //用户ID
 * 		gid_t st_gid; //组ID
 * 		dev_t st_rdev; //设备号,针对设备文件
 * 		off_t st_size; //文件字节数
 * 		unsigned long st_blksize; //系统块大小
 * 		unsigned long st_blocks; //文件所占的块数
 *		time_t st_atime; //最后一次访问的时间
 *		time_t st_mtime; //最后一次修改的时间
 *		time_t st_ctime; //最后一次改变的时间
 * }
 * */
