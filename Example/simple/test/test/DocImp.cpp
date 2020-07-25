#include "StdAfx.h"
#include "DocImp.h"

CDocImp::CDocImp(ITDDocument* pDoc)
{
	m_pDoc = pDoc;
	m_pDoc->GetSystem()->LogOut(_T("CDocImp�� �����Ǿ����ϴ�."));
	m_pDoc->GetButton(_T("run"))->SetManager(this, 0);	// ID 0�� �ο�	
	m_pBuffer	= m_pDoc->GetBuffer(_T("ABC"));
	m_Const = 1;
	pDoc->AddPropertyData(PROPERTY_TYPE_INT, 1, _T("���"), (DWORD_PTR)&m_Const, _T("��� ���� �����մϴ�."));
}

CDocImp::~CDocImp(void)
{
	m_pDoc->GetSystem()->LogOut(_T("CDocImp�� �����Ǿ����ϴ�."));
}

void CDocImp::OnButtonClick(DWORD dwID){
	m_pDoc->GetSystem()->LogOut(_T("��ư�� Ŭ�� �Ǿ����ϴ�."));
	
	DWORD* pB = (DWORD*)m_pBuffer->GetPointer();	// ���� ������ ���
	for(int y=0;y<480;y++){
		for(int x=0;x<640;x++){
			pB[x + y * 640] = x*y*m_Const;
		}
	}
	m_pBuffer->Present();	// �׸� ����� ȭ�鿡 �ٷ� ���δ�.
}

BOOL CDocImp::OnPropertyUpdate(ITDPropertyData* pProperty){
	// �����ڿ��� �Ӽ� ID�� 1�� �����߾���.
	if(pProperty->GetID()!=1) return FALSE;
	int data = *(int*)(pProperty->GetData());// ���ο� ���� �Է� �޴´�
	// ���� 1~20���� ����
	if(data < 1) data =1;
	else if(data >20) data = 20;
	*(int*)(pProperty->GetData()) = data;
	// ������ ���� ������Ʈ �Ѵ�
	pProperty->UpdateData(FALSE);
	return TRUE;
}

BOOL CDocImp::OnCommand(DWORD command, WPARAM wParam, LPARAM lParam){
	m_Const = (int)command;
	OnButtonClick(0);
	return TRUE;
}