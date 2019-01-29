
// RemoteDlg.h : 头文件
//

#pragma once
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include "afxcmn.h"


// CRemoteDlg 对话框
class CRemoteDlg : public CDialogEx
{
// 构造
public:
	CRemoteDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CMFCShellTreeCtrl m_tree;
	CMFCShellListCtrl m_shelllist;
//	afx_msg void OnNMDblclkMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListCtrl m_list;
	afx_msg void OnClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
};
