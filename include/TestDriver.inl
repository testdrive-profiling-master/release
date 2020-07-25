#include "TestDriver.h"
#include <windows.h>

#ifdef USE_VERILATOR
	#include <shellapi.h>
	#include "verilated.cpp"
	#include "verilated_threads.cpp"
	#include "verilated_dpi.cpp"
	#include "verilated_vcd_c.cpp"
	#include "verilated_fst_c.cpp"
	#include "verilated_vcd_gtkwave.cpp"
#endif

static LPCTSTR			MMFileName		= _T("TESTDRIVE_MEMORY_MAPPED");

class TestDriverMemory :
	public ITestDriverMemory
{
public:
	TestDriverMemory(LPCTSTR memory_name, DWORD dwDefaultByteSize = 0);
	~TestDriverMemory(void);

	STDMETHOD_(DWORD, GetSize)(void);											// get memory byte size
	STDMETHOD_(BOOL, IsValidAddress)(DWORD dwAddress);							// get address validation
	STDMETHOD_(BYTE*, GetPointer)(DWORD dwAddress = 0, DWORD dwByteSize = 0);	// get pointer from address and size definition
	STDMETHOD_(void, Flush)(DWORD dwAddress, DWORD dwSize);						// flush paged memory
	STDMETHOD_(TESTDRIVE_CONFIG*, GetConfig)(void);								// get configuration
	STDMETHOD_(LPCTSTR, GetName)(void);										// get memory name
	STDMETHOD_(void, Release)(void);											// release this object

	inline TestDriverMemory* GetNext(void){return m_pNext;}

	static TestDriverMemory* GetHead(void);

protected:
	TCHAR				m_sName[1024];
	HANDLE				m_hMapHandle;
	DWORD				m_dwByteSize;
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

ITestDriverMemory* TestDriver_GetMemory(LPCTSTR memory_name, DWORD dwDefaultByteSize){
	TestDriverMemory*	pMemory = TestDriverMemory::GetHead();
	if(!memory_name) memory_name = MMFileName;

	// 기존 메모리에서 검색해서 찾기
	while(pMemory){
		if(!_tcscmp(pMemory->GetName(), memory_name)) return pMemory;
		pMemory	= pMemory->GetNext();
	}

	// 없으면 생성 시도
	{
		pMemory = new TestDriverMemory(memory_name, dwDefaultByteSize);
		if(pMemory->IsValidAddress(0)) return pMemory;
		delete pMemory;
	}
	return NULL;
}

TestDriverMemory::TestDriverMemory(LPCTSTR memory_name, DWORD dwDefaultByteSize){
	m_pNext = m_pMemoryHead;
	m_pMemoryHead = this;

	m_dwByteSize	= 0;
	m_hMapHandle	= NULL;
	m_pConfig		= NULL;
	m_pMemoryBase	= NULL;
	*m_sName		= NULL;

	if(memory_name){
		{	// 메모리 핸들 찾기
			m_hMapHandle	= OpenFileMapping(FILE_MAP_ALL_ACCESS, false, memory_name);
			if(!m_hMapHandle){
				if(!dwDefaultByteSize) return;
				
				// 새로 생성
				if(m_hMapHandle = CreateFileMapping(INVALID_HANDLE_VALUE,
					NULL,
					PAGE_READWRITE,
					0,
					dwDefaultByteSize + sizeof(TESTDRIVE_CONFIG),
					memory_name))
				{
					
					TESTDRIVE_CONFIG* pConfig = (TESTDRIVE_CONFIG *)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG)); 
					pConfig->dwMemorySize	= dwDefaultByteSize;
					UnmapViewOfFile(pConfig);
				}else return;
			}
		}
		{	// 메모리 맵핑을 위한 채크
			m_pConfig		= (TESTDRIVE_CONFIG*)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG));
			if(!m_pConfig){
				CloseHandle(m_hMapHandle);
				m_hMapHandle	= NULL;
				return;
			}
			m_dwByteSize	= m_pConfig->dwMemorySize;
			UnmapViewOfFile(m_pConfig);
		}
		{	// 메모리 원래 크기로 할당
			m_pConfig		= (TESTDRIVE_CONFIG*)MapViewOfFile(m_hMapHandle, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TESTDRIVE_CONFIG) + m_dwByteSize);
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
	m_dwByteSize		= 0;
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

DWORD TestDriverMemory::GetSize(void){
	return m_dwByteSize;
}

BOOL TestDriverMemory::IsValidAddress(DWORD dwAddress){
	return (dwAddress < m_dwByteSize);
}

BYTE* TestDriverMemory::GetPointer(DWORD dwAddress, DWORD dwByteSize){
	if(!m_pMemoryBase || (dwAddress+dwByteSize) > m_dwByteSize) return NULL;
	return (m_pMemoryBase + dwAddress);
}

void TestDriverMemory::Flush(DWORD dwAddress, DWORD dwSize){
	BYTE* pMem	= GetPointer(dwAddress, dwSize);
	if(pMem) FlushViewOfFile((LPCVOID)pMem, (SIZE_T)dwSize);
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
