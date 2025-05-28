#include <afxwin.h>
#include <string>

class CMEdit :  public CEdit
{
  std::string& Buffer;

public:
  CMEdit(std::string& pBuffer) : Buffer(pBuffer){};
  void FormataPast();

  DECLARE_MESSAGE_MAP()
};

