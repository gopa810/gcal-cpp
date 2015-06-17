// FrameServer.h: interface for the CFrameServer class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "FrameMain.h"
#include "FrameFind.h"
#include "FrameLocs.h"
#include "FrameEvents.h"
#include "afxtempl.h"

class CFrameServer  
{
	CList <CFrameMain *, CFrameMain *> m_Arr;
public:
	void UpdateAllEventFrames();
	void RecalculateAllTodayScreens();
	void RemoveFromEventFrames();
	void CloseAllEventFrames();
	void CreateNewEventsFrame();
	void Initialize();
	void RemoveFromLocs();
	void CloseAllLocs();
	void CreateNewLocationFrame();
	CFrameEvents * m_pEvents;
	CFrameFind * m_pFinder;
	CFrameLocs * m_pLocs;
	void RemoveFromFinders();
	void CloseAllFinders();
	HICON m_mainIcon;
	void CreateNewFindFrame();
	void ChangeMainWindow(CFrameMain * pFrame);
	void CloseAllWindows();
	void UnlockServer();
	BOOL m_bLocked;
	CRect m_rectMain;
	CRect m_rectFind;
	void LockServer();
	void OnClose(CFrameMain * pFrame);
	CFrameMain * CreateNewFrame();
	CFrameServer();
	virtual ~CFrameServer();

};

