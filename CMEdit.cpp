#include "stdafx.h"
#include "resource.h"
#include <string>
#include "CMEdit.h"

#define CTRL_V 0x16

BEGIN_MESSAGE_MAP(CMEdit, CEdit)
END_MESSAGE_MAP()

void CMEdit::FormataPast()
{
  int nLineCount(GetLineCount());
  CString strText, strLine;

  // Dump every line of text of the edit control.

  Buffer.clear();

  for (int i = 0; i < nLineCount; i++)
  {
    int len(LineLength(LineIndex(i)));
    GetLine(i, strText.GetBuffer(len), len);
    strText.ReleaseBuffer(len);
    Buffer += strText.GetBuffer();
    if ((i + 1) != nLineCount) Buffer += "\r\n";

    AFXDUMP(strLine);
  }
}