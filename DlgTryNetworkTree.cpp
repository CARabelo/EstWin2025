// DlgTryNetworkTree.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTryNetworkTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DlgGetPath.h"								//Definition to get path


/////////////////////////////////////////////////////////////////////////////
// CDlgTryNetworkTree dialog
CDlgTryNetworkTree::CDlgTryNetworkTree(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTryNetworkTree::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTryNetworkTree)
	m_sPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlgTryNetworkTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTryNetworkTree)
	DDX_Text(pDX, IDC_EDIT_PATH, m_sPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgTryNetworkTree, CDialog)
	//{{AFX_MSG_MAP(CDlgTryNetworkTree)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GETPATH, OnButtonGetpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ***************************************************************************
//DESCRIPTION:
//		Setup inital path before dialog is displayed
//CREATED:
//		6-9-2000, 14:36:35 by john@mctainsh.com
// ***************************************************************************
BOOL CDlgTryNetworkTree::OnInitDialog()
{
	TRACE( _T("CDlgTryNetworkTree::OnInitDialog()\n") );
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

	//Start by displaying the current path
	int nDrive = _getdrive();
	TCHAR szPath[_MAX_PATH];
	VERIFY( _getdcwd( nDrive, szPath, _MAX_PATH ) != NULL );
	m_sPath = szPath;
	UpdateData( false );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CDlgTryNetworkTree::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDlgTryNetworkTree::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// ***************************************************************************
//DESCRIPTION:
//		Display the Path selection dialog.
//CREATED:
//		6-9-2000, 14:36:23 by john@mctainsh.com
// ***************************************************************************
void CDlgTryNetworkTree::OnButtonGetpath() 
{
	TRACE( _T("CDlgTryNetworkTree::OnButtonGetpath() \n") );
	if( UpdateData( true ) )
	{
		CDlgGetPath dlg( this );
		dlg.SetPath( m_sPath );
		if( dlg.DoModal() == IDOK )
		{
			m_sPath = dlg.GetPath( );
			UpdateData( false );
		}
	}
}
