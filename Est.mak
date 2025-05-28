# Microsoft Developer Studio Generated NMAKE File, Based on Est.dsp
!IF "$(CFG)" == ""
CFG=est - Win32 Debug
!MESSAGE No configuration specified. Defaulting to est - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "est - Win32 Debug" && "$(CFG)" != "est - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "est - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\Debug\Est.hlp" "$(OUTDIR)\Est.exe" "$(OUTDIR)\Est.bsc"


CLEAN :
	-@erase "$(INTDIR)\AbrirMedicao.obj"
	-@erase "$(INTDIR)\AbrirMedicao.sbr"
	-@erase "$(INTDIR)\AlaSarPasView.obj"
	-@erase "$(INTDIR)\AlaSarPasView.sbr"
	-@erase "$(INTDIR)\arqnovo.obj"
	-@erase "$(INTDIR)\arqnovo.sbr"
	-@erase "$(INTDIR)\CadLib.obj"
	-@erase "$(INTDIR)\CadLib.sbr"
	-@erase "$(INTDIR)\CAlargamentos.obj"
	-@erase "$(INTDIR)\CAlargamentos.sbr"
	-@erase "$(INTDIR)\CalcNota.obj"
	-@erase "$(INTDIR)\CalcNota.sbr"
	-@erase "$(INTDIR)\CalculaNota.obj"
	-@erase "$(INTDIR)\CalculaNota.sbr"
	-@erase "$(INTDIR)\CalcularMedicao.obj"
	-@erase "$(INTDIR)\CalcularMedicao.sbr"
	-@erase "$(INTDIR)\CalculaTerreno.obj"
	-@erase "$(INTDIR)\CalculaTerreno.sbr"
	-@erase "$(INTDIR)\CalcVolumes.obj"
	-@erase "$(INTDIR)\CalcVolumes.sbr"
	-@erase "$(INTDIR)\CArqAlargamentos.obj"
	-@erase "$(INTDIR)\CArqAlargamentos.sbr"
	-@erase "$(INTDIR)\CArqCanteiros.obj"
	-@erase "$(INTDIR)\CArqCanteiros.sbr"
	-@erase "$(INTDIR)\CArqCurHorizontais.obj"
	-@erase "$(INTDIR)\CArqCurHorizontais.sbr"
	-@erase "$(INTDIR)\CArqCurVerticais.obj"
	-@erase "$(INTDIR)\CArqCurVerticais.sbr"
	-@erase "$(INTDIR)\CArqGradientes.obj"
	-@erase "$(INTDIR)\CArqGradientes.sbr"
	-@erase "$(INTDIR)\CArqHPavimento.obj"
	-@erase "$(INTDIR)\CArqHPavimento.sbr"
	-@erase "$(INTDIR)\CArqIgualdades.obj"
	-@erase "$(INTDIR)\CArqIgualdades.sbr"
	-@erase "$(INTDIR)\CArqNivelamento.obj"
	-@erase "$(INTDIR)\CArqNivelamento.sbr"
	-@erase "$(INTDIR)\CArqOAC.obj"
	-@erase "$(INTDIR)\CArqOAC.sbr"
	-@erase "$(INTDIR)\CArqOAE.obj"
	-@erase "$(INTDIR)\CArqOAE.sbr"
	-@erase "$(INTDIR)\CArqPerfis.obj"
	-@erase "$(INTDIR)\CArqPerfis.sbr"
	-@erase "$(INTDIR)\CArqPistasFixas.obj"
	-@erase "$(INTDIR)\CArqPistasFixas.sbr"
	-@erase "$(INTDIR)\CArqSecTipo.obj"
	-@erase "$(INTDIR)\CArqSecTipo.sbr"
	-@erase "$(INTDIR)\CArqTalAterro.obj"
	-@erase "$(INTDIR)\CArqTalAterro.sbr"
	-@erase "$(INTDIR)\CArqTalCorte.obj"
	-@erase "$(INTDIR)\CArqTalCorte.sbr"
	-@erase "$(INTDIR)\CArqTaludesPavim.obj"
	-@erase "$(INTDIR)\CArqTaludesPavim.sbr"
	-@erase "$(INTDIR)\CArqTerreno.obj"
	-@erase "$(INTDIR)\CArqTerreno.sbr"
	-@erase "$(INTDIR)\CArqTerrPontos.obj"
	-@erase "$(INTDIR)\CArqTerrPontos.sbr"
	-@erase "$(INTDIR)\CCanteiros.obj"
	-@erase "$(INTDIR)\CCanteiros.sbr"
	-@erase "$(INTDIR)\CCurHorizontais.obj"
	-@erase "$(INTDIR)\CCurHorizontais.sbr"
	-@erase "$(INTDIR)\CCurVerticais.obj"
	-@erase "$(INTDIR)\CCurVerticais.sbr"
	-@erase "$(INTDIR)\CDeltaSuperficie.obj"
	-@erase "$(INTDIR)\CDeltaSuperficie.sbr"
	-@erase "$(INTDIR)\CDPopUpPontos.obj"
	-@erase "$(INTDIR)\CDPopUpPontos.sbr"
	-@erase "$(INTDIR)\CEstacasACalcular.obj"
	-@erase "$(INTDIR)\CEstacasACalcular.sbr"
	-@erase "$(INTDIR)\CGradientes.obj"
	-@erase "$(INTDIR)\CGradientes.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\CHPavimento.obj"
	-@erase "$(INTDIR)\CHPavimento.sbr"
	-@erase "$(INTDIR)\CIgualdade.obj"
	-@erase "$(INTDIR)\CIgualdade.sbr"
	-@erase "$(INTDIR)\ClassSetPontos.obj"
	-@erase "$(INTDIR)\ClassSetPontos.sbr"
	-@erase "$(INTDIR)\CListaPerfis.obj"
	-@erase "$(INTDIR)\CListaPerfis.sbr"
	-@erase "$(INTDIR)\CListaSecoes.obj"
	-@erase "$(INTDIR)\CListaSecoes.sbr"
	-@erase "$(INTDIR)\CLPontos.obj"
	-@erase "$(INTDIR)\CLPontos.sbr"
	-@erase "$(INTDIR)\COAC.obj"
	-@erase "$(INTDIR)\COAC.sbr"
	-@erase "$(INTDIR)\COACView.obj"
	-@erase "$(INTDIR)\COACView.sbr"
	-@erase "$(INTDIR)\COAE.obj"
	-@erase "$(INTDIR)\COAE.sbr"
	-@erase "$(INTDIR)\ComboProcDesSecao.obj"
	-@erase "$(INTDIR)\ComboProcDesSecao.sbr"
	-@erase "$(INTDIR)\confdesmedicao.obj"
	-@erase "$(INTDIR)\confdesmedicao.sbr"
	-@erase "$(INTDIR)\confproj.obj"
	-@erase "$(INTDIR)\confproj.sbr"
	-@erase "$(INTDIR)\CPistasFixas.obj"
	-@erase "$(INTDIR)\CPistasFixas.sbr"
	-@erase "$(INTDIR)\CPonto.obj"
	-@erase "$(INTDIR)\CPonto.sbr"
	-@erase "$(INTDIR)\CriarMedicao.obj"
	-@erase "$(INTDIR)\CriarMedicao.sbr"
	-@erase "$(INTDIR)\CSecao.obj"
	-@erase "$(INTDIR)\CSecao.sbr"
	-@erase "$(INTDIR)\CSectipo.obj"
	-@erase "$(INTDIR)\CSectipo.sbr"
	-@erase "$(INTDIR)\CSStack.obj"
	-@erase "$(INTDIR)\CSStack.sbr"
	-@erase "$(INTDIR)\CTalAterro.obj"
	-@erase "$(INTDIR)\CTalAterro.sbr"
	-@erase "$(INTDIR)\CTalCorte.obj"
	-@erase "$(INTDIR)\CTalCorte.sbr"
	-@erase "$(INTDIR)\CTerreno.obj"
	-@erase "$(INTDIR)\CTerreno.sbr"
	-@erase "$(INTDIR)\CTerrPontos.obj"
	-@erase "$(INTDIR)\CTerrPontos.sbr"
	-@erase "$(INTDIR)\CurHorView.obj"
	-@erase "$(INTDIR)\CurHorView.sbr"
	-@erase "$(INTDIR)\CurVerview.obj"
	-@erase "$(INTDIR)\CurVerview.sbr"
	-@erase "$(INTDIR)\CVisada.obj"
	-@erase "$(INTDIR)\CVisada.sbr"
	-@erase "$(INTDIR)\DCalcGeometrico.obj"
	-@erase "$(INTDIR)\DCalcGeometrico.sbr"
	-@erase "$(INTDIR)\DCalcGeoSeccDireto.obj"
	-@erase "$(INTDIR)\DCalcGeoSeccDireto.sbr"
	-@erase "$(INTDIR)\DCalcNotaVolumesDireto.obj"
	-@erase "$(INTDIR)\DCalcNotaVolumesDireto.sbr"
	-@erase "$(INTDIR)\DconfDesGeom.obj"
	-@erase "$(INTDIR)\DconfDesGeom.sbr"
	-@erase "$(INTDIR)\DCoordenadas.obj"
	-@erase "$(INTDIR)\DCoordenadas.sbr"
	-@erase "$(INTDIR)\DCoresDesGeometrico.obj"
	-@erase "$(INTDIR)\DCoresDesGeometrico.sbr"
	-@erase "$(INTDIR)\DcoresGreide.obj"
	-@erase "$(INTDIR)\DcoresGreide.sbr"
	-@erase "$(INTDIR)\DDeslizantes.obj"
	-@erase "$(INTDIR)\DDeslizantes.sbr"
	-@erase "$(INTDIR)\DEditaCurhoriz.obj"
	-@erase "$(INTDIR)\DEditaCurhoriz.sbr"
	-@erase "$(INTDIR)\DEntraPonto.obj"
	-@erase "$(INTDIR)\DEntraPonto.sbr"
	-@erase "$(INTDIR)\DEntraPontoGreide.obj"
	-@erase "$(INTDIR)\DEntraPontoGreide.sbr"
	-@erase "$(INTDIR)\DeslizantesGeom.obj"
	-@erase "$(INTDIR)\DeslizantesGeom.sbr"
	-@erase "$(INTDIR)\DesPerfilView.obj"
	-@erase "$(INTDIR)\DesPerfilView.sbr"
	-@erase "$(INTDIR)\DesPontosGeomView.obj"
	-@erase "$(INTDIR)\DesPontosGeomView.sbr"
	-@erase "$(INTDIR)\DesSecaoView.obj"
	-@erase "$(INTDIR)\DesSecaoView.sbr"
	-@erase "$(INTDIR)\DEstacaCota.obj"
	-@erase "$(INTDIR)\DEstacaCota.sbr"
	-@erase "$(INTDIR)\DGerarDXF.obj"
	-@erase "$(INTDIR)\DGerarDXF.sbr"
	-@erase "$(INTDIR)\DGerarDXFGre.obj"
	-@erase "$(INTDIR)\DGerarDXFGre.sbr"
	-@erase "$(INTDIR)\DGerarDXFGreide.obj"
	-@erase "$(INTDIR)\DGerarDXFGreide.sbr"
	-@erase "$(INTDIR)\DGerarDXFSecoes.obj"
	-@erase "$(INTDIR)\DGerarDXFSecoes.sbr"
	-@erase "$(INTDIR)\DGerarSuperficie.obj"
	-@erase "$(INTDIR)\DGerarSuperficie.sbr"
	-@erase "$(INTDIR)\Dialogo.obj"
	-@erase "$(INTDIR)\Dialogo.sbr"
	-@erase "$(INTDIR)\DiapontosGeom.obj"
	-@erase "$(INTDIR)\DiapontosGeom.sbr"
	-@erase "$(INTDIR)\DRotacionar.obj"
	-@erase "$(INTDIR)\DRotacionar.sbr"
	-@erase "$(INTDIR)\DSecionar.obj"
	-@erase "$(INTDIR)\DSecionar.sbr"
	-@erase "$(INTDIR)\DSelGreide.obj"
	-@erase "$(INTDIR)\DSelGreide.sbr"
	-@erase "$(INTDIR)\DSelSecao.obj"
	-@erase "$(INTDIR)\DSelSecao.sbr"
	-@erase "$(INTDIR)\DVolumes.obj"
	-@erase "$(INTDIR)\DVolumes.sbr"
	-@erase "$(INTDIR)\EditPopUp.obj"
	-@erase "$(INTDIR)\EditPopUp.sbr"
	-@erase "$(INTDIR)\est.obj"
	-@erase "$(INTDIR)\Est.pch"
	-@erase "$(INTDIR)\est.res"
	-@erase "$(INTDIR)\est.sbr"
	-@erase "$(INTDIR)\Estaca.obj"
	-@erase "$(INTDIR)\Estaca.sbr"
	-@erase "$(INTDIR)\estDoc.obj"
	-@erase "$(INTDIR)\estDoc.sbr"
	-@erase "$(INTDIR)\EstRel.obj"
	-@erase "$(INTDIR)\EstRel.sbr"
	-@erase "$(INTDIR)\estView.obj"
	-@erase "$(INTDIR)\estView.sbr"
	-@erase "$(INTDIR)\GeraSuperficie.obj"
	-@erase "$(INTDIR)\GeraSuperficie.sbr"
	-@erase "$(INTDIR)\GradienteView.obj"
	-@erase "$(INTDIR)\GradienteView.sbr"
	-@erase "$(INTDIR)\igualdades.obj"
	-@erase "$(INTDIR)\igualdades.sbr"
	-@erase "$(INTDIR)\ListaMedicoes.obj"
	-@erase "$(INTDIR)\ListaMedicoes.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\Mat.obj"
	-@erase "$(INTDIR)\Mat.sbr"
	-@erase "$(INTDIR)\MelEditBox.obj"
	-@erase "$(INTDIR)\MelEditBox.sbr"
	-@erase "$(INTDIR)\Monologo.obj"
	-@erase "$(INTDIR)\Monologo.sbr"
	-@erase "$(INTDIR)\MyToolBar.obj"
	-@erase "$(INTDIR)\MyToolBar.sbr"
	-@erase "$(INTDIR)\NivelView.obj"
	-@erase "$(INTDIR)\NivelView.sbr"
	-@erase "$(INTDIR)\OAEView.obj"
	-@erase "$(INTDIR)\OAEView.sbr"
	-@erase "$(INTDIR)\palette.obj"
	-@erase "$(INTDIR)\palette.sbr"
	-@erase "$(INTDIR)\PedirProj.obj"
	-@erase "$(INTDIR)\PedirProj.sbr"
	-@erase "$(INTDIR)\Perfil.obj"
	-@erase "$(INTDIR)\Perfil.sbr"
	-@erase "$(INTDIR)\projeto.obj"
	-@erase "$(INTDIR)\projeto.sbr"
	-@erase "$(INTDIR)\Relatorios.obj"
	-@erase "$(INTDIR)\Relatorios.sbr"
	-@erase "$(INTDIR)\SecTriangulacao.obj"
	-@erase "$(INTDIR)\SecTriangulacao.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\SuperCString.obj"
	-@erase "$(INTDIR)\SuperCString.sbr"
	-@erase "$(INTDIR)\Superficie.obj"
	-@erase "$(INTDIR)\Superficie.sbr"
	-@erase "$(INTDIR)\TaludesPavim.obj"
	-@erase "$(INTDIR)\TaludesPavim.sbr"
	-@erase "$(INTDIR)\TaludesPavimView.obj"
	-@erase "$(INTDIR)\TaludesPavimView.sbr"
	-@erase "$(INTDIR)\TaludesView.obj"
	-@erase "$(INTDIR)\TaludesView.sbr"
	-@erase "$(INTDIR)\TresPontosView.obj"
	-@erase "$(INTDIR)\TresPontosView.sbr"
	-@erase "$(INTDIR)\Typlstvw.obj"
	-@erase "$(INTDIR)\Typlstvw.sbr"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XYGreide.obj"
	-@erase "$(INTDIR)\XYGreide.sbr"
	-@erase "$(INTDIR)\xyMouse.obj"
	-@erase "$(INTDIR)\xyMouse.sbr"
	-@erase "$(OUTDIR)\Est.bsc"
	-@erase "$(OUTDIR)\Est.exe"
	-@erase "$(OUTDIR)\Est.ilk"
	-@erase "$(OUTDIR)\Est.pdb"
	-@erase ".\Debug\Est.hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /GR /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DISABLE WARNING C4786" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Est.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x416 /fo"$(INTDIR)\est.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/o"$(OUTDIR)\Est.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AbrirMedicao.sbr" \
	"$(INTDIR)\AlaSarPasView.sbr" \
	"$(INTDIR)\arqnovo.sbr" \
	"$(INTDIR)\CadLib.sbr" \
	"$(INTDIR)\CAlargamentos.sbr" \
	"$(INTDIR)\CalcNota.sbr" \
	"$(INTDIR)\CalculaNota.sbr" \
	"$(INTDIR)\CalcularMedicao.sbr" \
	"$(INTDIR)\CalculaTerreno.sbr" \
	"$(INTDIR)\CalcVolumes.sbr" \
	"$(INTDIR)\CArqAlargamentos.sbr" \
	"$(INTDIR)\CArqCanteiros.sbr" \
	"$(INTDIR)\CArqCurHorizontais.sbr" \
	"$(INTDIR)\CArqCurVerticais.sbr" \
	"$(INTDIR)\CArqGradientes.sbr" \
	"$(INTDIR)\CArqHPavimento.sbr" \
	"$(INTDIR)\CArqIgualdades.sbr" \
	"$(INTDIR)\CArqNivelamento.sbr" \
	"$(INTDIR)\CArqOAC.sbr" \
	"$(INTDIR)\CArqOAE.sbr" \
	"$(INTDIR)\CArqPerfis.sbr" \
	"$(INTDIR)\CArqPistasFixas.sbr" \
	"$(INTDIR)\CArqSecTipo.sbr" \
	"$(INTDIR)\CArqTalAterro.sbr" \
	"$(INTDIR)\CArqTalCorte.sbr" \
	"$(INTDIR)\CArqTaludesPavim.sbr" \
	"$(INTDIR)\CArqTerreno.sbr" \
	"$(INTDIR)\CArqTerrPontos.sbr" \
	"$(INTDIR)\CCanteiros.sbr" \
	"$(INTDIR)\CCurHorizontais.sbr" \
	"$(INTDIR)\CCurVerticais.sbr" \
	"$(INTDIR)\CDeltaSuperficie.sbr" \
	"$(INTDIR)\CDPopUpPontos.sbr" \
	"$(INTDIR)\CEstacasACalcular.sbr" \
	"$(INTDIR)\CGradientes.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\CHPavimento.sbr" \
	"$(INTDIR)\CIgualdade.sbr" \
	"$(INTDIR)\ClassSetPontos.sbr" \
	"$(INTDIR)\CListaPerfis.sbr" \
	"$(INTDIR)\CListaSecoes.sbr" \
	"$(INTDIR)\CLPontos.sbr" \
	"$(INTDIR)\COAC.sbr" \
	"$(INTDIR)\COACView.sbr" \
	"$(INTDIR)\COAE.sbr" \
	"$(INTDIR)\ComboProcDesSecao.sbr" \
	"$(INTDIR)\confdesmedicao.sbr" \
	"$(INTDIR)\confproj.sbr" \
	"$(INTDIR)\CPistasFixas.sbr" \
	"$(INTDIR)\CPonto.sbr" \
	"$(INTDIR)\CriarMedicao.sbr" \
	"$(INTDIR)\CSecao.sbr" \
	"$(INTDIR)\CSectipo.sbr" \
	"$(INTDIR)\CSStack.sbr" \
	"$(INTDIR)\CTalAterro.sbr" \
	"$(INTDIR)\CTalCorte.sbr" \
	"$(INTDIR)\CTerreno.sbr" \
	"$(INTDIR)\CTerrPontos.sbr" \
	"$(INTDIR)\CurHorView.sbr" \
	"$(INTDIR)\CurVerview.sbr" \
	"$(INTDIR)\CVisada.sbr" \
	"$(INTDIR)\DCalcGeometrico.sbr" \
	"$(INTDIR)\DCalcGeoSeccDireto.sbr" \
	"$(INTDIR)\DCalcNotaVolumesDireto.sbr" \
	"$(INTDIR)\DconfDesGeom.sbr" \
	"$(INTDIR)\DCoordenadas.sbr" \
	"$(INTDIR)\DCoresDesGeometrico.sbr" \
	"$(INTDIR)\DcoresGreide.sbr" \
	"$(INTDIR)\DDeslizantes.sbr" \
	"$(INTDIR)\DEditaCurhoriz.sbr" \
	"$(INTDIR)\DEntraPonto.sbr" \
	"$(INTDIR)\DEntraPontoGreide.sbr" \
	"$(INTDIR)\DeslizantesGeom.sbr" \
	"$(INTDIR)\DesPerfilView.sbr" \
	"$(INTDIR)\DesPontosGeomView.sbr" \
	"$(INTDIR)\DesSecaoView.sbr" \
	"$(INTDIR)\DEstacaCota.sbr" \
	"$(INTDIR)\DGerarDXF.sbr" \
	"$(INTDIR)\DGerarDXFGre.sbr" \
	"$(INTDIR)\DGerarDXFGreide.sbr" \
	"$(INTDIR)\DGerarDXFSecoes.sbr" \
	"$(INTDIR)\DGerarSuperficie.sbr" \
	"$(INTDIR)\Dialogo.sbr" \
	"$(INTDIR)\DiapontosGeom.sbr" \
	"$(INTDIR)\DRotacionar.sbr" \
	"$(INTDIR)\DSecionar.sbr" \
	"$(INTDIR)\DSelGreide.sbr" \
	"$(INTDIR)\DSelSecao.sbr" \
	"$(INTDIR)\DVolumes.sbr" \
	"$(INTDIR)\EditPopUp.sbr" \
	"$(INTDIR)\est.sbr" \
	"$(INTDIR)\Estaca.sbr" \
	"$(INTDIR)\estDoc.sbr" \
	"$(INTDIR)\EstRel.sbr" \
	"$(INTDIR)\estView.sbr" \
	"$(INTDIR)\GeraSuperficie.sbr" \
	"$(INTDIR)\GradienteView.sbr" \
	"$(INTDIR)\igualdades.sbr" \
	"$(INTDIR)\ListaMedicoes.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\Mat.sbr" \
	"$(INTDIR)\MelEditBox.sbr" \
	"$(INTDIR)\Monologo.sbr" \
	"$(INTDIR)\MyToolBar.sbr" \
	"$(INTDIR)\NivelView.sbr" \
	"$(INTDIR)\OAEView.sbr" \
	"$(INTDIR)\palette.sbr" \
	"$(INTDIR)\PedirProj.sbr" \
	"$(INTDIR)\Perfil.sbr" \
	"$(INTDIR)\projeto.sbr" \
	"$(INTDIR)\Relatorios.sbr" \
	"$(INTDIR)\SecTriangulacao.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\SuperCString.sbr" \
	"$(INTDIR)\Superficie.sbr" \
	"$(INTDIR)\TaludesPavim.sbr" \
	"$(INTDIR)\TaludesPavimView.sbr" \
	"$(INTDIR)\TaludesView.sbr" \
	"$(INTDIR)\TresPontosView.sbr" \
	"$(INTDIR)\Typlstvw.sbr" \
	"$(INTDIR)\XYGreide.sbr" \
	"$(INTDIR)\xyMouse.sbr"

