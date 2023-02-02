//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : TestDrive system
// Rev.  : 2/2/2023 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __TESTDRIVER_H__
#define __TESTDRIVER_H__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <tchar.h>

#ifdef USE_VERILATOR
	#include "verilated.h"
	#include "verilated_dpi.h"
#ifdef SIM_TRACE
#ifdef SIM_TRACE_FILE_OUTPUT
	#include "verilated_fst_c.h"
#else
	#include "verilated_vcd_c.h"
	#include "verilated_vcd_gtkwave.h"
#endif
#endif
	#include "verilated_threads.h"
#endif

typedef struct{
	DWORD	dwMemorySize;
	DWORD	UserConfig[1023];
}TESTDRIVE_CONFIG;

struct ITestDriverMemory{
	virtual DWORD GetSize(void) = 0;											// get memory byte size
	virtual BOOL IsValidAddress(DWORD dwAddress) = 0;							// get address validation
	virtual BYTE* GetPointer(DWORD dwAddress = 0, DWORD dwSize = 0) = 0;		// get pointer from address and size definition
	virtual void Flush(DWORD dwAddress, DWORD dwSize) = 0;						// flush paged memory
	virtual TESTDRIVE_CONFIG* GetConfig(void) = 0;								// get configuration
	virtual LPCTSTR GetName(void) = 0;											// get memory name
	virtual void Release(void) = 0;												// release this object
};

ITestDriverMemory* TestDriver_GetMemory(LPCTSTR memory_name = NULL, DWORD dwDefaultByteSize = 0);			// if memory_name is null, 'TESTDRIVE_MEMORY_MAPPED' memory will be returned.

void TestDriver_Cleanup(void);	// must call this at the last

#endif //__TESTDRIVER_H__
