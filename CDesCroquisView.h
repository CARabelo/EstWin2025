#pragma once
#include <afxwin.h>
class DesCroquisView : public CView
{
protected:
	DECLARE_DYNCREATE(DesCroquisView);

public:

    DesCroquisView();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* pDC);
};