"$(OUTDIR)\Est.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=gts.lib glib.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Est.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Est.exe" /libpath:"c:\glib" /libpath:"c:\dxf\dxf" 
LINK32_OBJS= \
	"$(INTDIR)\AbrirMedicao.obj" \
	"$(INTDIR)\AlaSarPasView.obj" \
	"$(INTDIR)\arqnovo.obj" \
	"$(INTDIR)\CadLib.obj" \
	"$(INTDIR)\CAlargamentos.obj" \
	"$(INTDIR)\CalcNota.obj" \
	"$(INTDIR)\CalculaNota.obj" \
	"$(INTDIR)\CalcularMedicao.obj" \
	"$(INTDIR)\CalculaTerreno.obj" \
	"$(INTDIR)\CalcVolumes.obj" \
	"$(INTDIR)\CArqAlargamentos.obj" \
	"$(INTDIR)\CArqCanteiros.obj" \
	"$(INTDIR)\CArqCurHorizontais.obj" \
	"$(INTDIR)\CArqCurVerticais.obj" \
	"$(INTDIR)\CArqGradientes.obj" \
	"$(INTDIR)\CArqHPavimento.obj" \
	"$(INTDIR)\CArqIgualdades.obj" \
	"$(INTDIR)\CArqNivelamento.obj" \
	"$(INTDIR)\CArqOAC.obj" \
	"$(INTDIR)\CArqOAE.obj" \
	"$(INTDIR)\CArqPerfis.obj" \
	"$(INTDIR)\CArqPistasFixas.obj" \
	"$(INTDIR)\CArqSecTipo.obj" \
	"$(INTDIR)\CArqTalAterro.obj" \
	"$(INTDIR)\CArqTalCorte.obj" \
	"$(INTDIR)\CArqTaludesPavim.obj" \
	"$(INTDIR)\CArqTerreno.obj" \
	"$(INTDIR)\CArqTerrPontos.obj" \
	"$(INTDIR)\CCanteiros.obj" \
	"$(INTDIR)\CCurHorizontais.obj" \
	"$(INTDIR)\CCurVerticais.obj" \
	"$(INTDIR)\CDeltaSuperficie.obj" \
	"$(INTDIR)\CDPopUpPontos.obj" \
	"$(INTDIR)\CEstacasACalcular.obj" \
	"$(INTDIR)\CGradientes.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CHPavimento.obj" \
	"$(INTDIR)\CIgualdade.obj" \
	"$(INTDIR)\ClassSetPontos.obj" \
	"$(INTDIR)\CListaPerfis.obj" \
	"$(INTDIR)\CListaSecoes.obj" \
	"$(INTDIR)\CLPontos.obj" \
	"$(INTDIR)\COAC.obj" \
	"$(INTDIR)\COACView.obj" \
	"$(INTDIR)\COAE.obj" \
	"$(INTDIR)\ComboProcDesSecao.obj" \
	"$(INTDIR)\confdesmedicao.obj" \
	"$(INTDIR)\confproj.obj" \
	"$(INTDIR)\CPistasFixas.obj" \
	"$(INTDIR)\CPonto.obj" \
	"$(INTDIR)\CriarMedicao.obj" \
	"$(INTDIR)\CSecao.obj" \
	"$(INTDIR)\CSectipo.obj" \
	"$(INTDIR)\CSStack.obj" \
	"$(INTDIR)\CTalAterro.obj" \
	"$(INTDIR)\CTalCorte.obj" \
	"$(INTDIR)\CTerreno.obj" \
	"$(INTDIR)\CTerrPontos.obj" \
	"$(INTDIR)\CurHorView.obj" \
	"$(INTDIR)\CurVerview.obj" \
	"$(INTDIR)\CVisada.obj" \
	"$(INTDIR)\DCalcGeometrico.obj" \
	"$(INTDIR)\DCalcGeoSeccDireto.obj" \
	"$(INTDIR)\DCalcNotaVolumesDireto.obj" \
	"$(INTDIR)\DconfDesGeom.obj" \
	"$(INTDIR)\DCoordenadas.obj" \
	"$(INTDIR)\DCoresDesGeometrico.obj" \
	"$(INTDIR)\DcoresGreide.obj" \
	"$(INTDIR)\DDeslizantes.obj" \
	"$(INTDIR)\DEditaCurhoriz.obj" \
	"$(INTDIR)\DEntraPonto.obj" \
	"$(INTDIR)\DEntraPontoGreide.obj" \
	"$(INTDIR)\DeslizantesGeom.obj" \
	"$(INTDIR)\DesPerfilView.obj" \
	"$(INTDIR)\DesPontosGeomView.obj" \
	"$(INTDIR)\DesSecaoView.obj" \
	"$(INTDIR)\DEstacaCota.obj" \
	"$(INTDIR)\DGerarDXF.obj" \
	"$(INTDIR)\DGerarDXFGre.obj" \
	"$(INTDIR)\DGerarDXFGreide.obj" \
	"$(INTDIR)\DGerarDXFSecoes.obj" \
	"$(INTDIR)\DGerarSuperficie.obj" \
	"$(INTDIR)\Dialogo.obj" \
	"$(INTDIR)\DiapontosGeom.obj" \
	"$(INTDIR)\DRotacionar.obj" \
	"$(INTDIR)\DSecionar.obj" \
	"$(INTDIR)\DSelGreide.obj" \
	"$(INTDIR)\DSelSecao.obj" \
	"$(INTDIR)\DVolumes.obj" \
	"$(INTDIR)\EditPopUp.obj" \
	"$(INTDIR)\est.obj" \
	"$(INTDIR)\Estaca.obj" \
	"$(INTDIR)\estDoc.obj" \
	"$(INTDIR)\EstRel.obj" \
	"$(INTDIR)\estView.obj" \
	"$(INTDIR)\GeraSuperficie.obj" \
	"$(INTDIR)\GradienteView.obj" \
	"$(INTDIR)\igualdades.obj" \
	"$(INTDIR)\ListaMedicoes.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Mat.obj" \
	"$(INTDIR)\MelEditBox.obj" \
	"$(INTDIR)\Monologo.obj" \
	"$(INTDIR)\MyToolBar.obj" \
	"$(INTDIR)\NivelView.obj" \
	"$(INTDIR)\OAEView.obj" \
	"$(INTDIR)\palette.obj" \
	"$(INTDIR)\PedirProj.obj" \
	"$(INTDIR)\Perfil.obj" \
	"$(INTDIR)\projeto.obj" \
	"$(INTDIR)\Relatorios.obj" \
	"$(INTDIR)\SecTriangulacao.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SuperCString.obj" \
	"$(INTDIR)\Superficie.obj" \
	"$(INTDIR)\TaludesPavim.obj" \
	"$(INTDIR)\TaludesPavimView.obj" \
	"$(INTDIR)\TaludesView.obj" \
	"$(INTDIR)\TresPontosView.obj" \
	"$(INTDIR)\Typlstvw.obj" \
	"$(INTDIR)\XYGreide.obj" \
	"$(INTDIR)\xyMouse.obj" \
	"$(INTDIR)\est.res"

