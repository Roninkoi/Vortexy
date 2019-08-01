#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

struct Window {
	GLFWwindow *window;

	int width;
	int height;

	int swapInterval;

	GLFWmonitor *monitor;

	double mouseX;
	double mouseY;

	double mouseOldX;
	double mouseOldY;

	double mouseDiffX;
	double mouseDiffY;
};

void r_createWindow(struct Window *w);

void r_destroyWindow(struct Window *w);

void r_updateWindow(struct Window *w, int *running);

#endif
