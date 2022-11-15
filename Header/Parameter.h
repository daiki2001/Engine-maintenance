#pragma once
#include <map>

/*ƒV[ƒ“‚ğ‚Ü‚½‚®—p*/

class Parameter final
{
private:
	Parameter() {}
	Parameter(const Parameter&) = delete;
	~Parameter() {}
	Parameter operator=(const Parameter&) = delete;
public:
	static void Set(const char* key, int vel);
	static int Get(const char* key);

private:
	static std::map<const char*, int> param;

};
