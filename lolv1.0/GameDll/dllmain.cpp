// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "dllmain.h"
#include "SkillServices.h"
#include "BufferServices.h"
#include "EquipmentServices.h"
#include "MonsterServices.h"
#include "BaseAddr.h"
#include "CShareStruct.h"
#include "ShareMemory.h"
#include "GameCall.h"
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: {
		::CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(ThreadProc), NULL, NULL, NULL);
		break;
	}
	case DLL_THREAD_ATTACH: {
		break;
	}
	case DLL_THREAD_DETACH: {
		break;
	}
	case DLL_PROCESS_DETACH: {
		break;
	}
	}
	return TRUE;
}

DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
{
	CMonsterServices cm;

	
	//�ж��Ƿ������Ϸ
	while ((DWORD)GameCall::GetInstance()->GetClientTickTime() < 1 || utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr) < 1)
	{
		Sleep(3000);
	}
	//HOOK����CALL
	if (!GameCall::GetInstance()->HookSkillUse())
	{
		utils::GetInstance()->log("ERROR: ameCall::GetInstance()->HookSkillUse()ʧ�ܣ�");
		return 0;
	}

	//���������ڴ�
	std::shared_ptr<ShareMemory<SHARED_MEMORY>> m_pSharedMemory(new ShareMemory<SHARED_MEMORY>(MAP_NAME));
	if (!m_pSharedMemory->openShareMemory())
	{
		utils::GetInstance()->log("ERROR: m_pSharedMemory->openShareMemory()���ִ���\n");
		return 0;
	}
	auto pSharedMemoryPointer = m_pSharedMemory->GetPointerOfFile();


	//������Ҷ���
	
	CSkillServices m_roleSkill(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));

	utils::GetInstance()->log("TIPS: �����ɹ���\n");
	while (true)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			person m_role(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
			//��������˼���Q
			if (pSharedMemoryPointer->bLockQ)
			{
				//auto mons = cm.GetNearleastPerson(&m_role);
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(0);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				//��� (�����ҵľ��� < ����Q�ľ��� && ��ҵ�ǰ���� > �������ĵ��� && �����Ѿ�ѧϰ && �����Ѿ���ȴ && ��һ���)  �͵��� ������CALL��Q������
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					m_role.GetDistance(&mons.GetPoint()) >0 &&
					skillQ.GetLevel() > 0 && 
					m_role.GetCurMp() > skillQ.GetExpendMP()&&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: ��ǰ��������:%x %s ʹ�ü���Q��",mons.GetNodeBase() ,Utf8ToAnsi(mons.GetName()).c_str());
					GameCall::GetInstance()->UseSkill(0, mons.GetNodeBase());
				}
			}
			if (pSharedMemoryPointer->bLockW)
			{
				//auto mons = cm.GetNearleastPerson(&m_role);
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(1);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				//��� (�����ҵľ��� < ����Q�ľ��� && ��ҵ�ǰ���� > �������ĵ��� && �����Ѿ�ѧϰ && �����Ѿ���ȴ && ��һ���) && ���˿ɼ� �͵��� ������CALL��Q������
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: ��ǰ��������:%x %s ʹ�ü���W��", mons.GetNodeBase(), Utf8ToAnsi(mons.GetName()).c_str());
					GameCall::GetInstance()->UseSkill(1, mons.GetNodeBase());
				}
			}
			if (pSharedMemoryPointer->bLockE)
			{
				//auto mons = cm.GetNearleastPerson(&m_role);
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(2);
				utils::GetInstance()->log("TIPS: ��������:%s", skillQ.GetName());
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				//��� (�����ҵľ��� < ����Q�ľ��� && ��ҵ�ǰ���� > �������ĵ��� && �����Ѿ�ѧϰ && �����Ѿ���ȴ && ��һ���)  �͵��� ������CALL��Q������
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: ��ǰ��������:%x %s ʹ�ü���E��", mons.GetNodeBase(), Utf8ToAnsi(mons.GetName()).c_str());
					GameCall::GetInstance()->UseSkill(2, mons.GetNodeBase());
				}
			}


			//���������A
			//��� �������ҵľ��� < �������� && ��ʱ > ��A��ʱ�� �͵��� Ѱ·�������λ�ã� �� ������ͨ����CALL
		}

		//���������T & ���Զ��ͷ�R
		if (GetAsyncKeyState(0x54)&0x8000)
		{
			person m_role(utils::GetInstance()->read<DWORD>(Base_RoleSelfAddr));
			if (pSharedMemoryPointer->bLockR)
			{
				//auto mons = cm.GetNearleastPerson(&m_role);
				auto skillQ = m_roleSkill.GetSkillObjectByIndex(3);
				auto mons = cm.GetHealthLeastPerson(&m_role, skillQ.GetSkillRange());
				//��� (�����ҵľ��� < ����Q�ľ��� && ��ҵ�ǰ���� > �������ĵ��� && �����Ѿ�ѧϰ && �����Ѿ���ȴ && ��һ���)  �͵��� ������CALL��Q������
				if (m_role.GetDistance(&mons.GetPoint()) < skillQ.GetSkillRange() &&
					skillQ.GetLevel() > 0 &&
					m_role.GetCurMp() > skillQ.GetExpendMP() &&
					!m_role.BDead() &&
					skillQ.bCoolDown())
				{
					utils::GetInstance()->log("TIPS: ��ǰ��������:%x %s ʹ�ü���R��", mons.GetNodeBase(), Utf8ToAnsi(mons.GetName()).c_str());
					GameCall::GetInstance()->UseSkill(3, mons.GetNodeBase());
				}
			}
		}

		Sleep(1);
	}

	return 0;
}
