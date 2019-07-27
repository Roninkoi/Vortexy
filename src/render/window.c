#include "render/window.h"

void r_createWindow(struct Window *w)
{
	w->width = WINDOW_WIDTH;
	w->height = WINDOW_HEIGHT;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	w->monitor = glfwGetPrimaryMonitor();

	w->window = glfwCreateWindow(w->width, w->height, "Vortexy", NULL, NULL);

	glfwMakeContextCurrent(w->window);
}

void r_updateWindow(struct Window *w, int *running)
{
	glfwSwapBuffers(w->window);

	glfwPollEvents();

	if (glfwWindowShouldClose(w->window) != 0) {
		*running = 0;
	}

	glfwGetWindowSize(w->window, &w->width, &w->height);
}

void r_destroyWindow(struct Window *w)
{
	if (w->width * w->height > 0) {
		glfwDestroyWindow(w->window);

		w->width = 0;
		w->height = 0;
	}
}
