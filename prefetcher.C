// #include "prefetcher.h"
// #include "mem-sim.h"

// bool Prefetcher::hasRequest(u_int32_t cycle) {
// 	return false;
// }

// Request Prefetcher::getRequest(u_int32_t cycle) {
// 	Request req;
// 	return req;
// }

// void Prefetcher::completeRequest(u_int32_t cycle) { return; }

// void Prefetcher::cpuRequest(Request req) { return; }


#include "prefetcher.h"
#include "mem-sim.h"
#include <stdio.h>
#include <iostream>

Prefetcher::Prefetcher() { 
	rptidx = 0;
	waitingidx = 0;
}

bool Prefetcher::hasRequest(u_int32_t cycle) { 
	return waitingidx;
}

Request Prefetcher::getRequest(u_int32_t cycle) { 
	waitingidx--;
	return waiting[0];
}

void Prefetcher::completeRequest(u_int32_t cycle) { }

void Prefetcher::cpuRequest(Request req) { 
	/* If not in RPT */
	u_int32_t req_tag = req.addr >> 15;
	std::vector <int> idx;

	// Getting index of corresponding 
	for (int i=0; i<RPT_SIZE; i++){
		if (rpt[i].tag == req_tag){  // Found tag in RPT
			idx.push_back(i);
		} 
	}

	int matchSize = idx.size();
	u_int32_t cur_addr = req.addr;
	bool existItem = false;

	// No matching Tag
	if (matchSize<1){
		rpt[rptidx].tag = req_tag;
		rpt[rptidx].address = cur_addr;
		rpt[rptidx].stride = 4;
		rpt[rptidx].state = 0;
		rptidx = (rptidx + 1) % RPT_SIZE;

		for (int i=0; i<20; i++){
			if (waiting[i].addr == (cur_addr+16)) existItem = true;
		}

		if (waitingidx < 20){
			if (!existItem){
				waiting[waitingidx].addr = cur_addr + 16;	
				waitingidx++;	
			}
		}
	}

	// Tag matches
	else{
		for (int i=0; i<matchSize; i++){
	// Check for address - stride = previous address
			u_int32_t stride_new = cur_addr - rpt[i].address;
			// stride stays remain
			if (rpt[i].state==0){
				rpt[i].stride = stride_new;
				rpt[i].state = 1;
				rpt[i].address = cur_addr;
			}

			else if (stride_new == rpt[i].stride){
				// update rpt
				rpt[i].address = cur_addr;
				if (rpt[i].state==1 || rpt[i].state==2) rpt[i].state = 2;
				else if (rpt[i].state==3) rpt[i].state = 1;

				if (waitingidx <20){
					if (rpt[i].state==2){
						for (int i=0; i<20; i++){
							if (waiting[i].addr == (cur_addr + rpt[i].stride)) existItem = true;
						}

						if (!existItem){
							waiting[waitingidx].addr = cur_addr + rpt[i].stride;	
							waitingidx++;		
						}
					
					}
					else{
						for (int i=0; i<20; i++){
							if (waiting[i].addr == (cur_addr+16)) existItem = true;
						}

						if (!existItem){
							waiting[waitingidx].addr = cur_addr + 16;	
							waitingidx++;
						}
								
					}	
				}
			}
			// stride changed
			else{
				for (int i=0; i<20; i++){
					if (waiting[i].addr == (cur_addr+16)) existItem = true;
				}

				if (!existItem){
					waiting[waitingidx].addr = cur_addr + 16;	
					waitingidx++;
				}
				if (rpt[i].state == 3 || rpt[i].state == 2) {
					rpt[rptidx].tag = req_tag;
					rpt[rptidx].stride = stride_new;
					rpt[rptidx].address = cur_addr;
					rpt[i].state = 3;
					rptidx = (rptidx + 1) % RPT_SIZE;
				}
				else if (rpt[i].state == 1){
					rpt[i].address = cur_addr;
					rpt[i].state = 3;
					rpt[i].stride = stride_new;
				}
			}
		}
	}
}


