#pragma once

class Stage final
{
public: //�V���O���g����
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;
};
