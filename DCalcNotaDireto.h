#pragma once
#include "afxcmn.h"

// CDCalcNotaVolumesDireto dialog

class CDCalcNotaVolumesDireto : public CDialog
{
	DECLARE_DYNAMIC(CDCalcNotaVolumesDireto)

public:
	CDCalcNotaVolumesDireto(CWnd* pParent = NULL);   // standard constructor
  BOOL CDCalcNotaVolumesDireto::Create(CWnd* pWnd) { return CDialog::Create(IDD,pWnd);}   //-- Para não modal

	virtual ~CDCalcNotaVolumesDireto();

// Dialog Data
	enum { IDD = IDD_CALCNOTAVOLUMESDIRETO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CProgressCtrl m_Progresso;
};
