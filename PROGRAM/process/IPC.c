#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/*
 * 信号:
 * 	 int kill(pid_t,int sig):发送信号sig给指定的进程(pid>0:进程号;pid=0:当前进程组;
 * pid=-1:所有进程;pid<-1进程组的绝对值)
 * 	 int raise(int sig):发送信号sig给当前进程
 * 	 unsigned int alarm(unsigned int seconds):在seconds秒之后安排发送一个SIGALRM信
 * 号给当前进程
 * 	 int pause():把程序的执行挂起直到有一个信号出现为止
 * 	 void (*signal(int sig, void (*handler)(int)))(int):设置信号处理方式(hander:处
 * 理函数;SIG_IGN:忽略处理;SIG_DFL:默认处理)
 * 	 int sigemptyset(sigset_t* set):初始化信号集(清空)
 * 	 int sigfillset(sigset_t* set):初始化信号集(加入所有的信号)
 * 	 int sigaddset(sigset_t* set,int sig):添加一个信号至信号集
 * 	 int sigdelset(sigset_t* set,int sig):从信号集中删除一个信号
 * 	 int sigismember(sigset_t* set,int sig):判断信号集中是否包含信号sig
 * 	 int sigprocmask(int how,sigset_t* set,sigset_t* oldset):用于改变进程的当前阻塞信号集(
 * SIG_UNBLOCK:删除;SIG_BLOCK:追加;SIG_SETMASK:设置)
 * */

void fun1(){
	pid_t pid=fork();
	if(pid==0){
		printf("Child is stop\n");
		raise(SIGSTOP);
		exit(1);
	}else{
		printf("Parent is kill the child\n");
		sleep(5);
		kill(pid,SIGKILL);
		sleep(5);
		exit(1);
	}
}

void deal(){
	printf("You input the ctrl + c\n");
	signal(SIGINT,SIG_DFL);
}

void fun2(){
	signal(SIGINT, deal);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_BLOCK,&set,NULL);
	printf("the process is starting...\n");
	sleep(5);
	printf("the process is stoping...\n");
	sigprocmask(SIG_UNBLOCK,&set,NULL);
}

/*
 * 管道:
 *	 int pipe(int pipefd[2]):创建无名管道文件(pipefd[0]用于读，pipefd[1]用于写)
 *	 int mkfifo(char* name,mode_t mode):创建有名管道文件(并设置权限与creat函数的
 *	 mode相同)，管道文件不许不能存在
 * */

void fun3(){
		int fd[2];
	    pipe(fd);
	    pid_t pid = fork();
	    if(pid > 0){
	        close(fd[0]);  //父进程写管道，需要关闭读端
	        char* str="Hello";
	        write(fd[1],str ,5);
	        close(fd[1]);// 关闭写端
	        sleep(10);
	        exit(1);
	    }else{
	    	close(fd[1]); //子进程读管道，先关闭写端
	    	char str[5];
	    	read(fd[0], str, 5);
	    	close(fd[0]);// 关闭读端
	    	printf("%s\n",str);
	    	exit(1);
	    }
}

void fun4(){
	pid_t pid = fork();
	if(pid > 0){
		mkfifo("pipe",0666);
		int fd=open("pipe", O_WRONLY);
		char* str="Hello";
	    write(fd,str ,5);//父进程写管道
	    close(fd);// 关闭写端
		sleep(5);
		unlink("pipe");
		exit(1);
    }else{
    	int fd=open("pipe", O_RDONLY);
    	char str[5];
    	read(fd, str, 5); //子进程读管道
    	close(fd);// 关闭读端
    	printf("%s\n",str);
		exit(1);
    }
}

/*
 * 消息列队:
 *	 key_t ftok(char* path,int id):建立一个用于IPC通信的ID值，当同一个文件或目录删除后
 *	重建，返回的ID不相同
 *	 int msgget(key_t key,int msgflg):建立消息列队(IPC_CREAT:存在就打开，不存在就创
 *	建，还可以设置创建权限)
 *	 int msgsnd(int msqid,void* msgp,int msgsz,int msgflg):发送消息(msgid是由msgget
 *  函数返回的消息队列标识符；msgp指向准备发送消息的指针；msg_sz是msg_ptr指向的消息的长
 *  度；msgflg:0为阻塞,IPC_NOWAIT:不阻塞)
 *	 int msgrcv(int msqid,void* msgp,int msgsz,long msgtyp,int msgflg):读取消息
 *	(msgtype表示接受mtype值的消息，0表示第一个消息)
 *	 int msgctl(int msgid, int command, struct msgid_ds *buf):控制消息队列(command:
 *	 IPC_RMID:删除消息队列)
 * */

struct msgp{
	long msg_type;
	char msg_text[5];
};

void fun5(){
	pid_t pid=fork();
	if(pid>0){
		key_t key=ftok("./hello",'a');
		printf("Parent:key_t:%d\n",key);
		int qid=msgget(key,IPC_CREAT|0666);
		struct msgp msg;
		msg.msg_type=11;
		strcpy(msg.msg_text,"Hello");
		msgsnd(qid,&msg,5,0);
		sleep(15);
		exit(1);
	}else{
		sleep(10);
		key_t key=ftok("./hello",'a');
		printf("Child:key_t:%d\n",key);
		int qid=msgget(key,IPC_CREAT|0666);
		struct msgp msg;
		msgrcv(qid,&msg,5,11,0);
		printf("msg->msg_type: %ld\n",msg.msg_type);
		printf("msg->msg_text: %s\n",msg.msg_text);
		msgctl(qid,IPC_RMID,NULL);
		exit(1);
	}
}

