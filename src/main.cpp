/*
 * PenguinPad - Lightweight Gamepad Testing Tool
 * Copyright (c) 2026 PenguinPad Contributors
 * 
 * This software is provided under the MIT License.
 * See LICENSE file for details.
 */

#include <iostream>
#include "app.h"

int main(int argc, char* argv[])
{
	App myApp("penguinpad", 1280, 720);

	if (!myApp.init()) {
		return -1;
	}

	myApp.run();

	return 0;
}