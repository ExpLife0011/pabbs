#include "stdafx.h"
#include "InjectDll.h"




CInjectDll::CInjectDll(std::string wndName, std::string className, std::string dllName)
{
	m_dllName = dllName;
	m_wndName = wndName;
	m_className = className;
	m_bInjectDllSuccess = false;
}

CInjectDll::~CInjectDll()
{
}

bool CInjectDll::injectDll()
{
	//����Ѿ�ע�룬��ֱ�ӷ���

	DWORD pid = 0; //����pid  
	HANDLE hProcess = NULL; //���̾��  
	LPDWORD AddressDW = NULL;
	DWORD dwwrite = 0;
	DWORD dwthreadid = 0;

	HANDLE threadHandle = NULL;

	//��ȡ��Ϸ���ھ��  
	auto GameHwnd = FindWindow("RiotWindowClass", NULL);
	if (GameHwnd != 0) {

		//�ж��Ƿ�ע��
		if (m_bInjectDllSuccess)
		{
			return true;
		}
		//��ȡ����pid  
		GetWindowThreadProcessId(GameHwnd, &pid);
		if (pid != 0) {
			//��ȡ��Ϸ���̾��  
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			if (hProcess != NULL) {
				//����Ϸ���̷���һ���ڴ�ռ�  
				AddressDW = (LPDWORD)VirtualAllocEx(hProcess, NULL, 256, MEM_COMMIT, PAGE_READWRITE);
				if (AddressDW != NULL) {
					//д��dllȫ··��  
					std::string temp;
					temp.resize(300);
					temp.resize(GetCurrentDirectory(temp.size(), const_cast<char*>(temp.c_str())));
					temp.append("\\");
					temp.append(m_dllName);
					WriteProcessMemory(hProcess, AddressDW, temp.c_str(),temp.size(), &dwwrite);
					if (dwwrite >= temp.size()) {
						threadHandle = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, AddressDW, NULL, NULL);
						if (threadHandle != NULL) {
							m_bInjectDllSuccess = true;
							//AfxMessageBox("ע��ɹ���������");
						}
						WaitForSingleObject(threadHandle, 0xFFFFFFFF);
						CloseHandle(threadHandle);
					}
					else {
						m_bInjectDllSuccess = false;
					}
					VirtualFreeEx(hProcess, AddressDW, 256, MEM_DECOMMIT);
					CloseHandle(hProcess);
					return m_bInjectDllSuccess;
				}
				else
				{
					m_bInjectDllSuccess = false;
					return false;
				}

			}
			else
			{
				m_bInjectDllSuccess = false;
				return false;
			}
		}
		else
		{
			m_bInjectDllSuccess = false;
			return false;
		}
	}
	else
	{
		m_bInjectDllSuccess = false;
		return false;
	}
	return true;
}

bool CInjectDll::getResult() const
{
	return m_bInjectDllSuccess;
}