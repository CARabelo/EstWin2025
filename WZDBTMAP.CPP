// WzdBtmap.cpp : implementation of the CWzdBitmap class
//
#include "stdafx.h"
#include "WzdBtmap.h"

/////////////////////////////////////////////////////////////////////////////
// CWzdBitmap

IMPLEMENT_DYNAMIC(CWzdBitmap, CBitmap)

void CWzdBitmap::Capture(const CRect &rect)
{
	// cleanup old captures
	if (m_pPalette)
	{
		DeleteObject();
		delete m_pPalette;
		m_pPalette = NULL;
	}

	// save width and height

	m_nWidth=rect.Width();
	m_nHeight=rect.Height();

	////////////////////////////////////////
	// copy screen image into a bitmap object
	////////////////////////////////////////

	// create a device context that accesses the whole screen

	CDC dcScreen;
  dcScreen.CreateDC("DISPLAY", NULL, NULL, NULL);

	// create an empty bitmap in memory

	CDC dcMem;
  dcMem.CreateCompatibleDC(&dcScreen);
  CreateCompatibleBitmap(&dcScreen, m_nWidth, m_nHeight);
  dcMem.SelectObject(this);

  // copy screen into empty bitmap

  dcMem.BitBlt(0,0,m_nWidth,m_nHeight,&dcScreen,rect.left,rect.top,SRCCOPY);

	// this bitmap is worthless without the current system palette, so...

	//////////////////////////////////////////
	// save system palette in this bitmap's palette
	///////////////////////////////////////////

	// create an empty logical palette that's big enough to hold all the colors

  int nColors((1 << (dcScreen.GetDeviceCaps(BITSPIXEL) * dcScreen.GetDeviceCaps(PLANES))));
	LOGPALETTE *pLogPal((LOGPALETTE *)new BYTE[sizeof(LOGPALETTE) + (nColors * sizeof(PALETTEENTRY))]);

	// initialize this empty palette's header

	pLogPal->palVersion    = 0x300;
	pLogPal->palNumEntries = nColors;

	// load this empty palette with the system palette's colors

  ::GetSystemPaletteEntries(dcScreen.m_hDC, 0, nColors,(LPPALETTEENTRY)(pLogPal->palPalEntry));

	// create the palette with this logical palette

	m_pPalette=new CPalette;
	m_pPalette->CreatePalette(pLogPal);

  // clean up

	delete []pLogPal;
	dcMem.DeleteDC();
	dcScreen.DeleteDC();

	m_bCaptured = true;
}
void CWzdBitmap::MostraBM(CDC* pDC)
{
	if (m_bCaptured)
	{
		// select bitmap palette
		CPalette *pOldPal = pDC->SelectPalette(GetPalette(),FALSE);
		pDC->RealizePalette();

		// get device context to select bitmap into
		CDC dcComp;
		dcComp.CreateCompatibleDC(pDC);
		dcComp.SelectObject(this);

		// draw bitmap
		pDC->BitBlt(0,0,m_nWidth,m_nHeight, &dcComp, 10,10,SRCCOPY);

		// reselect old palette
		pDC->SelectPalette(pOldPal,FALSE);
	}
}