"$(OUTDIR)\Est.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "est - Win32 Release"

OUTDIR=.
INTDIR=.
# Begin Custom Macros
OutDir=.
# End Custom Macros

ALL : ".\Est.hlp" "$(OUTDIR)\release\Est.exe"


CLEAN :
	-@erase "$(INTDIR)\AbrirMedicao.obj"
	-@erase "$(INTDIR)\AlaSarPasView.obj"
	-@erase "$(INTDIR)\arqnovo.obj"
	-@erase "$(INTDIR)\CadLib.obj"
	-@erase "$(INTDIR)\CAlargamentos.obj"
	-@erase "$(INTDIR)\CalcNota.obj"
	-@erase "$(INTDIR)\CalculaNota.obj"
	-@erase "$(INTDIR)\CalcularMedicao.obj"
	-@erase "$(INTDIR)\CalculaTerreno.obj"
	-@erase "$(INTDIR)\CalcVolumes.obj"
	-@erase "$(INTDIR)\CArqAlargamentos.obj"
	-@erase "$(INTDIR)\CArqCanteiros.obj"
	-@erase "$(INTDIR)\CArqCurHorizontais.obj"
	-@erase "$(INTDIR)\CArqCurVerticais.obj"
	-@erase "$(INTDIR)\CArqGradientes.obj"
	-@erase "$(INTDIR)\CArqHPavimento.obj"
	-@erase "$(INTDIR)\CArqIgualdades.obj"
	-@erase "$(INTDIR)\CArqNivelamento.obj"
	-@erase "$(INTDIR)\CArqOAC.obj"
	-@erase "$(INTDIR)\CArqOAE.obj"
	-@erase "$(INTDIR)\CArqPerfis.obj"
	-@erase "$(INTDIR)\CArqPistasFixas.obj"
	-@erase "$(INTDIR)\CArqSecTipo.obj"
	-@erase "$(INTDIR)\CArqTalAterro.obj"
	-@erase "$(INTDIR)\CArqTalCorte.obj"
	-@erase "$(INTDIR)\CArqTaludesPavim.obj"
	-@erase "$(INTDIR)\CArqTerreno.obj"
	-@erase "$(INTDIR)\CArqTerrPontos.obj"
	-@erase "$(INTDIR)\CCanteiros.obj"
	-@erase "$(INTDIR)\CCurHorizontais.obj"
	-@erase "$(INTDIR)\CCurVerticais.obj"
	-@erase "$(INTDIR)\CDeltaSuperficie.obj"
	-@erase "$(INTDIR)\CDPopUpPontos.obj"
	-@erase "$(INTDIR)\CEstacasACalcular.obj"
	-@erase "$(INTDIR)\CGradientes.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\CHPavimento.obj"
	-@erase "$(INTDIR)\CIgualdade.obj"
	-@erase "$(INTDIR)\ClassSetPontos.obj"
	-@erase "$(INTDIR)\CListaPerfis.obj"
	-@erase "$(INTDIR)\CListaSecoes.obj"
	-@erase "$(INTDIR)\CLPontos.obj"
	-@erase "$(INTDIR)\COAC.obj"
	-@erase "$(INTDIR)\COACView.obj"
	-@erase "$(INTDIR)\COAE.obj"
	-@erase "$(INTDIR)\ComboProcDesSecao.obj"
	-@erase "$(INTDIR)\confdesmedicao.obj"
	-@erase "$(INTDIR)\confproj.obj"
	-@erase "$(INTDIR)\CPistasFixas.obj"
	-@erase "$(INTDIR)\CPonto.obj"
	-@erase "$(INTDIR)\CriarMedicao.obj"
	-@erase "$(INTDIR)\CSecao.obj"
	-@erase "$(INTDIR)\CSectipo.obj"
	-@erase "$(INTDIR)\CSStack.obj"
	-@erase "$(INTDIR)\CTalAterro.obj"
	-@erase "$(INTDIR)\CTalCorte.obj"
	-@erase "$(INTDIR)\CTerreno.obj"
	-@erase "$(INTDIR)\CTerrPontos.obj"
	-@erase "$(INTDIR)\CurHorView.obj"
	-@erase "$(INTDIR)\CurVerview.obj"
	-@erase "$(INTDIR)\CVisada.obj"
	-@erase "$(INTDIR)\DCalcGeometrico.obj"
	-@erase "$(INTDIR)\DCalcGeoSeccDireto.obj"
	-@erase "$(INTDIR)\DCalcNotaVolumesDireto.obj"
	-@erase "$(INTDIR)\DconfDesGeom.obj"
	-@erase "$(INTDIR)\DCoordenadas.obj"
	-@erase "$(INTDIR)\DCoresDesGeometrico.obj"
	-@erase "$(INTDIR)\DcoresGreide.obj"
	-@erase "$(INTDIR)\DDeslizantes.obj"
	-@erase "$(INTDIR)\DEditaCurhoriz.obj"
	-@erase "$(INTDIR)\DEntraPonto.obj"
	-@erase "$(INTDIR)\DEntraPontoGreide.obj"
	-@erase "$(INTDIR)\DeslizantesGeom.obj"
	-@erase "$(INTDIR)\DesPerfilView.obj"
	-@erase "$(INTDIR)\DesPontosGeomView.obj"
	-@erase "$(INTDIR)\DesSecaoView.obj"
	-@erase "$(INTDIR)\DEstacaCota.obj"
	-@erase "$(INTDIR)\DGerarDXF.obj"
	-@erase "$(INTDIR)\DGerarDXFGre.obj"
	-@erase "$(INTDIR)\DGerarDXFGreide.obj"
	-@erase "$(INTDIR)\DGerarDXFSecoes.obj"
	-@erase "$(INTDIR)\DGerarSuperficie.obj"
	-@erase "$(INTDIR)\Dialogo.obj"
	-@erase "$(INTDIR)\DiapontosGeom.obj"
	-@erase "$(INTDIR)\DRotacionar.obj"
	-@erase "$(INTDIR)\DSecionar.obj"
	-@erase "$(INTDIR)\DSelGreide.obj"
	-@erase "$(INTDIR)\DSelSecao.obj"
	-@erase "$(INTDIR)\DVolumes.obj"
	-@erase "$(INTDIR)\EditPopUp.obj"
	-@erase "$(INTDIR)\est.obj"
	-@erase "$(INTDIR)\Est.pch"
	-@erase "$(INTDIR)\est.res"
	-@erase "$(INTDIR)\Estaca.obj"
	-@erase "$(INTDIR)\estDoc.obj"
	-@erase "$(INTDIR)\EstRel.obj"
	-@erase "$(INTDIR)\estView.obj"
	-@erase "$(INTDIR)\GeraSuperficie.obj"
	-@erase "$(INTDIR)\GradienteView.obj"
	-@erase "$(INTDIR)\igualdades.obj"
	-@erase "$(INTDIR)\ListaMedicoes.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\Mat.obj"
	-@erase "$(INTDIR)\MelEditBox.obj"
	-@erase "$(INTDIR)\Monologo.obj"
	-@erase "$(INTDIR)\MyToolBar.obj"
	-@erase "$(INTDIR)\NivelView.obj"
	-@erase "$(INTDIR)\OAEView.obj"
	-@erase "$(INTDIR)\palette.obj"
	-@erase "$(INTDIR)\PedirProj.obj"
	-@erase "$(INTDIR)\Perfil.obj"
	-@erase "$(INTDIR)\projeto.obj"
	-@erase "$(INTDIR)\Relatorios.obj"
	-@erase "$(INTDIR)\SecTriangulacao.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\SuperCString.obj"
	-@erase "$(INTDIR)\Superficie.obj"
	-@erase "$(INTDIR)\TaludesPavim.obj"
	-@erase "$(INTDIR)\TaludesPavimView.obj"
	-@erase "$(INTDIR)\TaludesView.obj"
	-@erase "$(INTDIR)\TresPontosView.obj"
	-@erase "$(INTDIR)\Typlstvw.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\XYGreide.obj"
	-@erase "$(INTDIR)\xyMouse.obj"
	-@erase "$(OUTDIR)\release\Est.exe"
	-@erase ".\Est.hlp"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MTd /W3 /Gi /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Est.pch" /Yu"stdafx.h" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x416 /fo"$(INTDIR)\est.res" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Est.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=glib.lib gts.lib /nologo /subsystem:windows /pdb:none /machine:I386 /out:"$(OUTDIR)\release\Est.exe" /libpath:"c:\glib" 
