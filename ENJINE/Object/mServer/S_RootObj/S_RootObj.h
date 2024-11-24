#pragma once
#include "../../S_Object.h"

class S_PlayerObj;

class S_RootObj : public S_Object
{
private:
	//std::vector<S_PlayerObj*> m_players;
public:
	S_RootObj();
	~S_RootObj();
	virtual void onProcess();
};

