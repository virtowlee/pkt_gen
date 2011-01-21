#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/times.h>

#define _GNU_SOURCE
#include <getopt.h>
#include "ipgen.h"
#include "log.h"
#include "timer.h"
#include "main.h"

#define VERSION "0.1.1 (2001/Mar/16)"

#define SRCPORT 1354
#define DEFAULT_PACKET_SIZE  70
#define DEFAULT_PACKET_COUNT 1

char *program_name;
int break_loop = 0;

share_data_t *p_shr_data = NULL;
thread_arg_t thread_arg[] = {
	{0, recv_packet_thread},
	{0, send_packet_thread},
	{0, NULL}
};


static struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"tcp", required_argument, NULL, 't'},
	{"udp", required_argument, NULL, 'u'},
	{"ip", required_argument, NULL, 'i'},
	{"ping", no_argument, NULL, 'p'},
	{"size", required_argument, NULL, 's'},
	{"count", required_argument, NULL, 'c'},
	{"log", required_argument, NULL, 'l'},
	{"interface", required_argument, NULL, 'I'},
	{NULL, 0, NULL, 0}
};

void usage(int status)
{
	if (status != 0)
		printf("Try `%s --help' for more information.\n",
			 program_name);
	else {
		printf("IPGen : IP packets generator\n");
		printf("Version %s\n", VERSION);
		printf("Leo Liang <leo.liang@china.com>\n\n");

		printf("Usage: program_name [option] ... src_ip dest_ip\n");
		printf("\nIP packets type:\n");
		printf("  -t, --tcp=PORT      TCP\n");
		printf("  -u, --udp=PORT      UDP\n");
		printf("  -i, --ip=PROTO      IP\n");
		printf("  -p, --ping          Ping(ICMP)\n");
		printf("\nPackets size:\n");
		printf("  -s, --size=VALUE    Packet size (default=%d)\n",
			 DEFAULT_PACKET_SIZE);
		printf("  -c, --count=VALUE   How many packets to send "
			 "(ignore when with -t, default=%d)\n",
			 DEFAULT_PACKET_COUNT);
		printf("\nInfomation:\n");
		printf("  -l, --log=LOGFILE   Log to file\n");
		printf("  --help              Show this help\n");
	}
	exit(status);
}


void sigint_handler(int sig)
{
	printf("Good bye !!\n");
	break_loop = 1;
}


int main(int argc, char* argv[])
{
	int packetType = 0;
	int port = 0;
	unsigned char proto = 0;
	int packetSize = DEFAULT_PACKET_SIZE;
	int packetCount = DEFAULT_PACKET_COUNT;
	char* srcHost = NULL;
	char* destHost = NULL;
	char* packetTypeName = NULL;
	int c;
	char *device = NULL;
	int i;

	signal(SIGINT, sigint_handler);

	program_name = argv[0];


	/* parse options */
	while ((c = getopt_long(argc, argv, "ht:u:i:ps:c:l:I:", long_options, NULL)) != -1) {
		switch (c) {
		case 'h':
			usage(0);
			break;
		case 't':
			packetType = IPPROTO_TCP;
			packetTypeName = "TCP";
			port = atoi(optarg);
			break;
		case 'u':
			packetType = IPPROTO_UDP;
			packetTypeName = "UDP";
			port = atoi(optarg);
			break;
		case 'i':
			packetType = IPPROTO_RAW;
			packetTypeName = "IP";
			proto = atoi(optarg);
			break;
		case 'p':
			packetType = IPPROTO_ICMP;
			packetTypeName = "ping(ICMP)";
			break;
		case 's':
			packetSize = atoi(optarg);
			break;
		case 'c':
			packetCount = atoi(optarg);
			break;
		case 'l':
			//OpenLog(optarg, LOG_INFO);
			OpenLog(optarg, LOG_DEBUG);
			break;
		case 'I':
			device = optarg;
			break;
		default:
			usage(1);
		}
	}
	if (packetType == 0)
		usage(1);
	/* should have two more args */
	if (argc-optind != 2)
		usage(1);
	srcHost = argv[optind];
	destHost = argv[optind+1];

	WriteLog(LOG_NOTICE,"--------------------\n");
	if (packetType == IPPROTO_RAW) {
		printf("Send %d IP(proto=%d) packets to %s, packet size is %d bytes\n",
			packetCount, proto, destHost, packetSize);
		WriteLog(LOG_NOTICE, "Send %d IP(proto=%d) packets to %s, packet size is %d bytes\n",
			packetCount, proto, destHost, packetSize);
	} else {
		printf("Send %d %s packets to %s:%d, packet size is %d bytes\n",
			packetCount, packetTypeName, destHost, port, packetSize);
		WriteLog(LOG_NOTICE, "Send %d %s packets to %s:%d, packet size is %d bytes\n",
			packetCount, packetTypeName, destHost, port, packetSize);
	}
	
	CreateRowSocket(packetType, device);
	setuid(getpid());

	for(i=0; thread_arg[i].func != NULL;i++){
		if (pthread_create(&thread_arg[i].pt_id, NULL, thread_arg[i].func, (void *)&thread_arg[i])) {
			fprintf(stdout, "[%s] FAIL: pthread_create()\n", __FUNCTION__);
			return -1;
		}
		printf("[%s] create thread : tid = %lu\n",__FUNCTION__, thread_arg[i].pt_id);
	}

#if 0
	switch (packetType) {
	case IPPROTO_ICMP:
		SendICMP(srcHost, destHost, packetSize, packetCount);
		break;
	case IPPROTO_UDP:
		SendUDP(srcHost ,SRCPORT, destHost, port, packetSize, packetCount);
		break;
	case IPPROTO_TCP:
		SendTCP(srcHost ,SRCPORT, destHost, port, packetSize, packetCount);
		break;
	case IPPROTO_RAW:
		SendIP(srcHost, destHost, proto, packetSize, packetCount);
		break;
	default:
		WriteLog(LOG_WARNING, "Nothing to do, development not finished yet?\n");
	}
#endif

	CloseLog();
	clean_thread(thread_arg);
	return 0;
}	

void 
*recv_packet_thread(void *arg)
{
	thread_arg_t *t_arg = (thread_arg_t *)arg;

	while(!break_loop){

	};

	return NULL;
}

void 
*send_packet_thread(void *arg)
{
	thread_arg_t *t_arg = (thread_arg_t *)arg;
	struct timeval  tm;
	time_t cru_time, last_time;

#if 0
	unsigned long cur_time;
	unsigned long last_time;
	unsigned long m_interval = 1000;	// msec
	unsigned long interval = 1000 * 1000 * 1000;
#endif

	get_cur_time(&last_time);

	gettimeofday(&tm, NULL);
	last_time = tm.tv_sec;

	while(!break_loop){
	
		gettimeofday(&tm, NULL);
		if(tm.tv_sec < (last_time + 1))
			continue;
			
		

#if 0
		get_cur_time(&cur_time);

		if(cur_time < (last_time + interval)){
			continue;
		}
#endif

		printf("cur_time : %d, last_time : %d\n", tm.tv_sec, last_time);
		
		last_time = tm.tv_sec;


	};

	return NULL;
}

void 
clean_thread(void *arg)
{
	thread_arg_t *t_arg = (thread_arg_t *)arg;
	thread_arg_t *t_arg_ret;
	int i;

	for(i=0; thread_arg[i].func != NULL;i++){
		pthread_join(t_arg->pt_id, (void **)&t_arg_ret);
		printf("[%s] clean thread : tid(%lu)\n", __FUNCTION__, thread_arg[i].pt_id);
	}
		
}
