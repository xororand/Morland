#pragma once
#include "../../S_Object.h"

class S_RootObj : public S_Object
{
public:
	S_RootObj();
	~S_RootObj();
	virtual void onProcess();
};