LINK32_OBJS= \
	"$(INTDIR)\AbrirMedicao.obj" \
	"$(INTDIR)\AlaSarPasView.obj" \
	"$(INTDIR)\arqnovo.obj" \
	"$(INTDIR)\CadLib.obj" \
	"$(INTDIR)\CAlargamentos.obj" \
	"$(INTDIR)\CalcNota.obj" \
	"$(INTDIR)\CalculaNota.obj" \
	"$(INTDIR)\CalcularMedicao.obj" \
	"$(INTDIR)\CalculaTerreno.obj" \
	"$(INTDIR)\CalcVolumes.obj" \
	"$(INTDIR)\CArqAlargamentos.obj" \
	"$(INTDIR)\CArqCanteiros.obj" \
	"$(INTDIR)\CArqCurHorizontais.obj" \
	"$(INTDIR)\CArqCurVerticais.obj" \
	"$(INTDIR)\CArqGradientes.obj" \
	"$(INTDIR)\CArqHPavimento.obj" \
	"$(INTDIR)\CArqIgualdades.obj" \
	"$(INTDIR)\CArqNivelamento.obj" \
	"$(INTDIR)\CArqOAC.obj" \
	"$(INTDIR)\CArqOAE.obj" \
	"$(INTDIR)\CArqPerfis.obj" \
	"$(INTDIR)\CArqPistasFixas.obj" \
	"$(INTDIR)\CArqSecTipo.obj" \
	"$(INTDIR)\CArqTalAterro.obj" \
	"$(INTDIR)\CArqTalCorte.obj" \
	"$(INTDIR)\CArqTaludesPavim.obj" \
	"$(INTDIR)\CArqTerreno.obj" \
	"$(INTDIR)\CArqTerrPontos.obj" \
	"$(INTDIR)\CCanteiros.obj" \
	"$(INTDIR)\CCurHorizontais.obj" \
	"$(INTDIR)\CCurVerticais.obj" \
	"$(INTDIR)\CDeltaSuperficie.obj" \
	"$(INTDIR)\CDPopUpPontos.obj" \
	"$(INTDIR)\CEstacasACalcular.obj" \
	"$(INTDIR)\CGradientes.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\CHPavimento.obj" \
	"$(INTDIR)\CIgualdade.obj" \
	"$(INTDIR)\ClassSetPontos.obj" \
	"$(INTDIR)\CListaPerfis.obj" \
	"$(INTDIR)\CListaSecoes.obj" \
	"$(INTDIR)\CLPontos.obj" \
	"$(INTDIR)\COAC.obj" \
	"$(INTDIR)\COACView.obj" \
	"$(INTDIR)\COAE.obj" \
	"$(INTDIR)\ComboProcDesSecao.obj" \
	"$(INTDIR)\confdesmedicao.obj" \
	"$(INTDIR)\confproj.obj" \
	"$(INTDIR)\CPistasFixas.obj" \
	"$(INTDIR)\CPonto.obj" \
	"$(INTDIR)\CriarMedicao.obj" \
	"$(INTDIR)\CSecao.obj" \
	"$(INTDIR)\CSectipo.obj" \
	"$(INTDIR)\CSStack.obj" \
	"$(INTDIR)\CTalAterro.obj" \
	"$(INTDIR)\CTalCorte.obj" \
	"$(INTDIR)\CTerreno.obj" \
	"$(INTDIR)\CTerrPontos.obj" \
	"$(INTDIR)\CurHorView.obj" \
	"$(INTDIR)\CurVerview.obj" \
	"$(INTDIR)\CVisada.obj" \
	"$(INTDIR)\DCalcGeometrico.obj" \
	"$(INTDIR)\DCalcGeoSeccDireto.obj" \
	"$(INTDIR)\DCalcNotaVolumesDireto.obj" \
	"$(INTDIR)\DconfDesGeom.obj" \
	"$(INTDIR)\DCoordenadas.obj" \
	"$(INTDIR)\DCoresDesGeometrico.obj" \
	"$(INTDIR)\DcoresGreide.obj" \
	"$(INTDIR)\DDeslizantes.obj" \
	"$(INTDIR)\DEditaCurhoriz.obj" \
	"$(INTDIR)\DEntraPonto.obj" \
	"$(INTDIR)\DEntraPontoGreide.obj" \
	"$(INTDIR)\DeslizantesGeom.obj" \
	"$(INTDIR)\DesPerfilView.obj" \
	"$(INTDIR)\DesPontosGeomView.obj" \
	"$(INTDIR)\DesSecaoView.obj" \
	"$(INTDIR)\DEstacaCota.obj" \
	"$(INTDIR)\DGerarDXF.obj" \
	"$(INTDIR)\DGerarDXFGre.obj" \
	"$(INTDIR)\DGerarDXFGreide.obj" \
	"$(INTDIR)\DGerarDXFSecoes.obj" \
	"$(INTDIR)\DGerarSuperficie.obj" \
	"$(INTDIR)\Dialogo.obj" \
	"$(INTDIR)\DiapontosGeom.obj" \
	"$(INTDIR)\DRotacionar.obj" \
	"$(INTDIR)\DSecionar.obj" \
	"$(INTDIR)\DSelGreide.obj" \
	"$(INTDIR)\DSelSecao.obj" \
	"$(INTDIR)\DVolumes.obj" \
	"$(INTDIR)\EditPopUp.obj" \
	"$(INTDIR)\est.obj" \
	"$(INTDIR)\Estaca.obj" \
	"$(INTDIR)\estDoc.obj" \
	"$(INTDIR)\EstRel.obj" \
	"$(INTDIR)\estView.obj" \
	"$(INTDIR)\GeraSuperficie.obj" \
	"$(INTDIR)\GradienteView.obj" \
	"$(INTDIR)\igualdades.obj" \
	"$(INTDIR)\ListaMedicoes.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\Mat.obj" \
	"$(INTDIR)\MelEditBox.obj" \
	"$(INTDIR)\Monologo.obj" \
	"$(INTDIR)\MyToolBar.obj" \
	"$(INTDIR)\NivelView.obj" \
	"$(INTDIR)\OAEView.obj" \
	"$(INTDIR)\palette.obj" \
	"$(INTDIR)\PedirProj.obj" \
	"$(INTDIR)\Perfil.obj" \
	"$(INTDIR)\projeto.obj" \
	"$(INTDIR)\Relatorios.obj" \
	"$(INTDIR)\SecTriangulacao.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\SuperCString.obj" \
	"$(INTDIR)\Superficie.obj" \
	"$(INTDIR)\TaludesPavim.obj" \
	"$(INTDIR)\TaludesPavimView.obj" \
	"$(INTDIR)\TaludesView.obj" \
	"$(INTDIR)\TresPontosView.obj" \
	"$(INTDIR)\Typlstvw.obj" \
	"$(INTDIR)\XYGreide.obj" \
	"$(INTDIR)\xyMouse.obj" \
	"$(INTDIR)\est.res"

