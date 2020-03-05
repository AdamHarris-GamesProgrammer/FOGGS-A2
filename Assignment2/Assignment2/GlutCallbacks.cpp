#include "GlutCallbacks.h"
#include "GameInstance.h"

namespace GLUTCallback {
	namespace {
		GameInstance* gameInstance = nullptr;
	}

	void Init(GameInstance* gl) {
		if (gl != nullptr) {
			gameInstance = gl;
		}
	}

	void Display() {
		if (gameInstance != nullptr) {
			gameInstance->Render();
		}
	}

	void Timer(int preferredRefresh) {
		gameInstance->Update();

		glutTimerFunc(preferredRefresh, GLUTCallback::Timer, preferredRefresh);
	}
}