#ifndef	__IOCTL_INTERNAL_INCLUDE_H__
#define	__IOCTL_INTERNAL_INCLUDE_H__

/* 服务器框架用句柄 */
struct ACL_IOCTL {
	int   event_mode;       /* ACL_EVENT_SELECT/ACL_EVENT_KERNEL */
	int   max_threads;	/* 最大启用线程数 */
	size_t stacksize;	/* 线程的堆栈尺寸 */
	int   idle_timeout;
	int   delay_sec;
	int   delay_usec;
	ACL_WORK_QUEUE *wq;	/* 线程池句柄 */

	ACL_EVENT *event;	/* 事件循环句柄 */
	int   enable_dog;

	ACL_IOCTL_THREAD_INIT_FN thread_init_fn;
	ACL_IOCTL_THREAD_EXIT_FN thread_exit_fn;
	void *thread_init_arg;
	void *thread_exit_arg;
};

typedef struct ACL_IOCTL_CTX {
	ACL_IOCTL *h_ioctl;
	ACL_VSTREAM *h_stream;

	ACL_IOCTL_NOTIFY_FN notify_fn;
	ACL_IOCTL_WORKER_FN worker_fn;
	void  *context;
} ACL_IOCTL_CTX;

/* in ioctl_thr.c */
void read_notify_callback_r(int event_type, void *context);
void write_notify_callback_r(int event_type, void *context);
void listen_notify_callback_r(int event_type, void *context);

/* in ioctl_proc.c */
void read_notify_callback(int event_type, void *context);
void write_notify_callback(int event_type, void *context);
void listen_notify_callback(int event_type, void *context);

#endif
