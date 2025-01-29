/* main.c - Synchronization demo */

/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/poweroff.h>
//#include <exception.h>

/*
 * The synchronization demo has two threads that utilize semaphores and sleeping
 * to take turns printing a greeting message at a controlled rate. The demo
 * shows both the static and dynamic approaches for spawning a thread; a real
 * world application would likely use the static approach for both threads.
 */

#define PIN_THREADS (IS_ENABLED(CONFIG_SMP) && IS_ENABLED(CONFIG_SCHED_CPU_MASK))

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

/* delay between greetings (in ms) */
#define SLEEPTIME 500

int end_loop = 0;
struct k_thread *main_thread = NULL;

void my_timer_handler(struct k_timer *dummy)
{
	//ARG_UNUSED(dummy);
    //Find arch_esf and check its values;

	uint64_t* sp = ((uint64_t*)dummy->user_data) - 16;
	printf("Timer handler Called\n");
	printf("Arch esf address:%lx\n", (unsigned long)sp);

	printf("ra = %llx\n", sp[ 0]);
	printf("t0 = %llx\n", sp[ 1]);
	printf("t1 = %llx\n", sp[ 2]);
	printf("t2 = %llx\n", sp[ 3]);
	printf("a0 = %llx\n", sp[ 4]);
	printf("a1 = %llx\n", sp[ 5]);
	printf("a2 = %llx\n", sp[ 6]);
	printf("a3 = %llx\n", sp[ 7]);
	printf("a4 = %llx\n", sp[ 8]);
	printf("a5 = %llx\n", sp[ 9]);
	printf("a6 = %llx\n", sp[10]);
	printf("a7 = %llx\n", sp[11]);
	printf("t3 = %llx\n", sp[12]);
	printf("t4 = %llx\n", sp[13]);
	printf("t5 = %llx\n", sp[14]);
	printf("t6 = %llx\n", sp[15]);
	__asm__ volatile (
					"li a0,  0xaaaaaa\n"
					"li a1,  0xaaaaaa\n"
					"li a2,  0xaaaaaa\n"
					"li a3,  0xaaaaaa\n"
					"li a4,  0xaaaaaa\n"
					"li a5,  0xaaaaaa\n"
					"li a6,  0xaaaaaa\n"
					"li a7,  0xaaaaaa\n"
					// "li s2,  0xaaaaaa\n"
					// "li s3,  0xaaaaaa\n"
					// "li s4,  0xaaaaaa\n"
					// "li s5,  0xaaaaaa\n"
					// "li s6,  0xaaaaaa\n"
					// "li s7,  0xaaaaaa\n"
					// "li s8,  0xaaaaaa\n"
					// "li s9,  0xaaaaaa\n"
					// "li s10, 0xaaaaaa\n"
					// "li s11, 0xaaaaaa\n"
					// "li t3,  0xaaaaaa\n"
					// "li t4,  0xaaaaaa\n"
					// "li t5,  0xaaaaaa\n"
				: 
				: 		
				: "a0","a1","a2","a3","a4","a5","a6","a7");	
	//struct arch_esf *esf_ptr =  (struct arch_esf *)main_thread->callee_saved.sp;
	//printf("CUrrent sp: %lx\n",main_thread->callee_saved.sp );
	end_loop = 1;

}

K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

