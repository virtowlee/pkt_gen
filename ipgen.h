#ifndef _IPGEN_H
#define _IPGEN_H

void CreateRowSocket(int protocal, char *device);
void SendICMP(char* srcHost, char* destHost, int packetSize, int packetCount);
void SendUDP(char* srcHost, int srcPort, char* destHost, int destPort,
				int packetSize, int packetCount);
void SendTCP(char* srcHost, int srcPort, char* destHost, int destPort,
				int packetSize, int packetCount);
void SendIP(char* srcHost, char* destHost, unsigned char proto,
	          int packetSize, int packetCount);

#endif /* ipgen.h */

