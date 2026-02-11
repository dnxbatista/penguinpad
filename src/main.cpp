/*
 * PenguinPad - Lightweight Gamepad Testing Tool
 * Copyright (c) 2026 PenguinPad Contributors
 * 
 * This software is provided under the MIT License.
 * See LICENSE file for details.
 */

#include <windows.h>
#include <iostream>
#include "app.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	App myApp("penguinpad", 1280, 720);

	if (!myApp.init()) {
		return -1;
	}

	myApp.run();

	return 0;
}