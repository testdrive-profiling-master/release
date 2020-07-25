#include "StdAfx.h"
#include "DocImp.h"

CDocImp::CDocImp(ITDDocument* pDoc)
{
	m_pDoc = pDoc;
	m_pDoc->GetSystem()->LogOut(_T("CDocImp이 생성되었습니다."));
	m_pDoc->GetButton(_T("run"))->SetManager(this, 0);	// ID 0번 부여	
	m_pBuffer	= m_pDoc->GetBuffer(_T("ABC"));
	m_Const = 1;
	pDoc->AddPropertyData(PROPERTY_TYPE_INT, 1, _T("상수"), (DWORD_PTR)&m_Const, _T("상수 값을 변경합니다."));
}

CDocImp::~CDocImp(void)
{
	m_pDoc->GetSystem()->LogOut(_T("CDocImp이 해제되었습니다."));
}

void CDocImp::OnButtonClick(DWORD dwID){
	m_pDoc->GetSystem()->LogOut(_T("버튼이 클릭 되었습니다."));
	
	DWORD* pB = (DWORD*)m_pBuffer->GetPointer();	// 버퍼 포인터 얻기
	for(int y=0;y<480;y++){
		for(int x=0;x<640;x++){
			pB[x + y * 640] = x*y*m_Const;
		}
	}
	m_pBuffer->Present();	// 그린 결과를 화면에 바로 보인다.
}

BOOL CDocImp::OnPropertyUpdate(ITDPropertyData* pProperty){
	// 생성자에서 속성 ID를 1로 지정했었다.
	if(pProperty->GetID()!=1) return FALSE;
	int data = *(int*)(pProperty->GetData());// 새로운 값을 입력 받는다
	// 값을 1~20으로 제한
	if(data < 1) data =1;
	else if(data >20) data = 20;
	*(int*)(pProperty->GetData()) = data;
	// 수정된 값을 업데이트 한다
	pProperty->UpdateData(FALSE);
	return TRUE;
}

BOOL CDocImp::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam){
	m_Const = (int)command;
	OnButtonClick(0);
	return TRUE;
}