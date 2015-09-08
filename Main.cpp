
#include "Bridge.h"

#include <iostream>
#include <sys/wait.h>

using namespace std;

int main(int argc, char* argv[]){

	int maxweight;
	Bridge theBridge;

	if(argc != 2){
		cout<<"Error: Max weight not entered";
		return 0;
	}
	else{
		maxweight = atoi(argv[1]);
		cout<<endl<<"Bridge max weight is: "<<maxweight<<endl;
	}

	if(!theBridge.createSemaphoreAndSharedSegment())
		return 0;

	string licensePlate;
	int timeSinceLastTruck;
	int truckWeight;
	int timeToCross;
	int pid;
	int orderTicket = -1;

	while(cin>>licensePlate){
		cin>>timeSinceLastTruck;
		cin>>truckWeight;
		cin>>timeToCross;

		sleep(timeSinceLastTruck);

		cout<<licensePlate<<" arrives at bridge with load weight "<<truckWeight<<endl;

		if(truckWeight > maxweight){
			cout<<licensePlate<< " rejected, from bridge for being too heavy"<<endl;
		}
		else{
			orderTicket += 1;

			pid=fork();
			if (pid == 0){

				while(!theBridge.enter_Bridge(truckWeight, maxweight, licensePlate, orderTicket));

				sleep(timeToCross);

				theBridge.leave_Bridge(truckWeight, maxweight, licensePlate);

				_exit(0);
			}
		}
	}

	//parent waiting
	while(wait(0)!=-1);

	theBridge.closeSemaphoreAndSharedSegment();
	cout<<"Bridge closed"<<endl;

	return 0;
}
