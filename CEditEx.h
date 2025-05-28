#include "afxwin.h"

#define VUE_USRMSG_ROLAR_REGISTRO  WM_APP + 3

class CEditEx :	public CEdit
{
	int NumPrimeiroCampo;
  ittlstwstr ItCampo;
  unsigned long NextCtrlID,PrevCtrlID;

public:

	CEditEx(ittlstwstr pIt,int pNumPrimCampo) : ItCampo(pIt), NumPrimeiroCampo(pNumPrimCampo),NextCtrlID(0),PrevCtrlID(0)  {};
	~CEditEx() {};
   unsigned long GoingToCTrlID() { return NextCtrlID;}
   unsigned long CamingFromCtrlID() { return PrevCtrlID;};

	DECLARE_MESSAGE_MAP()
  void UltimoCampo(ittlstwstr It) { ItCampo  = It;};
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKillFocus(CWnd* pNewWnd);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
};

