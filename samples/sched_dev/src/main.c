#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/poweroff.h>

/* Definizione delle dimensioni dello stack per i thread */
#define STACK_SIZE 2048

/* Definizione delle priorità dei thread (valore minore = maggiore priorità) */
#define THREAD1_PRIORITY 5
#define THREAD2_PRIORITY 5
#define NUM_LOOP 10

/* Definizione delle strutture dati per i thread */
K_THREAD_STACK_DEFINE(thread1_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(thread2_stack, STACK_SIZE);
struct k_thread thread1_data;
struct k_thread thread2_data;
int end_loop = 1;

void my_timer_handler(struct k_timer *dummy)
{
	ARG_UNUSED(dummy);
	printf("interrupt\n");
	end_loop = 0;
}

K_TIMER_DEFINE(sleep_t2, my_timer_handler, NULL);


void thread1(void *arg1, void *arg2, void *arg3) {
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);
	printf("start t1\n");
   	while(end_loop){printf("eseguo t1\n");}
	printf("finish t1\n");
}

void thread2(void *arg1,void *arg2, void *arg3){
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);
	printf("start t2\n");
	while(end_loop){printf("eseguo t2\n");}
	printf("finish t2\n");
	/*
	printf("start t2\n");
	k_timer_start(&sleep_t2,K_MSEC(1000),K_NO_WAIT);
	while(end_loop);
	printf("end t2\n");*/
}

int main(void) {
	
    k_tid_t t1 = k_thread_create(&thread1_data, thread1_stack, STACK_SIZE,
								thread1, NULL, NULL, NULL,
								THREAD1_PRIORITY, 0, K_NO_WAIT);
		
	k_tid_t t2 = k_thread_create(&thread2_data, thread2_stack, STACK_SIZE,
                               thread2, NULL, NULL, NULL,
                               THREAD2_PRIORITY, 0, K_NO_WAIT);	
	k_timer_start(&sleep_t2,K_MSEC(15000),K_NO_WAIT);
	k_thread_join(t1,K_FOREVER);
	printf("main finish\n");
	z_sys_poweroff();
}
