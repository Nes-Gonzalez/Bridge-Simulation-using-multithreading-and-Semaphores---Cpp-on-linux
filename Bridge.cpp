
#include "Bridge.h"


using namespace std;

bool Bridge::createSemaphoreAndSharedSegment(){
	stpcpy(sem_bridge,"nes_bridge");

	bridge = sem_open(sem_bridge, O_CREAT, 0600, 1);
	if(bridge == SEM_FAILED){
		perror("unable to create nes_bridge semaphore");
		return false;
	}


	semaphoreKey = 77788443;
	semaphoreID = shmget(semaphoreKey, MEMSIZE, 0600 | IPC_CREAT);

	if(semaphoreID == -1){
		perror("Cannot get shared segment");
		sem_close(bridge);
		sem_unlink(sem_bridge);
		return false;
	}

	currentBridgeWeigth = (int*) shmat (semaphoreID, NULL, 0);
	if(currentBridgeWeigth == NULL){
		cout<<"Error creating shard segment";
		sem_close(bridge);
		if(sem_unlink(sem_bridge) != 0){
			cout<< "Error: semaphore "<< sem_bridge << "was not deleted"<< endl;
		}
		return false;
	}

	semaphoreKey2 = 2144675;
	semaphoreID2 = shmget(semaphoreKey2, MEMSIZE, 0600 | IPC_CREAT);
	if(semaphoreID2 == -1){
		perror("Cannot get shared segment");
		sem_close(bridge);
		sem_unlink(sem_bridge);
		return false;
	}

	bridgeLettingIn = (int*) shmat (semaphoreID2, NULL, 0);
	if(bridgeLettingIn == NULL){
		cout<<"Error creating shard segment";
		sem_close(bridge);
		if(sem_unlink(sem_bridge) != 0){
			cout<< "Error: semaphore "<< sem_bridge << "was not deleted"<< endl;
		}
		return false;
	}

	return true;

}

bool Bridge::enter_Bridge(int truckWeight,int maxweight, string licensePlate, int orderTicket){

	int testWeight = 0;

	sem_wait(bridge);

	testWeight = *currentBridgeWeigth + truckWeight;
	if(testWeight > maxweight || *bridgeLettingIn != orderTicket){
		sem_post(bridge);
		return false;
	}
	else{
		*currentBridgeWeigth += truckWeight;

		cout<<licensePlate<<" enters bridge, current bridge weight: "<<*currentBridgeWeigth<<endl;
		*bridgeLettingIn += 1;
		sem_post(bridge);
		return true;
	}
}

bool Bridge::leave_Bridge(int truckWeight,int maxweight, string licensePlate){
	sem_wait(bridge);
	*currentBridgeWeigth -= truckWeight;
	cout<<licensePlate<<" leaves bridge, current bridge weight: "<<*currentBridgeWeigth<<endl;
	sem_post(bridge);
	return true;
}

bool Bridge::closeSemaphoreAndSharedSegment(){

	shmdt(currentBridgeWeigth);
	shmdt(bridgeLettingIn);
	shmctl(semaphoreID, IPC_RMID, NULL);
	shmctl(semaphoreID2, IPC_RMID, NULL);
	sem_close(bridge);
	if(sem_unlink(sem_bridge) != 0){
		cout<< "Error: semaphore "<< sem_bridge << "was not deleted"<< endl;
		return false;
	}
	return true;
}
