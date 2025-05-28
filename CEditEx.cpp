#include "stdafx.h"
#include "CEditEx.h"
#include "resource.h"

#define VUE_USRMSG_ROLAR_REGISTRO  WM_APP + 3

BEGIN_MESSAGE_MAP(CEditEx, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BOOL CEditEx::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_TAB)
		{
			CWnd* pFocusControl(GetFocus());

			unsigned long LP(0);

			if (pFocusControl->GetDlgCtrlID() == NumPrimeiroCampo) LP |= 2;

			GetParent()->SendMessage(VUE_USRMSG_ROLAR_REGISTRO, pFocusControl->GetDlgCtrlID(), LP);

			SetSel(0, -1, false);

			return TRUE;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CEditEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditEx::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditEx::OnKillFocus(CWnd* pNewWnd)
{
	NextCtrlID = PrevCtrlID = 0;

	if (pNewWnd != NULL) NextCtrlID = pNewWnd->GetDlgCtrlID();

	CEdit::OnKillFocus(pNewWnd);
}

void CEditEx::OnSetFocus(CWnd* pOldWnd)
{
	PrevCtrlID = NextCtrlID = 0;

	if (pOldWnd != NULL) PrevCtrlID = pOldWnd->GetDlgCtrlID();

	SetSel(0, -1, false);

	CEdit::OnSetFocus(pOldWnd);

}