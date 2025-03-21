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
int end_loop = 1, end_t = 1;
uint64_t mret_start,mret_end;

void my_timer_handler(struct k_timer *dummy)
{
	printf("interrupt\n");
	end_loop = 0;
	ARG_UNUSED(dummy);
}


K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);


void thread1(void *arg1, void *arg2, void *arg3) {
   while(end_t){
		printf("t1 sleeping\n");
		k_sleep(K_MSEC(50));
	}
	printf("t fine\n");
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);
}

static inline uint64_t read_mret(void) {
    uint64_t v3;
    __asm__ volatile ("csrr %0, mhpmcounter3" : "=r"(v3));
    return v3;
}

static inline void enable_pmc(void) {
    uint64_t val;
    __asm__ volatile ("csrrc %0, mcountinhibit, %1" : "=r"(val) : "r"(1 << 3));
}

int main(void) {
	enable_pmc();
	__asm__ volatile ("csrw mhpmevent3, 28");
	
    k_tid_t t1 = k_thread_create(&thread1_data, thread1_stack, STACK_SIZE,
                                   thread1, NULL, NULL, NULL,
                                   THREAD1_PRIORITY, 0, K_NO_WAIT);
	mret_start = read_mret();
	printf("start\n");
	for(int i=0;i<NUM_LOOP;i++){
		printf("loop %d\n",i);
		end_loop = 1;
		k_timer_start(&my_timer, K_MSEC(100), K_NO_WAIT);
		while(end_loop){
			printf("main sleeping\n");
			k_sleep(K_MSEC(50));

		}
	}
	
	end_t = 0;
	k_thread_join(t1,K_FOREVER);
	mret_end = read_mret();
	printf("Stalli mret: %llu\n",mret_end-mret_start);
	z_sys_poweroff();

}
