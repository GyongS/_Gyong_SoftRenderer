#pragma once
#include "afxwin.h"
#include "EditorScene.h"


// CObjectListView 폼 뷰입니다.

class CObjectListView : public CFormView
{
	DECLARE_DYNCREATE(CObjectListView)

protected:
	CObjectListView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CObjectListView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTLISTVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedUP();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedForward();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnLbnSelchangeObjectList();
	CListBox m_ListBox;
	std::wstring m_wsBlockColor = L"";
	std::wstring m_wsBlockType = L"";
public:
	GETTOR_SETTOR(_uint, m_iSelect, 0, Select);
private:
	int m_iCnt = 0;
public:
	afx_msg void OnBnClickedWhite();
	afx_msg void OnBnClickedRed();
	afx_msg void OnBnClickedBlue();
	afx_msg void OnBnClickedGreen();
	afx_msg void OnBnClickedYellow();
};


