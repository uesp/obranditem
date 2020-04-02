/*===========================================================================
 *
 * File:	ObSplitterWnd.H
 * Author:	Dave Humphrey (uesp@sympatico.ca)
 * Created On:	April 27, 2006
 *
 * Description
 *
 *=========================================================================*/
#ifndef __OBSPLITTERWND_H
#define __OBSPLITTERWND_H


/*===========================================================================
 *
 * Begin Required Includes
 *
 *=========================================================================*/
  
/*===========================================================================
 *		End of Required Includes
 *=========================================================================*/


/*===========================================================================
 *
 * Begin Class CObSplitterWnd Definition
 *
 *=========================================================================*/
class CObSplitterWnd : public CSplitterWnd {

  /*---------- Begin Protected Class Members ----------------------*/
protected:
  

  /*---------- Begin Protected Class Methods ----------------------*/
protected:


  /*---------- Begin Public Class Methods -------------------------*/
public:

	/* Construction/Destructor */
  CObSplitterWnd();
  virtual ~CObSplitterWnd();

	/* Overloaded to change layout order */
  virtual void RecalcLayout();  

	/* ClassWizard generated virtual function overrides */
  //{{AFX_VIRTUAL(CObSplitterWnd)
  //}}AFX_VIRTUAL

protected:

	/* Generated message map functions */
  //{{AFX_MSG(CObSplitterWnd)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()

};
/*===========================================================================
 *		End of Class CObSplitterWnd Definition
 *=========================================================================*/


//{{AFX_INSERT_LOCATION}}
//}}AFX_INSERT_LOCATION


#endif
/*===========================================================================
 *		End of File ObSplitterWnd.H
 *=========================================================================*/
