#ifndef __MAIN_H__
#define __MAIN_H__

#define MAX_Q_SIZE  10
#define BUFSIZE 30

enum stats {
	STATS_STOP,
	STATS_ALIVE,
	STATS_RUN
};

void *recv_packet_thread(void *);
void *send_packet_thread(void *);
void clean_thread(void *);

typedef struct thread_arg {
	pthread_t   pt_id;  /* thread id    */
	void * (* func)(void *);
}thread_arg_t;

typedef struct share_data {
//	ipc_msg_t   queue[MAX_Q_SIZE];
	int         idx;        /* last index   */
	int         cnt;        /* item counter */
	unsigned int addr;
	short       port;
	pthread_mutex_t mutex_data;
	pthread_cond_t  cv_data;
}share_data_t ;// *p_shr_data;




#endif
