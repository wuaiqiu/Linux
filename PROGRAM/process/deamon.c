#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/stat.h>

/*
 * 	int setpgid(pid_t pid, pid_t pgid):将一个进程加入到本会话中的另一个进程组，PID是要操
 * 作的进程，0代表本进程；PGID是要加入的进程组，0代表要加入的进程组的PGID是这个进程的PID。
 * 会话头进程不能改变自己所在的进程组。
 * 	pid_t getpgid(pid_t pid):获得指定进程所在组的组ID。
 * 	pid_t getpgrp():获得自己所在组的ID。
 * 	pid_t setsid():设置新的会话SID,将GID与SID全部换成PID.
 *	pid_t getsid(pid_t pid):获取指定进程所在的会话ID。
 *	pid_t tcgetpgrp(int fd):获取前台进程组ID
 *	int tcsetpgrp(int fd ,pid_t pgrpid):将前台进程组ID设置为pgrpid
 *
 * */

/*
 * 孤儿进程组
 * */
void fun1(){
	pid_t pid=fork();
	if(pid==0){
		printf("1.child  PPID: %d ,PID: %d, GID: %d, SID: %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		sleep(10);
		setpgid(0,0);
		printf("2.child  PPID: %d ,PID: %d, GID: %d, SID: %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		exit(1);
	}else{
		printf("1.parent  PID: %d, GID: %d, SID: %d\n",getpid(),getpgrp(),getsid(getpid()));
		sleep(2);
		exit(1);
	}
}

/*
 * 守护进程
 * */
void fun2(){
	pid_t pid=fork();
	if(pid>0){
		printf("1.parent  PID: %d, GID: %d, SID: %d\n",getpid(),getpgrp(),getsid(getpid()));
		sleep(2);
		exit(1); //1.父进程退出
	}else{
		printf("1.child  PPID: %d ,PID: %d, GID: %d, SID: %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		sleep(5);
		setsid();//2.设置新会话
		printf("2.child  PPID: %d ,PID: %d, GID: %d, SID: %d\n",getppid(),getpid(),getpgrp(),getsid(getpid()));
		chdir("/tmp");//3.改变工作目录
		umask(0);//4.重设文件掩码
		for(int i=0;i<NOFILE;i++)
			close(i);  //5.关闭文件描述符
		sleep(10);
	}
}

int main(){
	fun1();
	fun2();
	return 0;
}


/*
 *	 1.进程组:每一个进程都属于一个"进程组"，当一个进程被创建的时候，它默认是其父进程所在组的
 *	成员。一个进程的组ID(PGID)等于这个组的第一个成员(也称为进程组长)。
 *
 *	 2.会话:内核将几个进程组并为一个"会话"。会话的ID就是通过setsid()启动这个会话的进程的
 *	PID(也就是这个会话的第一个进程，通常是用户的shell)，这个进程也称为"会话头"，它随后产生
 *	的所有子孙进程都默认在这个会话里。
 *
 *	 3.前台进程组:每一个会话最多有一个进程组是"前台进程组"，控制终端会将输入和信号传给该进程组
 *	的成员。
 *
 *	 4.后台进程组:一个会话中，除前台进程组外的进程组都称为"后台进程组"，这些后台进程组的进程
 *	不参与终端的输入输出。去读控制终端时，终端会向后台作业发送一个特定的信号SIGTTIN。该信号
 *	通常会暂时停止此后台作业
 *
 *	 5.孤儿进程组:该进程组中每一个进程的父进程都属于另一个session，孤儿进程组是后台进程组，且
 *	没有控制终端，向孤儿进程组每一个进程发送挂断信号(SIGHUP终止进程)，接着又向暂停进程发送继续
 * 	运行信号(SIGCONT)。
 * */