"$(OUTDIR)\release\Est.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Est.dep")
!INCLUDE "Est.dep"
!ELSE 
!MESSAGE Warning: cannot find "Est.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "est - Win32 Debug" || "$(CFG)" == "est - Win32 Release"
SOURCE=.\AbrirMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\AbrirMedicao.obj"	"$(INTDIR)\AbrirMedicao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\AbrirMedicao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\AlaSarPasView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\AlaSarPasView.obj"	"$(INTDIR)\AlaSarPasView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\AlaSarPasView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\arqnovo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\arqnovo.obj"	"$(INTDIR)\arqnovo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\arqnovo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=..\DXF\DXF\CadLib\CadLib\Interface\VC\CadLib.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CadLib.obj"	"$(INTDIR)\CadLib.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CadLib.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\CAlargamentos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CAlargamentos.obj"	"$(INTDIR)\CAlargamentos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CAlargamentos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CalcNota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CalcNota.obj"	"$(INTDIR)\CalcNota.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CalcNota.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CalculaNota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CalculaNota.obj"	"$(INTDIR)\CalculaNota.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CalculaNota.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CalcularMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CalcularMedicao.obj"	"$(INTDIR)\CalcularMedicao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CalcularMedicao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CalculaTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CalculaTerreno.obj"	"$(INTDIR)\CalculaTerreno.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CalculaTerreno.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CalcVolumes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CalcVolumes.obj"	"$(INTDIR)\CalcVolumes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CalcVolumes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqAlargamentos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqAlargamentos.obj"	"$(INTDIR)\CArqAlargamentos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqAlargamentos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqCanteiros.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqCanteiros.obj"	"$(INTDIR)\CArqCanteiros.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqCanteiros.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqCurHorizontais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqCurHorizontais.obj"	"$(INTDIR)\CArqCurHorizontais.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqCurHorizontais.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqCurVerticais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqCurVerticais.obj"	"$(INTDIR)\CArqCurVerticais.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqCurVerticais.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqGradientes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqGradientes.obj"	"$(INTDIR)\CArqGradientes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqGradientes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqHPavimento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqHPavimento.obj"	"$(INTDIR)\CArqHPavimento.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqHPavimento.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqIgualdades.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqIgualdades.obj"	"$(INTDIR)\CArqIgualdades.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqIgualdades.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqNivelamento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqNivelamento.obj"	"$(INTDIR)\CArqNivelamento.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqNivelamento.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqOAC.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqOAC.obj"	"$(INTDIR)\CArqOAC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqOAC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqOAE.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqOAE.obj"	"$(INTDIR)\CArqOAE.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqOAE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqPerfis.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqPerfis.obj"	"$(INTDIR)\CArqPerfis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqPerfis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqPistasFixas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqPistasFixas.obj"	"$(INTDIR)\CArqPistasFixas.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqPistasFixas.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqSecTipo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqSecTipo.obj"	"$(INTDIR)\CArqSecTipo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqSecTipo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqTalAterro.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqTalAterro.obj"	"$(INTDIR)\CArqTalAterro.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqTalAterro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqTalCorte.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqTalCorte.obj"	"$(INTDIR)\CArqTalCorte.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqTalCorte.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqTaludesPavim.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqTaludesPavim.obj"	"$(INTDIR)\CArqTaludesPavim.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqTaludesPavim.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqTerreno.obj"	"$(INTDIR)\CArqTerreno.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqTerreno.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CArqTerrPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CArqTerrPontos.obj"	"$(INTDIR)\CArqTerrPontos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CArqTerrPontos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CCanteiros.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CCanteiros.obj"	"$(INTDIR)\CCanteiros.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CCanteiros.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CCurHorizontais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CCurHorizontais.obj"	"$(INTDIR)\CCurHorizontais.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CCurHorizontais.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CCurVerticais.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CCurVerticais.obj"	"$(INTDIR)\CCurVerticais.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CCurVerticais.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CDeltaSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CDeltaSuperficie.obj"	"$(INTDIR)\CDeltaSuperficie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CDeltaSuperficie.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CDPopUpPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CDPopUpPontos.obj"	"$(INTDIR)\CDPopUpPontos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CDPopUpPontos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CEstacasACalcular.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CEstacasACalcular.obj"	"$(INTDIR)\CEstacasACalcular.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CEstacasACalcular.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CGradientes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CGradientes.obj"	"$(INTDIR)\CGradientes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CGradientes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\ChildFrm.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\ChildFrm.obj"	"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CHPavimento.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CHPavimento.obj"	"$(INTDIR)\CHPavimento.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CHPavimento.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CIgualdade.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CIgualdade.obj"	"$(INTDIR)\CIgualdade.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CIgualdade.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\ClassSetPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\ClassSetPontos.obj"	"$(INTDIR)\ClassSetPontos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\ClassSetPontos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CListaPerfis.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CListaPerfis.obj"	"$(INTDIR)\CListaPerfis.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CListaPerfis.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CListaSecoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CListaSecoes.obj"	"$(INTDIR)\CListaSecoes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CListaSecoes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CLPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CLPontos.obj"	"$(INTDIR)\CLPontos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CLPontos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\COAC.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\COAC.obj"	"$(INTDIR)\COAC.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\COAC.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\COACView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\COACView.obj"	"$(INTDIR)\COACView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\COACView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\COAE.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\COAE.obj"	"$(INTDIR)\COAE.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\COAE.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\ComboProcDesSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\ComboProcDesSecao.obj"	"$(INTDIR)\ComboProcDesSecao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\ComboProcDesSecao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\confdesmedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\confdesmedicao.obj"	"$(INTDIR)\confdesmedicao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\confdesmedicao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\confproj.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\confproj.obj"	"$(INTDIR)\confproj.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\confproj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CPistasFixas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CPistasFixas.obj"	"$(INTDIR)\CPistasFixas.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CPistasFixas.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CPonto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CPonto.obj"	"$(INTDIR)\CPonto.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CPonto.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CriarMedicao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CriarMedicao.obj"	"$(INTDIR)\CriarMedicao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CriarMedicao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CSecao.obj"	"$(INTDIR)\CSecao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CSecao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CSectipo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CSectipo.obj"	"$(INTDIR)\CSectipo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CSectipo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CSStack.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CSStack.obj"	"$(INTDIR)\CSStack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CSStack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CTalAterro.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CTalAterro.obj"	"$(INTDIR)\CTalAterro.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CTalAterro.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CTalCorte.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CTalCorte.obj"	"$(INTDIR)\CTalCorte.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CTalCorte.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CTerreno.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CTerreno.obj"	"$(INTDIR)\CTerreno.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CTerreno.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CTerrPontos.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CTerrPontos.obj"	"$(INTDIR)\CTerrPontos.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CTerrPontos.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CurHorView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CurHorView.obj"	"$(INTDIR)\CurHorView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CurHorView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CurVerview.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CurVerview.obj"	"$(INTDIR)\CurVerview.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CurVerview.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\CVisada.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\CVisada.obj"	"$(INTDIR)\CVisada.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\CVisada.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DCalcGeometrico.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DCalcGeometrico.obj"	"$(INTDIR)\DCalcGeometrico.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DCalcGeometrico.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DCalcGeoSeccDireto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DCalcGeoSeccDireto.obj"	"$(INTDIR)\DCalcGeoSeccDireto.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DCalcGeoSeccDireto.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DCalcNotaVolumesDireto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DCalcNotaVolumesDireto.obj"	"$(INTDIR)\DCalcNotaVolumesDireto.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DCalcNotaVolumesDireto.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DconfDesGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DconfDesGeom.obj"	"$(INTDIR)\DconfDesGeom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DconfDesGeom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DCoordenadas.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DCoordenadas.obj"	"$(INTDIR)\DCoordenadas.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DCoordenadas.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DCoresDesGeometrico.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DCoresDesGeometrico.obj"	"$(INTDIR)\DCoresDesGeometrico.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DCoresDesGeometrico.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DcoresGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DcoresGreide.obj"	"$(INTDIR)\DcoresGreide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DcoresGreide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DDeslizantes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DDeslizantes.obj"	"$(INTDIR)\DDeslizantes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DDeslizantes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DEditaCurhoriz.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DEditaCurhoriz.obj"	"$(INTDIR)\DEditaCurhoriz.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DEditaCurhoriz.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DEntraPonto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DEntraPonto.obj"	"$(INTDIR)\DEntraPonto.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DEntraPonto.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DEntraPontoGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DEntraPontoGreide.obj"	"$(INTDIR)\DEntraPontoGreide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DEntraPontoGreide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DeslizantesGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DeslizantesGeom.obj"	"$(INTDIR)\DeslizantesGeom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DeslizantesGeom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DesPerfilView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DesPerfilView.obj"	"$(INTDIR)\DesPerfilView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DesPerfilView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DesPontosGeomView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DesPontosGeomView.obj"	"$(INTDIR)\DesPontosGeomView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DesPontosGeomView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DesSecaoView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DesSecaoView.obj"	"$(INTDIR)\DesSecaoView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DesSecaoView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DEstacaCota.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DEstacaCota.obj"	"$(INTDIR)\DEstacaCota.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DEstacaCota.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DGerarDXF.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DGerarDXF.obj"	"$(INTDIR)\DGerarDXF.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DGerarDXF.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DGerarDXFGre.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DGerarDXFGre.obj"	"$(INTDIR)\DGerarDXFGre.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DGerarDXFGre.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DGerarDXFGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DGerarDXFGreide.obj"	"$(INTDIR)\DGerarDXFGreide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DGerarDXFGreide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DGerarDXFSecoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DGerarDXFSecoes.obj"	"$(INTDIR)\DGerarDXFSecoes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DGerarDXFSecoes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DGerarSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DGerarSuperficie.obj"	"$(INTDIR)\DGerarSuperficie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DGerarSuperficie.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Dialogo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Dialogo.obj"	"$(INTDIR)\Dialogo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Dialogo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DiapontosGeom.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DiapontosGeom.obj"	"$(INTDIR)\DiapontosGeom.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DiapontosGeom.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DRotacionar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DRotacionar.obj"	"$(INTDIR)\DRotacionar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DRotacionar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DSecionar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DSecionar.obj"	"$(INTDIR)\DSecionar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DSecionar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DSelGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DSelGreide.obj"	"$(INTDIR)\DSelGreide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DSelGreide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DSelSecao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DSelSecao.obj"	"$(INTDIR)\DSelSecao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DSelSecao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\DVolumes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\DVolumes.obj"	"$(INTDIR)\DVolumes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\DVolumes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\EditPopUp.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\EditPopUp.obj"	"$(INTDIR)\EditPopUp.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\EditPopUp.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\est.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\est.obj"	"$(INTDIR)\est.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\est.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\hlp\est.hpj

