#if !defined(AFX_CLISTTEST_H__BD3AC10B_792A_46BB_BCF8_390630208A29__INCLUDED_)
#define AFX_CLISTTEST_H__BD3AC10B_792A_46BB_BCF8_390630208A29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// clisttest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// clisttest window

class clisttest : public CListCtrl
{
// Construction
public:
	clisttest();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(clisttest)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~clisttest();

	// Generated message map functions
protected:
	//{{AFX_MSG(clisttest)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLISTTEST_H__BD3AC10B_792A_46BB_BCF8_390630208A29__INCLUDED_)
