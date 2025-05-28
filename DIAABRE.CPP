/*
void arqnovo::OnProcurar() 
{
  CString ext,nomearq,filtro;
  
  if (Nome.IsEmpty()) nomearq = "*";
  else nomearq = Nome;

  filtro  = "Todos os arquivos de dados - "+ nomearq + " | " + nomearq + ".igu; " + nomearq +".niv; ";
  filtro += nomearq + ".ver; " + nomearq + ".hor; " + nomearq + ".lim; " + nomearq + ".pav; ";
  filtro += nomearq + ".tco; " + nomearq + ".taa; " + nomearq + ".tpa; " + nomearq + ".sec; ";
  filtro += nomearq + ".oac; " + nomearq + ".oae; " + nomearq + ".can; " + nomearq + ".tpo; ";
  filtro += nomearq + ".fix; | ";
  filtro += "Igualdades ( " + nomearq + ".igu) | " + nomearq + ".igu; |"; 
  filtro += "Nivelamento ( " + nomearq + ".niv) | " + nomearq + ".niv; |"; 
  filtro += "Curvas verticais ( " + nomearq + ".ver) | " + nomearq + ".ver; |"; 
  filtro += "Curvas Horizontais ( " + nomearq + ".hor) | " + nomearq + ".hor; |"; 
  filtro += "Limpeza ( " + nomearq + ".lim) | " + nomearq + ".lim; |"; 
  filtro += "Espessuras do pavimento ( " + nomearq + ".pav) | " + nomearq + ".pav; |"; 
  filtro += "Taludes de corte ( " + nomearq + ".tco) | " + nomearq + ".tco; |"; 
  filtro += "Taludes de aterro( " + nomearq + ".taa) | " + nomearq + ".taa; |"; 
  filtro += "Taludes do pavimento ( " + nomearq + ".tpa) | " + nomearq + ".tpa; |"; 
  filtro += "Seções tipo do pavimento ( " + nomearq + ".sec) | " + nomearq + ".sec; |"; 
  filtro += "Espessuras do pavimento ( " + nomearq + ".pav) | " + nomearq + ".pav; |"; 
  filtro += "O.A.C ( " + nomearq + ".oac) | " + nomearq + ".oac; |"; 
  filtro += "O.A.E ( " + nomearq + ".oae) | " + nomearq + ".oae; |"; 
  filtro += "Canteiros centrais ( " + nomearq + ".can) | " + nomearq + ".can; |"; 
  filtro += "Terreno por pontos ( " + nomearq + ".tpo) | " + nomearq + ".tpo; |"; 
  filtro += "O.A.C ( " + nomearq + ".oac) | " + nomearq + ".oac; |"; 
  filtro += "Pistas fixas ( " + nomearq + ".fix) | " + nomearq + ".fix; ||";

  CFileDialog diaabrearq(TRUE,NULL,NULL,OFN_EXPLORER | OFN_FILEMUSTEXIST,filtro,NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir arquivo :";
  if (diaabrearq.DoModal() == IDOK)
  {
    nomearq = diaabrearq.GetPathName();
    nomearq.MakeUpper();
    if (!nomearq.IsEmpty())
    {
      ext = diaabrearq.GetFileExt();
      ext.MakeUpper();
      if (!(ext == "IGU" || ext == "NIV" || ext == "VER" || ext == "HOR" || ext == "LIM" ||
            ext == "PAV" || ext == "TCO" || ext == "TAA" || ext == "TPA" || ext == "SEC" ||
            ext == "OAC" || ext == "OAE" || ext == "CAN" || ext == "TPO" || ext == "FIX"))
        monolog.mensagem(4,nomearq); //--- Erro, arquivo inválido.
    }
  }
}
*/
