// ABpcManager.h: interface for the ABpcManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABPCMANAGER_H__C441BB4B_7033_4F56_931F_F75372CDA9E6__INCLUDED_)
#define AFX_ABPCMANAGER_H__C441BB4B_7033_4F56_931F_F75372CDA9E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum eTrackInResult
{
	TRACKIN_RES_PASS,
	TRACKIN_RES_ERR_MSS,
	TRACKIN_RES_ERR_ETC,
};

class ABpcManager  
{
public:
	ABpcManager();
	virtual ~ABpcManager();

	void SetTrackInResult( int iRes );
	int GetTrackInResult() { return m_iTrackInResult; }
	void Initialize();

protected:
	int m_iTrackInResult;

};

extern ABpcManager g_bpc;
#endif // !defined(AFX_ABPCMANAGER_H__C441BB4B_7033_4F56_931F_F75372CDA9E6__INCLUDED_)
