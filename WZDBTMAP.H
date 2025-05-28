#ifndef WZDBITMAP_H
#define WZDBITMAP_H

class CWzdBitmap : public CBitmap
{
	bool m_bCaptured;
	public:
	DECLARE_DYNAMIC(CWzdBitmap)

	// Constructors

	CWzdBitmap::CWzdBitmap() : m_pPalette(NULL),m_nWidth(0),m_nHeight(0),m_bCaptured(false) {}

	void Capture(const CRect &rect);
	void MostraBM(CDC* pDC);
	CPalette *GetPalette(){return m_pPalette;};

	// Implementation

	virtual ~CWzdBitmap(){if (m_pPalette)	delete m_pPalette;}

	// Attributes

	int	m_nWidth;
	int m_nHeight;

	// Operations

	private:
	CPalette *m_pPalette;
};
#endif
