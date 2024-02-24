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
struct FREQ {
	int freq;
	int count;
	struct FREQ *next;
};
int get_int(char *buf)
{
	if (strlen(buf) == 0) {
		return -1;
	}
	char val[128];
	for (int i = 0; i < 128; i++) {
		if (buf[i] == '\n') {
			break;
		}
		val[i] = buf[i];
		val[i + 1] = 0;
	}
	return atoi(val);
}
char *goto_next_line(char *buf)
{
	for (int i = 0; true; i++) {
		if (buf[i] == '\n') {
			return &buf[i + 1];
		}
	}
}
void add_num(int *array, int num)
{
	for (int i = 0; true; i++) {
		if (array[i] <= 0) {
			array[i] = num;
			array[i + 1] = 0;
			break;
		}
		if (array[i] == num) {
			break;
		}
	}
}
struct FREQ *add_freq(int rate, struct FREQ *freq)
{
	if (rate == -1) {
		return freq;
	}
	struct FREQ **node = &freq;
	while (true) {
		if (*node == NULL) {
			(*node) = (struct FREQ *)malloc(sizeof(struct FREQ));
			(*node)->freq = rate;
			(*node)->count = 1;
			(*node)->next = NULL;
			break;
		}
		if ((*node)->freq == rate) {
			(*node)->count++;
			break;
		}
		node = &((*node)->next);
	}
	return freq;
}
int cmp(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}
struct FREQ *init_freq(struct FREQ *freq, char *buf)
{
	char *ptr = buf;
	int array[128] = { 0 };
	while (get_int(ptr) != -1) {
		ptr = goto_next_line(ptr);
		add_num(array, get_int(ptr));
	}
	qsort(array, 128, sizeof(int), cmp);
	for (int i = 0; i < 128; i++) {
		if (array[i] == 0) {
			continue;
		}
		freq = add_freq(array[i], freq);
	}
	struct FREQ **node = &freq;
	while (true) {
		if (*node == NULL) {
			break;
		}
		(*node)->count = 0;
		node = &((*node)->next);
	}
	return freq;
}
void print_n_char(int n)
{
	for (int i = 0; i <= n; i++) {
		printf("#");
		fflush(stdout);
		usleep(20000);
	}
}
void print_bar(int total, int current)
{
	int barlen = 32;
	barlen = (int)((float)barlen * ((float)current / (float)total));
	print_n_char(barlen);
	printf("\033[45G%5.2f%s\n", ((float)current / (float)total) * 100, "%");
}
void dump_freq(struct FREQ *freq)
{
	int total = 0;
	struct FREQ **node = &freq;
	while (true) {
		if (*node == NULL) {
			break;
		}
		total += (*node)->count;
		node = &((*node)->next);
	}
	node = &freq;
	while (true) {
		if (*node == NULL) {
			break;
		}
		printf("%4dMhz : ", (*node)->freq);
		fflush(stdout);
		print_bar(total, (*node)->count);
		node = &((*node)->next);
	}
}
void log_freq(char *buf)
{
	struct FREQ *freq = init_freq(NULL, buf);
	char *ptr = buf;
	while (get_int(ptr) != -1) {
		ptr = goto_next_line(ptr);
		freq = add_freq(get_int(ptr), freq);
	}
	dump_freq(freq);
}
void read_freq(void)
{
	char *file = "/data/adb/modules/cpulog/log/gpu";
	int fd = open(file, O_RDONLY);
	struct stat filestat;
	fstat(fd, &filestat);
	off_t size = filestat.st_size;
	char *buf = malloc(size + 1);
	read(fd, buf, size);
	close(fd);
	log_freq(buf);
}
int main()
{
	printf("\033[?25l\033[1;38;2;254;228;208m");
	printf("GPU:\n");
	read_freq();
	printf("\n\n");
	printf("\033[?25h\033[0m");
}