int main(void)
{
	//Start a timer that will interrupt main thread, hopefully saving it's context
	k_timer_start(&my_timer, K_MSEC(1), K_NO_WAIT);
	uint64_t sp;
	__asm__("mv %0,sp\n"
			: "=r" (sp));
	printf("Current sp:%llx\n", (uint64_t)sp);
	//main_thread = _current;
	//void* user_data_addr = &(my_timer.user_data);
	//unsigned long __wv = (unsigned long)(val);		
	__asm__ goto (
					"sd sp, 0(%[user_data_addr])\n"
					"li t0, 5\n"
					"li t1,  6\n"
					"li t2,  7\n"
					"li a0,  10\n"
					"li a1,  11\n"
					"li a2,  12\n"
					"li a3,  13\n"
					"li a4,  14\n"
					"li a5,  15\n"
					"li a6,  16\n"
					"li a7,  17\n"
					"li s2,  18\n"
					"li s3,  19\n"
					"li s4,  20\n"
					"li s5,  21\n"
					"li s6,  22\n"
					"li s7,  23\n"
					"li s8,  24\n"
					"li s9,  25\n"
					"li s10, 26\n"
					"li s11, 27\n"
					"li t3,  28\n"
					"li t4,  29\n"
					"li t5,  30\n"
					"infinite_loop : lw t6, 0(%[end_loop])\n"
					"beqz t6, infinite_loop\n"
					"li t6,  5\n"
					"bne t0,  t6, %l[fail]\n"
					"li t6,  6\n"
					"bne t1,  t6, %l[fail]\n"
					"li t6,  7\n"
					"bne t2,  t6, %l[fail]\n"
					"li t6,  10\n"
					"bne a0,  t6, %l[fail]\n"
					"li t6,  11\n"
					"bne a1,  t6, %l[fail]\n"
					"li t6,  12\n"
					"bne a2,  t6, %l[fail]\n"
					"li t6,  13\n"
					"bne a3,  t6, %l[fail]\n"
					"li t6,  14\n"
					"bne a4,  t6, %l[fail]\n"
					"li t6,  15\n"
					"bne a5,  t6, %l[fail]\n"
					"li t6,  16\n"
					"bne a6,  t6, %l[fail]\n"
					"li t6,  17\n"
					"bne a7,  t6, %l[fail]\n"
					"li t6,  18\n"
					"bne s2,  t6, %l[fail]\n"
					"li t6,  19\n"
					"bne s3,  t6, %l[fail]\n"
					"li t6,  20\n"
					"bne s4,  t6, %l[fail]\n"
					"li t6,  21\n"
					"bne s5,  t6, %l[fail]\n"
					"li t6,  22\n"
					"bne s6,  t6, %l[fail]\n"
					"li t6,  23\n"
					"bne s7,  t6, %l[fail]\n"
					"li t6,  24\n"
					"bne s8,  t6, %l[fail]\n"
					"li t6,  25\n"
					"bne s9,  t6, %l[fail]\n"
					"li t6, 26\n"
					"bne s10, t6, %l[fail]\n"
					"li t6, 27\n"
					"bne s11, t6, %l[fail]\n"
					"li t6,  28\n"
					"bne t3,  t6, %l[fail]\n"
					"li t6,  29\n"
					"bne t4,  t6, %l[fail]\n"
					"li t6,  30\n"
					"bne t5,  t6, %l[fail]\n"
					"j %l[pass]"
				: 
				: [end_loop] "r" (&end_loop), [user_data_addr] "r" (&my_timer.user_data)	
				: "t0","t1","t2","a0","a1","a2","a3","a4","a5","a6","a7","s2","s3","s4","s5","s6","s7","s8","s9","s10","s11","t3","t4","t5","t6","memory"
				: fail, pass); //"memory" probably unneeded			


//let it take s0 and s1
	// __asm__ volatile ("infinite_loop : ld s0, 0(%[end_loop])\n"
	// 				"beqz s0, infinite_loop"		
	// 			: 
	// 			: [end_loop] "r" (&end_loop)			
	// 			: "x3", "x4", "x5"
	// 			); //"memory" probably unneeded			


// infinite_loop:
// 	__asm__ volatile goto ("beqz %[end_loop], %l[infinite_loop]"		
// 				: 
// 				: [end_loop]"r" (end_loop)			
// 				: "x3", "x4", "x5"
// 				: infinite_loop); //"memory" probably unneeded			
fail:
	printf("Error encountered\n");
	sys_poweroff();
pass:
	printf("Main successfully ended\n");
	sys_poweroff();
}