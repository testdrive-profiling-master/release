#pragma once
#include "testdrive.h"

class CDocImp : public TDImplDocumentBase,
				public ITDButtonManager
{
public:
	CDocImp(ITDDocument* pDoc);
	~CDocImp(void);

	STDMETHOD_(void, OnButtonClick)(DWORD dwID);
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData* pProperty);
	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);

	ITDDocument*	m_pDoc;
	ITDBuffer*		m_pBuffer;

	int				m_Const;
};
