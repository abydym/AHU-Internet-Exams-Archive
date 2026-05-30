#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_THREADS 8
#define ARRAY_SIZE 1000000

pthread_rwlock_t rwlock; // 读写锁
int array[ARRAY_SIZE]; // 数组

void *reader(void *arg) { // 读者线程函数
	int i, j;
	for (i = 0; i < 10000; i++) {
		pthread_rwlock_rdlock(&rwlock); // 获取读锁
		j = rand() % ARRAY_SIZE;
		printf("Reader %d: array[%d] = %d\n", *(int *)arg, j, array[j]);
		pthread_rwlock_unlock(&rwlock); // 释放读锁
	}
	return NULL;
}

void *writer(void *arg) { // 写者线程函数
	int i, j;
	for (i = 0; i < 1000; i++) {
		pthread_rwlock_wrlock(&rwlock); // 获取写锁
		j = rand() % ARRAY_SIZE;
		array[j] = i;
		printf("Writer %d: array[%d] = %d\n", *(int *)arg, j, array[j]);
		pthread_rwlock_unlock(&rwlock); // 释放写锁
	}
	return NULL;
}

int main() {
	pthread_t threads[NUM_THREADS]; // 线程数组
	int thread_ids[NUM_THREADS]; // 线程ID数组
	int i;
	struct timeval start, end;
	double elapsed_time;
	
	pthread_rwlock_init(&rwlock, NULL); // 初始化读写锁
	
	for (i = 0; i < ARRAY_SIZE; i++) {
		array[i] = i;
	}
	
	gettimeofday(&start, NULL); // 记录开始时间
	
	for (i = 0; i < NUM_THREADS; i++) {
		thread_ids[i] = i + 1;
		if (i % 2 == 0) {
			pthread_create(&threads[i], NULL, reader, &thread_ids[i]); // 创建读者线程
		} else {
			pthread_create(&threads[i], NULL, writer, &thread_ids[i]); // 创建写者线程
		}
	}
	
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL); // 等待所有线程结束
	}
	
	gettimeofday(&end, NULL); // 记录结束时间
	elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
	
	printf("Elapsed time: %lf seconds\n", elapsed_time);
	
	pthread_rwlock_destroy(&rwlock); // 销毁读写锁
	
	return 0;
}
