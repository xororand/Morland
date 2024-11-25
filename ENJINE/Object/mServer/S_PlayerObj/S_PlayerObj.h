#pragma once
#include "../../S_Object.h"
#include "box2d/box2d.h"

class Peer;

class S_PlayerObj : public S_Object
{
private:
	Peer* m_peer		= nullptr;
	b2BodyId m_bodyId	= b2_nullBodyId; // id тела
public:
	S_PlayerObj(Peer* peer);
	~S_PlayerObj();

	Peer* getPeer()			{ return m_peer; }
	b2BodyId getBodyID()	{ return m_bodyId; }

	virtual void save();
	virtual void onProcess();
};

