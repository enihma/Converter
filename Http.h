#pragma once
#include "APICore.h"

class Http {

public:
	Http();
	~Http();

	float getCurrency(char* from, char* to);

	bool statusConnect = false;
private:
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hRequest;
};