!IF  "$(CFG)" == "est - Win32 Debug"

OutDir=.\Debug
ProjDir=.
TargetName=Est
InputPath=.\hlp\est.hpj

"$(INTDIR)\Est.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ELSEIF  "$(CFG)" == "est - Win32 Release"

OutDir=.
ProjDir=.
TargetName=Est
InputPath=.\hlp\est.hpj

"$(INTDIR)\Est.hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	<<tempfile.bat 
	@echo off 
	"$(ProjDir)\makehelp.bat"
<< 
	

!ENDIF 

SOURCE=.\est.rc

"$(INTDIR)\est.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\Estaca.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Estaca.obj"	"$(INTDIR)\Estaca.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Estaca.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\estDoc.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\estDoc.obj"	"$(INTDIR)\estDoc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\estDoc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\EstRel.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\EstRel.obj"	"$(INTDIR)\EstRel.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\EstRel.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\estView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\estView.obj"	"$(INTDIR)\estView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\estView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\GeraSuperficie.cpp

!IF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\GeraSuperficie.obj"	"$(INTDIR)\GeraSuperficie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\GeraSuperficie.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\GradienteView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\GradienteView.obj"	"$(INTDIR)\GradienteView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\GradienteView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\igualdades.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\igualdades.obj"	"$(INTDIR)\igualdades.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\igualdades.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\ListaMedicoes.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\ListaMedicoes.obj"	"$(INTDIR)\ListaMedicoes.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\ListaMedicoes.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\MainFrm.obj"	"$(INTDIR)\MainFrm.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Mat.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Mat.obj"	"$(INTDIR)\Mat.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Mat.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\MelEditBox.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\MelEditBox.obj"	"$(INTDIR)\MelEditBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\MelEditBox.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Monologo.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Monologo.obj"	"$(INTDIR)\Monologo.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Monologo.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\MyToolBar.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\MyToolBar.obj"	"$(INTDIR)\MyToolBar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\MyToolBar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\NivelView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\NivelView.obj"	"$(INTDIR)\NivelView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\NivelView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\OAEView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\OAEView.obj"	"$(INTDIR)\OAEView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\OAEView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\palette.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\palette.obj"	"$(INTDIR)\palette.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\palette.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\PedirProj.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\PedirProj.obj"	"$(INTDIR)\PedirProj.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\PedirProj.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Perfil.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Perfil.obj"	"$(INTDIR)\Perfil.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Perfil.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\projeto.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\projeto.obj"	"$(INTDIR)\projeto.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\projeto.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Relatorios.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Relatorios.obj"	"$(INTDIR)\Relatorios.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Relatorios.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\SecTriangulacao.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\SecTriangulacao.obj"	"$(INTDIR)\SecTriangulacao.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\SecTriangulacao.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Splash.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Splash.obj"	"$(INTDIR)\Splash.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Splash.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "est - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /GR /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "DISABLE WARNING C4786" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Est.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Est.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "est - Win32 Release"

