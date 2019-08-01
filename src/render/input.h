#ifndef INPUT_H
#define INPUT_H

#include "render/render.h"
#include "phys/sys.h"

float dist;

void scrollCallback(GLFWwindow *window, double xoffs, double yoffs) {
	dist += -(float) yoffs / 10.0f;
}

void r_getInput(struct Renderer *r, struct Sys* s)
{
	glfwSetScrollCallback(r->window.window, scrollCallback);

	r->window.mouseOldX = r->window.mouseX;
	r->window.mouseOldY = r->window.mouseY;

	glfwGetCursorPos(r->window.window, &r->window.mouseX, &r->window.mouseY);

	r->window.mouseDiffX = r->window.mouseOldX - r->window.mouseX;
	r->window.mouseDiffY = r->window.mouseOldY - r->window.mouseY;

	if (glfwGetMouseButton(r->window.window, GLFW_MOUSE_BUTTON_LEFT)) {
		r->model = p_mat4RotateY(&r->model, (float) r->window.mouseDiffX * 0.004f);
		//r->model = p_mat4RotateX(&r->model, (float) r->window.mouseDiffY * 0.004f);
	}

	float ms = r->delta * 0.01f; // mov spd
	float rs = r->delta * 0.02f; // rot spd

	if (glfwGetKey(r->window.window, GLFW_KEY_W)) {
		r->camPos.z += ms * cosf(r->camRot.y);
		r->camPos.x -= ms * sinf(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_S)) {
		r->camPos.z -= ms * cosf(r->camRot.y);
		r->camPos.x += ms * sinf(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_D)) {
		r->camPos.x += ms * cosf(r->camRot.y);
		r->camPos.z += ms * sinf(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_A)) {
		r->camPos.x -= ms * cosf(r->camRot.y);
		r->camPos.z -= ms * sinf(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_R)) {
		r->camPos.y += ms;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_F)) {
		r->camPos.y -= ms;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_UP)) {
		r->camRot.x += rs;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_DOWN)) {
		r->camRot.x -= rs;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_LEFT)) {
		r->camRot.y += rs;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_RIGHT)) {
		r->camRot.y -= rs;
	}
}

#endif //INPUT_H
