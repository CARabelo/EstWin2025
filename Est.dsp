# Microsoft Developer Studio Project File - Name="est" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=est - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Est.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Est.mak" CFG="est - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "est - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "est - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "est - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MTd /W3 /GR /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DISABLE WARNING C4786" /FR /Yu"stdafx.h" /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /D "_DEBUG" /mktyplib203 /win32
# SUBTRACT MTL /nologo
# ADD BASE RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 gts.lib glib.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:"c:\glib" /libpath:"c:\dxf\dxf"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\est___Wi"
# PROP BASE Intermediate_Dir ".\est___Wi"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /Gi /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# SUBTRACT BASE CPP /Gf /Gy
# ADD CPP /nologo /Zp1 /MTd /W3 /Gi /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /mktyplib203 /win32
# ADD BASE RSC /l 0x416 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x416
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /map
# ADD LINK32 glib.lib gts.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:".\release\Est.exe" /libpath:"c:\glib"
# SUBTRACT LINK32 /map /debug

!ENDIF 

# Begin Target

# Name "est - Win32 Debug"
# Name "est - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\AbrirMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AlaSarPasView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\arqnovo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DXF\DXF\CadLib\CadLib\Interface\VC\CadLib.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CAlargamentos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CalcNota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CalculaNota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CalcularMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CalculaTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CalcVolumes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqAlargamentos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqCanteiros.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqCurHorizontais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqCurVerticais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqGradientes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqHPavimento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqIgualdades.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqNivelamento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqOAC.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqOAE.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqPerfis.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqPistasFixas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqSecTipo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqTalAterro.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqTalCorte.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqTaludesPavim.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CArqTerrPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CCanteiros.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CCurHorizontais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CCurVerticais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDeltaSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CDPopUpPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\cedNomeProj.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CEstacasACalcular.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CGradientes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CHPavimento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CIgualdade.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ClassSetPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CListaPerfis.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CListaSecoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CLPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\COAC.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\COACView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\COAE.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ComboProcDesSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\confdesmedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\confproj.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CPistasFixas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CPonto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CriarMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CSectipo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CSStack.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTalAterro.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTalCorte.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTerrPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CTerrPontos.h
# End Source File
# Begin Source File

SOURCE=.\CurHorView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CurVerview.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CVisada.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DCalcGeometrico.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DCalcGeoSeccDireto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DCalcNotaVolumesDireto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DconfDesGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DCoordenadas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DCoresDesGeometrico.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DcoresGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DDeslizantes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEditaCurhoriz.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEntraPonto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEntraPontoGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DeslizantesGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DesPerfilView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DesPontosGeomView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DesSecaoView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DEstacaCota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DGerarDXF.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DGerarDXFGre.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DGerarDXFGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DGerarDXFSecoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DGerarSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Dialogo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DiapontosGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DRotacionar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSecionar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSelGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DSelSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DVolumes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EditPopUp.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\est.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\hlp\est.hpj

!IF  "$(CFG)" == "est - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=Est
InputPath=.\hlp\est.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "est - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.
ProjDir=.
TargetName=Est
InputPath=.\hlp\est.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\est.rc
# End Source File
# Begin Source File

SOURCE=.\Estaca.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\estDoc.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EstRel.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\estView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GeraSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GradienteView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\igualdades.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ListaMedicoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Mat.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MelEditBox.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Monologo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MyToolBar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NivelView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\OAEView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\palette.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PedirProj.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Perfil.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Perfil.h
# End Source File
# Begin Source File

SOURCE=.\projeto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Relatorios.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SecTriangulacao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Splash.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SuperCString.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Superficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TaludesPavim.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TaludesPavimView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TaludesView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TresPontosView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Typlstvw.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\XYGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\xyMouse.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

!ELSEIF  "$(CFG)" == "est - Win32 Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AbrirMedicao.h
# End Source File
# Begin Source File

SOURCE=.\AlaSarPasview.h
# End Source File
# Begin Source File

SOURCE=.\arqnovo.h
# End Source File
# Begin Source File

SOURCE=..\dxf\dxf\cadlib\cadlib\Interface\VC\CadLib.h
# End Source File
# Begin Source File

SOURCE=.\CAlargamentos.h
# End Source File
# Begin Source File

SOURCE=.\calcnota.h
# End Source File
# Begin Source File

SOURCE=.\CalculaMedicao.h
# End Source File
# Begin Source File

SOURCE=.\CalculaNota.h
# End Source File
# Begin Source File

SOURCE=.\CalculaTerreno.h
# End Source File
# Begin Source File

