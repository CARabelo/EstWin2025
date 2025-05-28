// CEdit.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "afxdialogex.h"
#include "CEdit.h"


// caixa de diálogo de CEdit

IMPLEMENT_DYNAMIC(CEdit, CDialogEx)

CEdit::CEdit(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ENTRACOORDXY, pParent)
{

}

CEdit::~CEdit()
{
}

void CEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEdit, CDialogEx)
END_MESSAGE_MAP()


// manipuladores de mensagens de CEdit
