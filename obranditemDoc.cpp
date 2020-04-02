/*===========================================================================
 *
 * File:	obranditemDoc.CPP
 * Author:	Dave Humphrey (uesp@m0use.net)
 * Created On:	Wednesday, 19 April, 2006
 *
 * Implementation of the CObranditemDoc document class.
 *
 *=========================================================================*/

	/* Include Files */
#include "stdafx.h"
#include "obranditem.h"
#include "obranditemDoc.h"


/*===========================================================================
 *
 * Begin Location Definitions
 *
 *=========================================================================*/

	/* Debug definitions */
#ifdef _DEBUG
  #define new DEBUG_NEW
  #undef THIS_FILE
  static char THIS_FILE[] = __FILE__;
#endif

  IMPLEMENT_DYNCREATE(CObranditemDoc, CDocument)

/*===========================================================================
 *		End of Location Definitions
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObranditemDoc Message Map
 *
 *=========================================================================*/
BEGIN_MESSAGE_MAP(CObranditemDoc, CDocument)
  //{{AFX_MSG_MAP(CObranditemDoc)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()
/*===========================================================================
 *		End of Class CObranditemDoc Message Map
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Dispatch Map
 *
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Constructor
 *
 *=========================================================================*/
CObranditemDoc::CObranditemDoc() {

 }
/*===========================================================================
 *		End of Class CObranditemDoc Constructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Destructor
 *
 *=========================================================================*/
CObranditemDoc::~CObranditemDoc() {
 }

/*===========================================================================
 *		End of Class CObranditemDoc Destructor
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Method - void DeleteContents ();
 *
 *=========================================================================*/
void CObranditemDoc::DeleteContents() {
  CDocument::DeleteContents();

  m_Generate.Destroy();
  m_Generate.InitializeEspFile();
 }
/*===========================================================================
 *		End of Class Method CObranditemDoc::DeleteContents()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Event - BOOL OnNewDocument ();
 *
 *=========================================================================*/
BOOL CObranditemDoc::OnNewDocument() {
  if (!CDocument::OnNewDocument()) return FALSE;

  UpdateAllViews(NULL, OBRI_DOC_HINT_UPDATEALL, NULL);
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObranditemDoc::OnNewDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Method - void Serialize (ar);
 *
 *=========================================================================*/
void CObranditemDoc::Serialize(CArchive& ar) {

  //assert(false);
 }
/*===========================================================================
 *		End of Class Method CObranditemDoc::Serialize()
 *=========================================================================*/


#ifdef _DEBUG
/*===========================================================================
 *
 * Class CObranditemDoc Diagnostics
 * 
 *=========================================================================*/
void CObranditemDoc::AssertValid() const {
  CDocument::AssertValid();
 }

void CObranditemDoc::Dump(CDumpContext& dc) const {
  CDocument::Dump(dc);
 }
/*===========================================================================
 *		End of Class CObranditemDoc Diagnostics
 *=========================================================================*/
#endif


/*===========================================================================
 *
 * Class CObranditemDoc Event - BOOL OnOpenDocument (lpszPathName);
 *
 * Loads a project file.
 *
 *=========================================================================*/
BOOL CObranditemDoc::OnOpenDocument (LPCTSTR lpszPathName) {
  bool Result;

	/* Ensure the document is empty */
  DeleteContents();
  SetModifiedFlag();

  Result = m_Generate.LoadProjectFile(lpszPathName);

  if (!Result) {
    ReportSaveLoadException(lpszPathName, NULL, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
    return (FALSE); 
   }

  SetModifiedFlag(FALSE);
  UpdateAllViews(NULL, OBRI_DOC_HINT_UPDATEALL, NULL);
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObranditemDoc::OnOpenDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Event - BOOL OnSaveDocument (lpszPathName);
 *
 *=========================================================================*/
BOOL CObranditemDoc::OnSaveDocument (LPCTSTR lpszPathName) {
  bool Result;

  UpdateAllViews(NULL, OBRI_DOC_HINT_GETDATA, NULL);

  Result = m_Generate.SaveProjectFile(lpszPathName);

  if (!Result) {
    ReportSaveLoadException(lpszPathName, NULL, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
    return (FALSE); 
   }

  SetModifiedFlag(FALSE);
  return (TRUE);
 }
/*===========================================================================
 *		End of Class Event CObranditemDoc::OnSaveDocument()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Method - bool RunBatch (pFilename);
 *
 *=========================================================================*/
bool CObranditemDoc::RunBatch (const TCHAR* pFilename) {
  bool Result;

	/* Parse all input files */
  m_ProjectFiles.Destroy();
  Result = ParseProjectFile(pFilename);
  if (!Result) return (false);
  
	/* Check if there are any project files to process */
  if (m_ProjectFiles.GetSize() <= 0) {
    AddObGeneralError("No project files to process!");
    return (false);
   }

  return ProcessBatchFiles();
 }
/*===========================================================================
 *		End of Class Method CObranditemDoc::RunBatch()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Method - bool ProcessBatchFiles (void);
 *
 *=========================================================================*/
bool CObranditemDoc::ProcessBatchFiles (void) {
  CSString* pString;
  dword     Index;
  bool      Result;

	/* Perform a full reset of generation data initially */
  //m_Generate.Reset();

  for (Index = 0; Index < m_ProjectFiles.GetSize(); ++Index) {
    pString = m_ProjectFiles[Index];

    SystemLog.Printf("Processing project '%s'...", pString->c_str());

    this->SetPathName(*pString, FALSE);
    Result = m_Generate.MergeProjectFile(*pString);
    if (!Result) continue;

    Result = m_Generate.CreateItems(Index == 0);
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObranditemDoc::ProcessBatchFiles()
 *=========================================================================*/


/*===========================================================================
 *
 * Class CObranditemDoc Method - bool ParseProjectFile (pFilename);
 *
 *=========================================================================*/
bool CObranditemDoc::ParseProjectFile (const TCHAR* pFilename) {
  CObFile   File;
  CSString  Buffer;
  CSString* pNewString;
  bool      Result;
    
  Result = File.Open(pFilename, "rt");
  if (!Result) return (false);

  while (!File.IsEOF()) {
    Result = File.ReadLine(Buffer);
    Buffer.Trim();

    if (Result && !Buffer.IsEmpty()) {
      pNewString = new CSString(Buffer);
      m_ProjectFiles.Add(pNewString);
     }
   }

  return (true);
 }
/*===========================================================================
 *		End of Class Method CObranditemDoc::ParseProjectFile()
 *=========================================================================*/
