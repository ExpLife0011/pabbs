
// ConsoleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Console.h"
#include "ConsoleDlg.h"
#include "afxdialogex.h"
#include "ShareMemory.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "InjectDll.h"

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

DWORD WINAPI ThreadProcA(_In_ LPVOID lpParameter)
{
	//���Ӵ��ڵĻص�����
	CInjectDll injectClass("", "RiotWindowClass", "GameDll.dll");
	while (true)
	{
		injectClass.injectDll();
		Sleep(1000);
	}
}

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CConsoleDlg �Ի���



CConsoleDlg::CConsoleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONSOLE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CConsoleDlg::~CConsoleDlg()
{

}

void CConsoleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_bLockQ);
	DDX_Control(pDX, IDC_CHECK2, m_bLockW);
	DDX_Control(pDX, IDC_CHECK3, m_bLockE);
	DDX_Control(pDX, IDC_CHECK4, m_bLockR);
	DDX_Control(pDX, IDC_CHECK5, m_bLockQAA);
}

BEGIN_MESSAGE_MAP(CConsoleDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK1, &CConsoleDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CConsoleDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CConsoleDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CConsoleDlg::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CConsoleDlg::OnBnClickedCheck5)
END_MESSAGE_MAP()


// CConsoleDlg ��Ϣ��������

BOOL CConsoleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	//���������ڴ�
	if (!m_sharedMemory.CreateSharedMemory())
	{
		AfxMessageBox("���������ڴ�ʧ�ܣ��Ƿ��������ͬ���Ĵ��ڣ�");
		return FALSE;
	}

	//����ע���߳�
	m_ThreadHanle = ::CreateThread(NULL, NULL,LPTHREAD_START_ROUTINE(ThreadProcA) , NULL, NULL, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CConsoleDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CConsoleDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CConsoleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CConsoleDlg::OnBnClickedCheck1()
{
	m_sharedMemory.GetPointerOfMapView()->bLockQ = m_bLockQ.GetCheck() == BST_CHECKED ? true : false;
}

void CConsoleDlg::OnBnClickedCheck2()
{
	m_sharedMemory.GetPointerOfMapView()->bLockW = m_bLockW.GetCheck() == BST_CHECKED ? true : false;
}


void CConsoleDlg::OnBnClickedCheck3()
{
	m_sharedMemory.GetPointerOfMapView()->bLockE = m_bLockE.GetCheck() == BST_CHECKED ? true : false;
}


void CConsoleDlg::OnBnClickedCheck4()
{
	m_sharedMemory.GetPointerOfMapView()->bLockR = m_bLockR.GetCheck() == BST_CHECKED ? true : false;
}


void CConsoleDlg::OnBnClickedCheck5()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_sharedMemory.GetPointerOfMapView()->bOpenAA = m_bLockQAA.GetCheck() == BST_CHECKED ? true : false;
}

