#pragma once

class GameInstance;

namespace GLUTCallback
{
	void Init(GameInstance* gl);

	void Display();
	void Timer(int preferredRefresh);
	void Keyboard(unsigned char key, int x, int y);
	void KeyboardUp(unsigned char key, int x, int y);
	void ActiveMouseMotion(int x, int y);
	
};

