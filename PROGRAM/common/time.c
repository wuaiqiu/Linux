#include <stdio.h>
#include <time.h>
/*
 * time_t time(time_t *t):返回时间戳
 * tm* gmtime(time_t *t):将时间戳转化成格林尼治时间
 * char* asctime(tm* tm):将日期和时间格式化为字符串
 * tm* localtime(time_t *timep):取得当地的时间和日期
 * */
int main(){
	time_t timep;
	struct tm *p;
	time(&timep);
	printf("%s",asctime(gmtime(&timep)));
	p=localtime(&timep);
	printf("%d年%d月%d日",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
	printf("%d:%d:%d",p->tm_hour,p->tm_min,p->tm_sec);
	return 0;
}

/*
 * time_t:为一个long int
 *
 * struct tm{
 * 		int tm_sec;  //秒数(0-59)
 * 		int tm_min;  //分钟(0-59)
 * 		int tm_hour; //小时(0-23)
 * 		int tm_mday; //日数(1-31)
 * 		int tm_mon;  //月份(0-11)
 * 		int tm_year; //从1900年起至今的年数
 * 		int tm_wday; //星期(0-6)
 * 		int tm_yday; //天数(0-365)
 * 		int tm_isdst; //夏时制时间
 * }
 * */