CPP_SWITCHES=/nologo /Zp1 /MTd /W3 /Gi /GX /ZI /Od /I "c:\glib" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\Est.pch" /Yc"stdafx.h" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\Est.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\SuperCString.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\SuperCString.obj"	"$(INTDIR)\SuperCString.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\SuperCString.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Superficie.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Superficie.obj"	"$(INTDIR)\Superficie.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Superficie.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\TaludesPavim.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\TaludesPavim.obj"	"$(INTDIR)\TaludesPavim.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\TaludesPavim.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\TaludesPavimView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\TaludesPavimView.obj"	"$(INTDIR)\TaludesPavimView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\TaludesPavimView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\TaludesView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\TaludesView.obj"	"$(INTDIR)\TaludesView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\TaludesView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\TresPontosView.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\TresPontosView.obj"	"$(INTDIR)\TresPontosView.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\TresPontosView.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\Typlstvw.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\Typlstvw.obj"	"$(INTDIR)\Typlstvw.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\Typlstvw.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\XYGreide.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\XYGreide.obj"	"$(INTDIR)\XYGreide.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\XYGreide.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 

SOURCE=.\xyMouse.cpp

!IF  "$(CFG)" == "est - Win32 Debug"


"$(INTDIR)\xyMouse.obj"	"$(INTDIR)\xyMouse.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ELSEIF  "$(CFG)" == "est - Win32 Release"


"$(INTDIR)\xyMouse.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Est.pch"


!ENDIF 


!ENDIF 

