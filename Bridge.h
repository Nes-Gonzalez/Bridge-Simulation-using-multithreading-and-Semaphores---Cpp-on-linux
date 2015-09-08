
#ifndef BRIDGE_H
#define BRIDGE_H

#define MEMSIZE 4

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class Bridge {
	public:
		bool createSemaphoreAndSharedSegment();
		bool enter_Bridge(int, int, string, int);
		bool leave_Bridge(int, int, string);
		bool closeSemaphoreAndSharedSegment();
	private:
		long semaphoreKey;
		int semaphoreID;
		long semaphoreKey2;
		int semaphoreID2;
		int *currentBridgeWeigth;
		int *bridgeLettingIn;
		sem_t *bridge;
		char sem_bridge[];
		int maxweight;

};
#endif
