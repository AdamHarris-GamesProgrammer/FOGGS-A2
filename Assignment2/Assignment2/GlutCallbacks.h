#pragma once

class GameInstance;

namespace GLUTCallback
{
	void Init(GameInstance* gl);

	void Display();
	void Timer(int preferredRefresh);
	
};

