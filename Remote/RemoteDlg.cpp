
// RemoteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Remote.h"
#include "RemoteDlg.h"
#include "afxdialogex.h"
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <iostream>
#include<fstream>
#include <psapi.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteDlg 对话框



CRemoteDlg::CRemoteDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRemoteDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_tree);
	DDX_Control(pDX, IDC_MFCSHELLLIST3, m_shelllist);
	DDX_Control(pDX, IDC_LIST2, m_list);
}

BEGIN_MESSAGE_MAP(CRemoteDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_NOTIFY(NM_DBLCLK, IDC_MFCSHELLLIST3, &CRemoteDlg::OnNMDblclkMfcshelllist3)
ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST3, &CRemoteDlg::OnClickMfcshelllist3)
ON_BN_CLICKED(IDOK, &CRemoteDlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CRemoteDlg::OnBnClickedCancel)
ON_NOTIFY(NM_CLICK, IDC_LIST2, &CRemoteDlg::OnClickList2)
ON_BN_CLICKED(IDC_BUTTON1, &CRemoteDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRemoteDlg 消息处理程序

BOOL CRemoteDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	DWORD styles = m_list.GetExtendedStyle();
	m_list.SetExtendedStyle(styles | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_list.DeleteAllItems();//先清空
	//set the list's titles
	TCHAR rgtsz[][6] = { _T("进程"), _T("PID") };
	LV_COLUMN lvcolumn;
	CRect rect;
	m_list.GetWindowRect(&rect);//获取list的宽度
	for (int i = 0; i < 2; i++)
	{
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER;
		lvcolumn.fmt = LVCFMT_LEFT;
		lvcolumn.pszText = rgtsz[i];
		lvcolumn.iSubItem = i;
		lvcolumn.iOrder = i;
		lvcolumn.cx = rect.Width() / 2;
		m_list.InsertColumn(i, &lvcolumn);
	}
	m_tree.Expand(m_tree.GetRootItem(), TVE_EXPAND);
	m_tree.SetRelatedList(&m_shelllist);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteDlg::OnClickMfcshelllist3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CString FilePath;

	for (int i = 0; i < m_shelllist.GetItemCount();i++)
	{
		if (m_shelllist.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED){
			m_shelllist.GetItemPath(FilePath,i);
			GetDlgItem(IDC_EDIT1)->SetWindowTextW(FilePath);
			break;
		}
	}
	*pResult = 0;
}
void CRemoteDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	setlocale(LC_ALL, "chs");
	// 1. 使用快照来遍历进程
	// 1.1 创建进程快照
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hProcSnap) {
		return;
	}
	//typedef struct tagPROCESSENTRY32W
	//{
	//    DWORD   dwSize;                 // 结构体大小
	//    DWORD   cntUsage;               /// 不使用
	//    DWORD   th32ProcessID;          // this process
	//    ULONG_PTR th32DefaultHeapID;    /// 不使用
	//    DWORD   th32ModuleID;           /// 不使用
	//    DWORD   cntThreads;             // 进程的线程个数
	//    DWORD   th32ParentProcessID;    // 父进程ID
	//    LONG    pcPriClassBase;         // 进程优先级
	//    DWORD   dwFlags;                /// 不使用
	//    WCHAR   szExeFile[ MAX_PATH ];  // 进程名
	//} PROCESSENTRY32W;
	PROCESSENTRY32 processInfo;
	// 初始化结构体的结构体字节字段
	processInfo.dwSize = sizeof(processInfo);
	// 获取第一个进程
	Process32First(hProcSnap, &processInfo);
	int i = 0;
	BOOL bTrue = FALSE;
	BOOL bRet = FALSE;
	TCHAR path[MAX_PATH];
	DWORD dwPathLengh = MAX_PATH;
	HANDLE hProc;
	CString PID = L"";
	do
	{
		hProc = OpenProcess(PROCESS_QUERY_INFORMATION,
			FALSE, processInfo.th32ProcessID);

		PID.Format(L"%d", processInfo.th32ProcessID);
		m_list.InsertItem(i,L"");
		m_list.SetItemText(i, 0, processInfo.szExeFile);
		m_list.SetItemText(i, 1, PID);
		i++;
	} while (Process32Next(hProcSnap, &processInfo));
}


void CRemoteDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CString PID,PATH;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(PATH);
	GetDlgItem(IDC_EDIT2)->GetWindowTextW(PID);
	DWORD pid = _ttoi(PID);
	TCHAR* path = PATH.GetBuffer();
	HANDLE hProcess = OpenProcess(
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		NULL, pid);
	if (!hProcess)
		return ;
	//C:\\tel\\MyRecv.dll
	int nLen = sizeof(WCHAR)*(wcslen(path) + 1);
	LPVOID pBuf = VirtualAllocEx(hProcess, NULL, nLen, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!pBuf)
		return ;
	DWORD dwWrite = 0;
	if (!WriteProcessMemory(hProcess, pBuf, path, nLen, &dwWrite))
		return ;
	// LoadLibrary
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, NULL,
		(LPTHREAD_START_ROUTINE)LoadLibraryW, pBuf, 0, 0);
	WaitForSingleObject(hRemoteThread, -1);
	if (!hRemoteThread)
		return ;
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pBuf, 0, MEM_FREE);
	MessageBoxA(0, "注入成功", "", 0);
}


void CRemoteDlg::OnClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	CString PID;

	for (int i = 0; i < m_list.GetItemCount(); i++)
	{
		if (m_list.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED){
			PID = m_list.GetItemText(i,1);
			GetDlgItem(IDC_EDIT2)->SetWindowTextW(PID);
			break;
		}
	}
	*pResult = 0;
}


void CRemoteDlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	exit(0);
}