SOURCE=.\CalcVolumes.h
# End Source File
# Begin Source File

SOURCE=.\CArqAlargamentos.h
# End Source File
# Begin Source File

SOURCE=.\CArqCanteiros.h
# End Source File
# Begin Source File

SOURCE=.\carqcurhorizontais.h
# End Source File
# Begin Source File

SOURCE=.\CArqCurVerticais.h
# End Source File
# Begin Source File

SOURCE=.\CArqGradientes.h
# End Source File
# Begin Source File

SOURCE=.\carqhpavimento.h
# End Source File
# Begin Source File

SOURCE=.\CArqIgualdades.h
# End Source File
# Begin Source File

SOURCE=.\carqnivelamento.h
# End Source File
# Begin Source File

SOURCE=.\carqoac.h
# End Source File
# Begin Source File

SOURCE=.\carqoae.h
# End Source File
# Begin Source File

SOURCE=.\carqPerfis.h
# End Source File
# Begin Source File

SOURCE=.\carqsectipo.h
# End Source File
# Begin Source File

SOURCE=.\carqTalAterro.h
# End Source File
# Begin Source File

SOURCE=.\carqtalcorte.h
# End Source File
# Begin Source File

SOURCE=.\cArqTaludesPavim.h
# End Source File
# Begin Source File

SOURCE=.\CArqTerreno.h
# End Source File
# Begin Source File

SOURCE=.\carqterrpontos.h
# End Source File
# Begin Source File

SOURCE=.\CCanteiros.h
# End Source File
# Begin Source File

SOURCE=.\ccurhorizontais.h
# End Source File
# Begin Source File

SOURCE=.\CCurverticais.h
# End Source File
# Begin Source File

SOURCE=.\CDeltaSuperficie.h
# End Source File
# Begin Source File

SOURCE=.\cdpopuppontos.h
# End Source File
# Begin Source File

SOURCE=.\cedNomeProj.h
# End Source File
# Begin Source File

SOURCE=.\CEstacasACalcular.h
# End Source File
# Begin Source File

SOURCE=.\cgradientes.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\chpavimento.h
# End Source File
# Begin Source File

SOURCE=.\cIgualdade.h
# End Source File
# Begin Source File

SOURCE=.\ClassSetPontos.h
# End Source File
# Begin Source File

SOURCE=.\clistaperfis.h
# End Source File
# Begin Source File

SOURCE=.\clistasecoes.h
# End Source File
# Begin Source File

SOURCE=.\CLPontos.h
# End Source File
# Begin Source File

SOURCE=.\coac.h
# End Source File
# Begin Source File

SOURCE=.\COACView.h
# End Source File
# Begin Source File

SOURCE=.\coae.h
# End Source File
# Begin Source File

SOURCE=.\ComboProcDesSecao.h
# End Source File
# Begin Source File

SOURCE=.\Confproj.h
# End Source File
# Begin Source File

SOURCE=.\CPistasFixas.h
# End Source File
# Begin Source File

SOURCE=.\CPonto.h
# End Source File
# Begin Source File

SOURCE=.\CriarMedicao.h
# End Source File
# Begin Source File

SOURCE=.\csecao.h
# End Source File
# Begin Source File

SOURCE=.\csectipo.h
# End Source File
# Begin Source File

SOURCE=.\CSStack.h
# End Source File
# Begin Source File

SOURCE=.\cTalAterro.h
# End Source File
# Begin Source File

SOURCE=.\ctalcorte.h
# End Source File
# Begin Source File

SOURCE=.\cterreno.h
# End Source File
# Begin Source File

SOURCE=.\curhorview.h
# End Source File
# Begin Source File

SOURCE=.\CurVerview.h
# End Source File
# Begin Source File

SOURCE=.\cvisada.h
# End Source File
# Begin Source File

SOURCE=.\DCalcGeometrico.h
# End Source File
# Begin Source File

SOURCE=.\DCalcGeoSeccDireto.h
# End Source File
# Begin Source File

SOURCE=.\DCalcNotaVolumesDireto.h
# End Source File
# Begin Source File

SOURCE=.\DconfDesGeom.h
# End Source File
# Begin Source File

SOURCE=.\DCoordenadas.h
# End Source File
# Begin Source File

SOURCE=.\dcoresdesgeometrico.h
# End Source File
# Begin Source File

SOURCE=.\DcoresGreide.h
# End Source File
# Begin Source File

SOURCE=.\DDeslizantes.h
# End Source File
# Begin Source File

SOURCE=.\DEditaCurhoriz.h
# End Source File
# Begin Source File

SOURCE=.\DEntraPonto.h
# End Source File
# Begin Source File

