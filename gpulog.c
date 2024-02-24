#include <linux/stat.h>
#include <unistd.h>
#include <linux/limits.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#if __STDC_VERSION__ < 202000L
#define bool _Bool
#define true ((_Bool) + 1u)
#define false ((_Bool) + 0u)
#endif
void init(void)
{
	system("rm -rf /data/adb/modules/cpulog/log");
	mkdir("/data/adb/modules/cpulog/log", S_IRGRP | S_IROTH | S_IRUSR | S_IWGRP | S_IWOTH | S_IWUSR);
}
void log_gpu_freq(void)
{
	char *path = "/sys/kernel/gpu/gpu_clock";
	int fd = open(path, O_RDONLY);
	char buf[32];

	size_t len = read(fd, buf, sizeof(buf));
	buf[len] = 0;
	close(fd);

	char *logpath = "/data/adb/modules/cpulog/log/gpu";
	fd = open(logpath, O_APPEND | O_WRONLY | O_CREAT, S_IRGRP | S_IROTH | S_IRUSR | S_IWGRP | S_IWOTH | S_IWUSR);
	write(fd, buf, strlen(buf));

	close(fd);
}
int main()
{
	init();
	while (true) {
		log_gpu_freq();
		sleep(3);
	}
}
