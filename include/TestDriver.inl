//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#include "TestDriver.h"

#ifdef USE_VERILATOR
	#include <shellapi.h>
	#include "verilated.cpp"
	#include "verilated_threads.cpp"
	#include "verilated_dpi.cpp"
#ifdef SIM_TRACE
#ifdef SIM_TRACE_FILE_OUTPUT
	#include "verilated_fst_c.cpp"
#else
	#include "verilated_vcd_c.cpp"
	#include "verilated_vcd_gtkwave.cpp"
#endif
#endif
#endif

static LPCTSTR			MMFileName		= _T("TESTDRIVE_MEMORY_MAPPED");

class TestDriverMemory :
	public ITestDriverMemory
{
public:
	TestDriverMemory(LPCTSTR memory_name, uint64_t dwDefaultByteSize = 0);
	~TestDriverMemory(void);

	virtual uint64_t GetSize(void);												// get memory byte size
	virtual BOOL IsValidAddress(uint64_t ulAddress);							// get address validation
	virtual BYTE* GetPointer(uint64_t ulAddress = 0, uint64_t ulSize = 0);		// get pointer from address and size definition
	virtual void Flush(uint64_t ulAddress, uint64_t ulSize);					// flush paged memory
	virtual TESTDRIVE_CONFIG* GetConfig(void);									// get configuration
	virtual LPCTSTR GetName(void);												// get memory name
	virtual void Release(void);													// release this object

	inline TestDriverMemory* GetNext(void){return m_pNext;}

	static TestDriverMemory* GetHead(void);

protected:
	TCHAR				m_sName[1024];
	HANDLE				m_hMapHandle;
	uint64_t			m_ulByteSize;
	TestDriverMemory*	m_pNext;
	TESTDRIVE_CONFIG*	m_pConfig;
	BYTE*				m_pMemoryBase;

	static TestDriverMemory*	m_pMemoryHead;
};

void TestDriverMemory_ReleaseAll(void);

TestDriverMemory*	TestDriverMemory::m_pMemoryHead	= NULL;

void TestDriverMemory_ReleaseAll(void){
	while(TestDriverMemory::GetHead()) delete TestDriverMemory::GetHead();
}

TestDriverMemory* TestDriverMemory::GetHead(void){
	return m_pMemoryHead;
}

ITestDriverMemory* TestDriver_GetMemory(LPCTSTR memory_name, uint64_t ulDefaultByteSize){
	TestDriverMemory*	pMemory = TestDriverMemory::GetHead();
	if(!memory_name) memory_name = MMFileName;

	// Find by searching in existing memory
	while(pMemory){
		if(!_tcscmp(pMemory->GetName(), memory_name)) return pMemory;
		pMemory	= pMemory->GetNext();
	}

	// If not, try to create
	{
		pMemory = new TestDriverMemory(memory_name, ulDefaultByteSize);
		if(pMemory->IsValidAddress(0)) return pMemory;
		delete pMemory;
	}
	return NULL;
}

TestDriverMemory::TestDriverMemory(LPCTSTR memory_name, uint64_t ulDefaultByteSize){
	m_pNext = m_pMemoryHead;
	m_pMemoryHead = this;

	m_ulByteSize	= 0;
	m_hMapHandle	= NULL;
	m_pConfig		= NULL;
	m_pMemoryBase	= NULL;
	*m_sName		= NULL;

	if(memory_name){
		{	// Find memory handle
			uint64_t		mapped_size = ulDefaultByteSize + sizeof(TESTDRIVE_CONFIG);
			m_hMapHandle	= OpenFileMapping(FILE_MAP_ALL_ACCESS, false, memory_name);
			if(!m_hMapHandle){
				if(!ulDefaultByteSize) return;
				
				// newly create
				if(m_hMapHandle = CreateFileMapping(INVALID_HANDLE_VALUE,
					NULL,
					PAGE_READWRITE,
					(mapped_size >> 32) & 0xFFFFFFFF,
					mapped_size & 0xFFFFFFFF,
					memory_name))
				{
					
					TESTDRIVE_CONFIG* pConfig = (TESTDRIVE_CONFIG *)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG)); 
					pConfig->ulMemorySize	= ulDefaultByteSize;
					UnmapViewOfFile(pConfig);
				}else return;
			}
		}
		{	// Check for memory mapping
			m_pConfig		= (TESTDRIVE_CONFIG*)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG));
			if(!m_pConfig){
				CloseHandle(m_hMapHandle);
				m_hMapHandle	= NULL;
				return;
			}
			m_ulByteSize	= m_pConfig->ulMemorySize;
			UnmapViewOfFile(m_pConfig);
		}
		{	// Allocate the original size of memory
			m_pConfig		= (TESTDRIVE_CONFIG*)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG) + m_ulByteSize);
			m_pMemoryBase	= ((BYTE*)m_pConfig) + sizeof(TESTDRIVE_CONFIG);
		}
		_tcscpy(m_sName, memory_name);
	}
}

TestDriverMemory::~TestDriverMemory(void){
	if(m_pMemoryHead == this){
		m_pMemoryHead = m_pNext;
	}else{
		TestDriverMemory* pLink = m_pMemoryHead;
		while(pLink){
			if(pLink->m_pNext == this){
				pLink->m_pNext = m_pNext;
				break;
			}
			pLink = pLink->m_pNext;
		}
	}
	m_pNext				= NULL;
	m_ulByteSize		= 0;
	m_pMemoryBase		= NULL;
	if(m_pConfig){
		UnmapViewOfFile(m_pConfig);
		m_pConfig		= NULL;
	}
	if(m_hMapHandle){
		CloseHandle(m_hMapHandle);
		m_hMapHandle	= NULL;
	}
}

void TestDriverMemory::Release(void){
	delete this;
}

uint64_t TestDriverMemory::GetSize(void){
	return m_ulByteSize;
}

BOOL TestDriverMemory::IsValidAddress(uint64_t ulAddress){
	return (ulAddress < m_ulByteSize);
}

BYTE* TestDriverMemory::GetPointer(uint64_t ulAddress, uint64_t dwByteSize){
	if(!m_pMemoryBase || (ulAddress+dwByteSize) > m_ulByteSize) return NULL;
	return (m_pMemoryBase + ulAddress);
}

void TestDriverMemory::Flush(uint64_t ulAddress, uint64_t ulSize){
	BYTE* pMem	= GetPointer(ulAddress, ulSize);
	if(pMem) FlushViewOfFile((LPCVOID)pMem, ulSize);
}

TESTDRIVE_CONFIG* TestDriverMemory::GetConfig(void){
	return m_pConfig;
}

LPCTSTR TestDriverMemory::GetName(void){
	return m_sName;
}

void TestDriver_Cleanup(void){
	TestDriverMemory_ReleaseAll();
}
