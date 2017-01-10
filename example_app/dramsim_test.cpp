/*********************************************************************************
*  Copyright (c) 2010-2011, Elliott Cooper-Balis
*                             Paul Rosenfeld
*                             Bruce Jacob
*                             University of Maryland 
*                             dramninjas [at] gmail [dot] com
*  All rights reserved.
*  
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*  
*     * Redistributions of source code must retain the above copyright notice,
*        this list of conditions and the following disclaimer.
*  
*     * Redistributions in binary form must reproduce the above copyright notice,
*        this list of conditions and the following disclaimer in the documentation
*        and/or other materials provided with the distribution.
*  
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*********************************************************************************/
int __attribute__((strong)) SHOW_SIM_OUTPUT = true; 

#include <stdio.h>
#include "dramsim_test.h"
#include <inttypes.h>

int reads = 0;
uint64_t last_clk = 0ull;

using namespace DRAMSim;

/* callback functors */
void some_object::read_complete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
	printf("[Callback] read complete: %d 0x%lx cycle=%lu (+%"PRIu64")\n", id, address, clock_cycle, clock_cycle - last_clk);
	reads--;
	last_clk = clock_cycle;
}

void some_object::write_complete(unsigned id, uint64_t address, uint64_t clock_cycle)
{
	printf("[Callback] write complete: %d 0x%lx cycle=%lu (+%"PRIu64")\n", id, address, clock_cycle, clock_cycle - last_clk);
	last_clk = clock_cycle;
}


void some_object::ref_complete(unsigned id, unsigned rank, uint64_t clock_cycle)
{
	printf("[Callback] refresh complete: %d r[%u] cycle=%lu\n", id, rank, clock_cycle);
}

/* FIXME: this may be broken, currently */
void power_callback(double a, double b, double c, double d)
{
//	printf("power callback: %0.3f, %0.3f, %0.3f, %0.3f\n",a,b,c,d);
}

int main()
{
	some_object obj;
	int i,j;
	TransactionCompleteCB *read_cb = new Callback<some_object, void, unsigned, uint64_t, uint64_t>(&obj, &some_object::read_complete);
	TransactionCompleteCB *write_cb = new Callback<some_object, void, unsigned, uint64_t, uint64_t>(&obj, &some_object::write_complete);
	RefCompleteCB *ref_cb = new Callback<some_object, void, unsigned, unsigned, uint64_t>(&obj, &some_object::ref_complete);

	/* pick a DRAM part to simulate */
	MultiChannelMemorySystem *mem = getMemorySystemInstance("ini/DDR3_micron_32M_8B_x4_sg125.ini", "system.ini", "..", "example_app", 2048); 
	mem->RegisterCallbacks(read_cb, write_cb, power_callback, ref_cb);

	printf("dramsim_test main()\n");
	printf("-----MEM1------\n");
	
	for (j = 0; j < 30; j++) {
		for (i = 0; i < 1024; i += 64) {
			mem->addTransaction(false, 0x800000 + i);
			mem->addTransaction(true, 0x800400 + i);
			reads++;
		}
		
		for (i = 0; i < 1024; i += 64) {
			mem->addTransaction(false, 0x808000 + i);
			reads++;
		}
		
		while (reads)
		{
			mem->update();
		}
	}
	
	return 0; 
}

