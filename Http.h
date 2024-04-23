#pragma once
#include "APICore.h"

class Http {

public:
	Http();
	~Http();

	float getCurrency(char* from, char* to);
private:
	HINTERNET hSession;
	HINTERNET hConnect;
	HINTERNET hRequest;
};