SOURCE=.\DEntraPontoGreide.h
# End Source File
# Begin Source File

SOURCE=.\DeslizantesGeom.h
# End Source File
# Begin Source File

SOURCE=.\DesPerfilView.h
# End Source File
# Begin Source File

SOURCE=.\DesPontosGeomView.h
# End Source File
# Begin Source File

SOURCE=.\DesSecaoView.h
# End Source File
# Begin Source File

SOURCE=.\DGerarDXF.h
# End Source File
# Begin Source File

SOURCE=.\DGerarDXFGre.h
# End Source File
# Begin Source File

SOURCE=.\DGerarDXFGreide.h
# End Source File
# Begin Source File

SOURCE=.\DGerarDXFSecoes.h
# End Source File
# Begin Source File

SOURCE=.\DGerarSuperficie.h
# End Source File
# Begin Source File

SOURCE=.\Dialogo.h
# End Source File
# Begin Source File

SOURCE=.\DiapontosGeom.h
# End Source File
# Begin Source File

SOURCE=.\DPontoGreide.h
# End Source File
# Begin Source File

SOURCE=..\dxf\dxf\cadlib\cadlib\Interface\VC\DrawingDef.h
# End Source File
# Begin Source File

SOURCE=.\DRotacionar.h
# End Source File
# Begin Source File

SOURCE=.\DSecionar.h
# End Source File
# Begin Source File

SOURCE=.\DSelGreide.h
# End Source File
# Begin Source File

SOURCE=.\DSelSecao.h
# End Source File
# Begin Source File

SOURCE=.\DVolumes.h
# End Source File
# Begin Source File

SOURCE=..\dxf\dxf\cadlib\cadlib\Interface\VC\DXFDef.h
# End Source File
# Begin Source File

SOURCE=.\EditPopUp.h
# End Source File
# Begin Source File

SOURCE=.\est.h
# End Source File
# Begin Source File

SOURCE=.\Estaca.h
# End Source File
# Begin Source File

SOURCE=.\estDoc.h
# End Source File
# Begin Source File

SOURCE=.\EstRel.h
# End Source File
# Begin Source File

SOURCE=.\estView.h
# End Source File
# Begin Source File

SOURCE=.\GeraSuperficie.h
# End Source File
# Begin Source File

SOURCE=.\gradienteview.h
# End Source File
# Begin Source File

SOURCE=.\igualdades.h
# End Source File
# Begin Source File

SOURCE=.\ListaMedicoes.h
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\Mat.h
# End Source File
# Begin Source File

SOURCE=.\MelEditBox.h
# End Source File
# Begin Source File

SOURCE=.\Monologo.h
# End Source File
# Begin Source File

SOURCE=.\MyToolBar.h
# End Source File
# Begin Source File

SOURCE=.\NivelView.h
# End Source File
# Begin Source File

SOURCE=.\OAEView.h
# End Source File
# Begin Source File

SOURCE=.\palette.h
# End Source File
# Begin Source File

SOURCE=.\PedirProj.h
# End Source File
# Begin Source File

SOURCE=.\projeto.h
# End Source File
# Begin Source File

SOURCE=.\Relatorios.h
# End Source File
# Begin Source File

SOURCE=.\resource.hm
# End Source File
# Begin Source File

SOURCE=.\SecTriangulacao.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE=.\SuperCString.h
# End Source File
# Begin Source File

SOURCE=.\Superficie.h
# End Source File
# Begin Source File

SOURCE=.\TaludesPavim.h
# End Source File
# Begin Source File

SOURCE=.\taludespavimview.h
# End Source File
# Begin Source File

SOURCE=.\Taludesview.h
# End Source File
# Begin Source File

SOURCE=.\trespontosview.h
# End Source File
# Begin Source File

SOURCE=.\typedefs.h
# End Source File
# Begin Source File

SOURCE=.\typlstvw.h
# End Source File
# Begin Source File

SOURCE=.\XYGreide.h
# End Source File
# Begin Source File

SOURCE=.\xyMouse.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\RES\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\RES\desenhos.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\desgreid.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dessecao.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Est.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_estt.ico
# End Source File
# Begin Source File

SOURCE=.\RES\imagelis.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\imglis.bmp
# End Source File
# Begin Source File

SOURCE=.\res\projetos.bmp
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\subtitul.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\toolbar1.bmp
# End Source File
# End Group
# End Target
# End Project
# Section est : {005DD440-0000-0000-0000-000000000000}
# 	1:10:IDB_SPLASH:114
# 	2:21:SplashScreenInsertKey:4.0
# End Section
