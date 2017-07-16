// AMTLotManager.h: interface for the AMTLotManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
#define AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// ALot
class ALot
{
public:
	ALot();
	ALot( CString strLotID, CString strPartID );
	virtual ~ALot();

	CString GetLotID() { return m_strLotID; }
	CString GetPartID() { return m_strPartID; }

protected:
	CString m_strLotID;
	CString m_strPartID;
};


// AMTLotManager
class AMTLotManager  
{
public:
	AMTLotManager();
	virtual ~AMTLotManager();

	void OnNewLotIn( CString strLotID, CString strPartID );
	CString GetLotIDAt( int iIdx );
	ALot& GetLotAt( int iIdx );
	ALot& GetLotByLotID( CString strLotID );
	int GetLotCount() { return m_vecLot.size(); }
	void DeleteLot( CString strLotID );
	void DeleteLot( int iIdx );

protected:
	std::vector< ALot > m_vecLot;
	ALot	m_dummyLot;
};

extern AMTLotManager g_lotMgr;

#endif // !defined(AFX_AMTLOTMANAGER_H__4FDFD96A_39CA_476A_A470_F73B06CE8398__INCLUDED_)
