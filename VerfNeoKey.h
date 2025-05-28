typedef struct LicenseParameters
{
	unsigned char Enable;
	unsigned char SingleInstance;
	unsigned char Netusers;
	unsigned char ExpireDate[8];
	unsigned char Counter[2];
	unsigned char NEOKEYID;
	unsigned char Reserved[2];
}LicenseParameters;

class VerfNeoKey
{
  bool Status;

  int ret;
  int i;
  unsigned char VerfNeoKey::Challenge[17];
  unsigned char VerfNeoKey::HardwareID[9];
  unsigned char Buffer[400];
  unsigned char NKID;
  unsigned char Enable;
  unsigned char SingleInstance;
  unsigned char Netusers;
  unsigned char ExpireDate[8];
  unsigned int Counter;
  int address;
  unsigned int loop;

  struct LicenseParameters MyLic;

public:

  enum KEY_STATUS {KEY_SEM_STATUS,KEY_OK,KEY_AGUARDANDO_INSTALACAO,KEY_INSTALADA_OK,KEY_PAGAMENTO_ATRASADO,KEY_CANCELADA,
                   KEY_NAO_AUTORIZADA,KEY_ID_INVALIDO,ERRO_NEOPROT,ERRO_LER_ID,KEY_STATUS_INVALIDO};

  static std::map<std::string, VerfNeoKey::KEY_STATUS> mapStrSTATUS;
    
  VerfNeoKey(void);
  ~VerfNeoKey(void);

  bool Verifica();
  KEY_STATUS LeStatusChave(std::string& strID);
  VerfNeoKey::KEY_STATUS LeID(std::string& strDado);
  bool GravaStatusChave(KEY_STATUS& Status);
};

