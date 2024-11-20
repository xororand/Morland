#pragma once
#include "../../S_Object.h"

class Peer;

class S_PlayerObj : public S_Object
{
private:
	Peer* m_peer = nullptr;
public:
	S_PlayerObj(Peer* peer);
	~S_PlayerObj();
	virtual void onProcess();
};

