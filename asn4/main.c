/* Hadi El-Kadri
November 11, 2020
*/

#include <stdlib.h>
#include <stdbool.h>
#include "scheduling.h"

/*
##  Round-Robin Scheduling Algorithm  ##

	Parameters:-

	queue: 1D int array containing the CPU burst time for every process in a ready queue
	np: number of processes (length of `queue` array)
	tq: time quantum for the RR algorithm


	Returns:-

	rr_result struct pointer, where
	rr_result -> order: 1D int array, containing the execution order of processes (i.e. `(rr_result -> order)[0]` is the process number of the first process)
		** hint: p1 is always the first item in order of selection in round-robin and FCFS
    rr_result -> order_n: int, representing the size of the `order` array, described above ^
	rr_result -> turnarounds: 1D int array, containing the turnaround time for each process (i.e. `(rr_result -> turnarounds)[0]` is the turnaround time for p1)
*/
rr_result *rr(int *queue, int np, int tq)
{
	//Mallocs
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);
	
	//Create decoy queue for calculating size of result->order
	int q[np];
	for (int y = 0; y < np; y++){
	    q[y] = queue[y];
	}
	
	int count = 0;
	
	//Iterate through decoy queue
	for (int z = 0; z < np; z++){
	    bool d = false;

	    while (d == false){
			//Take away tq from element in index until 0 and add to count
	        int result = q[z] - tq;
	        count += 1;
	        if (result <= 0){
	            d = true;
	        }
	        q[z] = q[z] - tq;
	    }
	}

	//Malloc to the size of count after it is found
	result->order = malloc(sizeof(int) * count);
	//Create array to store turn around times
	int ta_times[np];
	//Set result->order_n to count
	result->order_n = count;
	
	//Initialize all values in ta_times array to 0
	for (int h = 0; h < np; h++){
		ta_times[h] = 0;
	}

	bool done = false;
	int count1 = 0;

	//While loop: stay in loop until all values in each index of the queue are 0
	while (done == false){

		done = true;
		
		//Check to see if all values are 0 or not
		for (int k = 0; k < np; k++){
			if(queue[k] > 0){
				done = false;
		}
		}
		
		//If all values in queue are 0 processes are done and exit
		if (done == true) break;
		
		//Iterate through each index in the queue
		for(int i = 0; i < np ; i++){

			//If the burst time is less than the time quantum and greater than 0
			if (queue[i] <= tq && queue[i] > 0){
			    result->order[count1] = i;
			    ta_times[i] = ta_times[i] + queue[i];
			    queue[i] = queue [i] - tq;
			    count1 += 1;

			//Add times to each turnaround time before the elements own
			if (i != 0){
			    
				for (int j = 0; j < (i - 1); j++){
					ta_times[j] = ta_times[j] + queue[i];
				}
				}
			}

			//If the burst time is greater than the time quantum
			if (queue[i] > tq) {
				result->order[count1] = i;
				ta_times[i] = ta_times[i] + tq;
				queue[i] = queue [i] - tq;
				count1 += 1;
				
				//Add the time quantum to each turn around time before the elements own
				if (i != 0){
			
				for (int j = 0; j < (i - 1); j++){
					ta_times[j] = ta_times[j] + tq;
				}
				}
			}

		}
	}

	//Copy all turnaround times from helper array to turnaround array
	for (int p = 0; p < np; p++){
		(result->turnarounds)[p] = ta_times[p];
    
}

	return result;
}
