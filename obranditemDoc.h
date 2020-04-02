/*===========================================================================
 *
 * File:	obranditemDoc.H
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Definition of the CObranditemDoc document class.
 *
 *=========================================================================*/
#ifndef __obranditemDoc_H
#define __obranditemDoc_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  #include "randitem/obri_generate.h"
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Definitions
 *
 *=========================================================================*/

	/* Update hints */
  #define OBRI_DOC_HINT_UPDATEALL 0x1004
  #define OBRI_DOC_HINT_GETDATA   0x1005

  typedef CObPtrArray<CSString> CSStringArray;

/*===========================================================================
 *		End of Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Definition
 * 
 *=========================================================================*/
class CObranditemDoc : public CDocument {

  /*---------- Begin Protected Class Members --------------------------*/
protected:
  CObriGenerate		m_Generate;
  
  CSStringArray		m_ProjectFiles;



  /*---------- Begin Protected Class Methods --------------------------*/
protected:
	
	/* Create from serialization only */
  CObranditemDoc();
  DECLARE_DYNCREATE(CObranditemDoc)


  /*---------- Begin Public Class Methods -----------------------------*/
public:

	/* Destructor */
  virtual ~CObranditemDoc();

	/* Batch file methods */
  bool ParseProjectFile  (const TCHAR* pFilename);
  bool RunBatch          (const TCHAR* pFilename);
  bool ProcessBatchFiles (void);

	/* Get class members */
  CObriGenerate& GetGenerate() { return (m_Generate); }
  CObEspFile&    GetEspFile()  { return (m_Generate.GetEspFile()); }

	/* Diagnostics */
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif


	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObranditemDoc)
	public:
  virtual BOOL OnNewDocument();
  virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

	/* Generated message map functions */
protected:
  //{{AFX_MSG(CObranditemDoc)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

 };
/*===========================================================================
 *		End of Class CObranditemDoc Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif
/*===========================================================================
 *		End of File obranditemDoc.H
 *=========================================================================*/
