#pragma once
#include <vector>
#include "skill.h"
class CSkillServices
{
public:
	CSkillServices(DWORD dwObjectBase);
	~CSkillServices();

	//��ɶ���ı���
	void travse();
	skill GetSkillObjectByIndex(DWORD dwIndex);
private:
	std::vector<skill> m_skillList;
	DWORD m_dwObjectBase;
};
