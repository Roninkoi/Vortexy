#ifndef INPUT_H
#define INPUT_H

#include <util/util.h>
#include "window.h"
#include "render.h"
#include "phys/sys.h"

void scrollCallback(GLFWwindow *window, double xoffs, double yoffs) {
	scroll += -(float) yoffs / 10.0f;
}

int ctrlDown;
int shiftDown;

int n1Down;
int n2Down;
int n3Down;
int n4Down;
int n5Down;
int n6Down;

int uDown;

void r_getInput(struct Renderer *r, struct Sys* s)
{
	glfwSetScrollCallback(r->window.window, scrollCallback);

	r->window.mouseOldX = r->window.mouseX;
	r->window.mouseOldY = r->window.mouseY;

	glfwGetCursorPos(r->window.window, &r->window.mouseX, &r->window.mouseY);

	r->window.mouseDiffX = r->window.mouseOldX - r->window.mouseX;
	r->window.mouseDiffY = r->window.mouseOldY - r->window.mouseY;

	if (glfwGetMouseButton(r->window.window, GLFW_MOUSE_BUTTON_LEFT)) {
		//r->model = mat4RotateY(&r->model, (float) r->window.mouseDiffX * 0.004f);
		//r->model = mat4RotateX(&r->model, (float) r->window.mouseDiffY * 0.004f);
		r->modelRot.y += (float) r->window.mouseDiffX * 0.0024f;
	}

	float ms = r->delta * 0.01f; // mov spd
	float rs = r->delta * 0.02f; // rot spd

	if (fabs(scroll) > 0.0f) { // mouse scroll = "zoom"
		r->camPos.z -= 1.0f * scroll * cosf(r->camRot.y);
		r->camPos.y -= 1.0f * scroll * sinf(r->camRot.x);
		r->camPos.x += 1.0f * scroll * sinf(r->camRot.y);
		scroll = 0.0f;
	}

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
	
	if (glfwGetKey(r->window.window, GLFW_KEY_LEFT_CONTROL)) {
		if (!ctrlDown) {
			ctrlDown = 1;
			--s->selected;
			if (s->selected < 0)
				s->selected = 0;

			printf("Selected: %i\n", s->selected);
		}
	}
	else {
		ctrlDown = 0;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_LEFT_SHIFT)) {
		if (!shiftDown) {
			shiftDown = 1;
			++s->selected;

			printf("Selected: %i\n", s->selected);
		}
	}
	else {
		shiftDown = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_1)) {
		if (!n1Down) {
			n1Down = 1;
			r->vis = setBit(r->vis, 1, !getBit(r->vis, 1));
		}
	}
	else {
		n1Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_2)) {
		if (!n2Down) {
			n2Down = 2;
			r->vis = setBit(r->vis, 2, !getBit(r->vis, 2));
		}
	}
	else {
		n2Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_3)) {
		if (!n3Down) {
			n3Down = 1;
			r->vis = setBit(r->vis, 3, !getBit(r->vis, 3));
		}
	}
	else {
		n3Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_4)) {
		if (!n4Down) {
			n4Down = 1;
			r->vis = setBit(r->vis, 4, !getBit(r->vis, 4));
		}
	}
	else {
		n4Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_5)) {
		if (!n5Down) {
			n5Down = 1;
			r->vis = setBit(r->vis, 5, !getBit(r->vis, 5));
		}
	}
	else {
		n5Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_6)) {
		if (!n6Down) {
			n6Down = 1;
			r->vis = setBit(r->vis, 6, !getBit(r->vis, 6));
		}
	}
	else {
		n6Down = 0;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_I)) {
		if (!s->simulating)
			printf("STARTED\n");
		
		s->simulating = 1;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_O)) {
		if (s->simulating)
			printf("STOPPED\n");
		
		s->simulating = 0;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_U)) {
		if (!uDown) {
			uDown = 1;
			printf("RESET\n");
		
			s->reset = 1;
		}
	}
	else {
		uDown = 0;
	}
}

#endif //INPUT_H
