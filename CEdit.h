#pragma once
#include "afxdialogex.h"


// caixa de diálogo de CEdit

class CEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CEdit)

public:
	CEdit(CWnd* pParent = nullptr);   // construtor padrão
	virtual ~CEdit();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENTRACOORDXY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
};
