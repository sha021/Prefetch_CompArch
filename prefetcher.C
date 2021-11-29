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

Prefetcher::Prefetcher() { _ready = false; }

bool Prefetcher::hasRequest(u_int32_t cycle) { return _ready; }

Request Prefetcher::getRequest(u_int32_t cycle) { return _nextReq; }

void Prefetcher::completeRequest(u_int32_t cycle) { _ready = false; }

void Prefetcher::cpuRequest(Request req) { 
	if(!_ready) {
		_nextReq.addr = req.addr + 16;
		_ready = true;
	}
}