/*
 * 共享内存:
 *	int shmget(key_t key,int size,int shmflg):创建共享内存，并返回shmid(IPC_CREAT:
 * 存在就打开，不存在就创建，还可以设置创建权限；size:共享内存大小，以字节为单位)
 *	void* shmat(int shmid,void* shmaddr,int shmflg):映射共享内存(shmaddr:共享内存
 * 映射在进程空间的地址，一般为NULL;shmflg:0读写权限，SHM_RDONLY只读)
 *	int shmdt(void* shmaddr):解除共享内存映射
 *	int shmctl(int shm_id, int cmd, struct shmid_ds *buf):共享内存管理(cmd:
 * IPC_RMID:删除共享内存)
 * */

void fun6(){
	pid_t pid=fork();
	if(pid>0){
		key_t key=ftok("./hello",'a');
		int shmid=shmget(key,4096,IPC_CREAT|0666);
		char* addr=(char*)shmat(shmid,NULL,0);
		strcpy( addr, "Hi, I am parent process!\n");
		shmdt(addr);
		sleep(5);
		exit(1);
	}else{
	    key_t key=ftok("./hello",'a');
		int shmid=shmget(key,4096,IPC_CREAT|0666);
		char* addr=(char*)shmat(shmid,NULL,0);
		printf("%s",addr);
		shmctl(shmid,IPC_RMID,NULL);
		exit(1);
	}
}

int main(){
	fun1();
	fun2();
	fun3();
	fun4();
	fun5();
	fun6();
	return 0;
}


/*
 * 	 1.进程间通信(InterProcess Communication,IPC):每个进程各自有不同的进程地址空间，任何
 * 一个进程的全局变量在另一个进程中都不可以被访问，所以进程之间要交换数据必须通过内核，在内
 * 核开辟一块缓冲区，P1进程把数据从用户空间拷到内核的缓存区中，P2进程在从内核缓冲区把数据拷
 * 到用户空间。内核提供的这种机制称为进程间通信。
 *
 * 	 2.通信方式:
 * 	 	a.信号(signal):进程之间可以互相通过系统调用kill来发送信号，内核也可以因为内部事件
 * 	 而给进程发送信号，通知进程发生了某件事件。
 *
 *		常用信号(非实时信号:多次发送，只能作用一次)
 * 	 	SIGHUP  --- 会话关闭(终端关闭) 			  --- 终止
 * 	 	SIGINT  --- 中断(ctrl+c) 		 		  --- 终止
 * 	 	SIGQUIT --- 非正常终止		 			  --- 终止
 * 	 	SIGKILL --- 立即终止，不能阻塞，处理，忽略  --- 终止
 * 	 	SIGALRM --- 由函数alarm发出 				  ---终止
 * 	 	SIGSTOP --- 暂停进程，不能阻塞，处理，忽略  --- 暂停
 * 	 	SIGTSTP --- 暂停进程(ctrl+z) 			  ---暂停
 * 	 	SIGCHLD --- exit函数发出					  ---忽略
 *
 * 	 	b.管道(pipe):一种半双工的通信方式，数据只能单向流动，而且只能在具有亲缘关系(父子，兄弟)
 * 	 的进程间使用。消息存在与内核缓存区中。
 * 	 	c.命名管道(FIFO):一种半双工的通信方式，但它允许无亲缘关系的进程间通信。
 *
 *		 注意:
 *		 	1).管道写入端的引用计数=0，管道读取端的引用计数>0:读取完剩下的数据后就返回0
 *		 	2).管道写入端的引用计数>0(但没有写入数据)，管道读取端的引用计数>0:读取完剩
 *		   下的数据后，read阻塞
 *		    3).管道读取端的引用计数=0，管道写入端的引用计数>0:写入数据会返回SIGPIPE异常终止。
 *		    4).管道读取端的引用计数>0(但没有读取数据)，管道写入端的引用计数>0:当数据缓存区
 *		   写满后，write阻塞
 *
 * 	 	d.消息列队(message queue):一条保存在内核中的消息(具有特定的格式以及特定的优先级)的列表。
 * 	 用户进程可以向消息列队中添加消息，也可以从消息列队中读取消息。
 *
 *		消息结构:
 *		struct msgp{
 *			long msg_type;
 *          //消息
 *		};
 *
 * 	 	e.共享内存(shared memory):当一个程序想和另外一个程序通信的时候。那内存将会为这两个程序
 * 	 生成一块公共的内存区域。这块被两个进程分享的内存区域叫做共享内存。访问共享内存区域和访问进程
 * 	 独有的内存区域一样快，并不须要通过系统调用或者其他须要切入内核的过程来完毕。
 *
 *		 (1).mmap()与munmap()映射方式:各自进程空间内存映射同一个磁盘文件
 * 	 	 (2).System V共享内存:各自进程空间内存映射一个shm文件(存在与内存)
 *
 * 	 	f.信号量(semaphore):信号量是一个计数器，用来控制多进程或多线程对共享资源的访问问题。常作为
 * 	 一种锁机制解决同步问题
 *
 * 	 	h.套接字(socket):允许在不同的机器上的两个不同进程进行通信常用于网络编程
 * */
