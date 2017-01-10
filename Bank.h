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

#ifndef BANK_H
#define BANK_H

//Bank.h
//
//Header file for bank class
//

#include "SystemConfiguration.h"
#include "SimulatorObject.h"
#include "BankState.h"
#include "BusPacket.h"
#include <iostream>

namespace DRAMSim
{

/** Storage space structure for written data
 *
 * The bank class is just a glorified sparse storage data structure
 * that keeps track of written data in case the simulator wants a
 * function DRAM model
 * A vector of size NUM_COLS keeps a linked list of rows and their
 * associated values.
 *
 * @todo If anyone wants to actually store data, see the 'data_storage' branch and perhaps try to merge that into master
 */
class Bank
{
	typedef struct _DataStruct
	{
		unsigned row;
		void *data;
		struct _DataStruct *next;
	} DataStruct;

public:
	//functions
	Bank(ostream &dramsim_log_);
	/** Search for a node with the right row value.
	 * If not found returns the tracer value 0xDEADBEEF.
	 * @param BusPacket Packet containing data and address to read.
	 */
	void read(BusPacket *busPacket);
	/** Add an entry to the proper linked list.
	 * Replaces the value in a row if it was was already written.
	 * @param BusPacket Packet containing data and address to write.
	 */
	void write(const BusPacket *busPacket);

	/** @todo Unused */
	BankState currentState;

private:
	// private member
	std::vector<DataStruct *> rowEntries;
	ostream &dramsim_log; 

	static DataStruct *searchForRow(unsigned row, DataStruct *head);
};
}

#endif

