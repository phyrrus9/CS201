#define _GNU_SOURCE
#define __USE_GNU
#include <stdio.h>
#include <openssl/sha.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <errno.h>

#define CLKS clk1 = clock()
#define CLKP clk2 = clock()
#define GETCLK(a, b) ((clk2 - clk1) / (CLOCKS_PER_SECOND))
#define NCORES 4

int semaphore = 0;
int cores[NCORES] = { 0 };

struct hashentry
{
	char string[34];
	char outbuf[34];
};

struct threadparams
{
	int len;
	pthread_t threadid;
};

int stick_this_thread_to_core(int core_id)
{
#ifdef __APPLE__
	return 0;
#else
	int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
	if (core_id >= num_cores)  // core_id = 0, 1, ... n-1 if system has n cores
		return EINVAL;
	
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);
	
	pthread_t current_thread = pthread_self();
	return pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
#endif
}

int select_core()
{
	int i;
	for (i = 0; i < NCORES; i++)
	{
		if (cores[i] == 0)
		{
			cores[i] = 1;
			return i;
		}
	}
	return -1;
}

void sha256(char *string, char *outputBuffer, int len)
{
	unsigned char digest[SHA256_DIGEST_LENGTH];
	int i;
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, string, strlen(string));
	SHA256_Final(digest, &ctx);
	for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(&outputBuffer[i*2], "%02x", (unsigned int)digest[i]);
}

void all_combinations__( char* x, int len, FILE *f )
{
	char c;
	for (c = 32; c < 123; ++c)
	{
		x[len] = c;
		if (len <= 0)
		{
			struct hashentry entry;
			memcpy(entry.string, x, 33);
			sha256(x, entry.outbuf, len);
			entry.outbuf[33] = 0;
			fwrite(&entry, sizeof(struct hashentry), 1, f);
			fflush(f);
		}
		else
			all_combinations__(x, len - 1, f);
	}
}

void *getcombinations(void *a)
{
	struct threadparams *p = (struct threadparams *)a;
	int len = p->len;
	pthread_t threadid = p->threadid;
#ifndef __APPLE__
	int core = select_core();
#endif
	printf("Thread started: %d\n", len);
	char filename[30];
	char x[65];
	sprintf(filename, "%d.bin", len);
	FILE *f = fopen(filename, "wb");
	semaphore++;
#ifndef __APPLE__
	printf("Got thread with threadid %d, binding to core %d\n", threadid, core);
	stick_this_thread_to_core(core);
#endif
	all_combinations__(x, len, f );
	semaphore--;
#ifndef __APPLE__
	cores[core] = 0;
#endif
	fclose(f);
	printf("\tThread %d completed!\n", len);
	return NULL;
}

int main(int argc, char * * argv)
{
	char string[65] = {0};
	FILE *f;
	int len, endlen = atoi(argv[1]);
	int i, j, k;
	char filename[30];
	pthread_t *threads = malloc(sizeof(pthread_t) * endlen + 3);
	pthread_t threadid;
	if (argc < 3)
		len = 0;
	else
		len = atoi(argv[2]);
	while (len < endlen)
	{
		if (semaphore < 4)
		{
			int tmplen = len;
			struct threadparams sender;
			sender.len = len;
			pthread_create(&sender.threadid, NULL, &getcombinations, &sender);
			sleep(1);
			++len;
		}
	}
	while (semaphore);
	printf("\r%30s\rDone!\n", " ");
}
