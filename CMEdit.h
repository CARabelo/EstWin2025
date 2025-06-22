#include <afxwin.h>
#include <string>

class CMEdit :  public CEdit
{
  public:

  std::string& Buffer;

  CMEdit(std::string& pBuffer) : Buffer(pBuffer){};
  void FormataPast();

  DECLARE_MESSAGE_MAP()
};

