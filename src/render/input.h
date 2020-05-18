#ifndef INPUT_H
#define INPUT_H

#include <util/util.h>
#include "window.h"
#include "render.h"
#include "phys/sys.h"

int ctrlDown;
int shiftDown;

int n1Down;
int n2Down;
int n3Down;
int n4Down;
int n5Down;
int n6Down;
int n7Down;

int uDown;

int lmbDown;

float msx;
float msy;
float selx;
float sely;
int kset;

void scrollCallback(GLFWwindow *window, double xoffs, double yoffs) {
	scroll += -(float) yoffs / 10.0f;
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
		//r->model = mat4RotateY(&r->model, (float) r->window.mouseDiffX * 0.004f);
		//r->model = mat4RotateX(&r->model, (float) r->window.mouseDiffY * 0.004f);
		r->modelRot.y += (float) r->window.mouseDiffX * 0.0024f;

		if (getBit(r->vis, 6) && !lmbDown) {
			lmbDown = 1;

			int ind = -1;

			float minz = 1000.0f;

			mat4 mi = mat4Mul(&r->proj, &r->view);
			mi = mat4Mul(&mi, &r->model);

			vec4 mp = Vec4((float) r->window.mouseX, -(float) r->window.mouseY, 0.0f, 0.0f);
			mp.x /= (float) r->width * 0.5f;
			mp.y /= (float) r->height * 0.5f;
			mp.x -= 1.0f;
			mp.y += 1.0f;

			msx = mp.x;
			msy = mp.y;

			for (int i = 0; i < s->objs[0].mesh.indNum / 3; i += 1) {
				float x = s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3] * 4];
				x += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 1] * 4];
				x += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 2] * 4];
				x /= 3.0f;

				float y = s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3] * 4 + 1];
				y += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 1] * 4 + 1];
				y += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 2] * 4 + 1];
				y /= 3.0f;

				float z = s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3] * 4 + 2];
				z += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 1] * 4 + 2];
				z += s->objs[0].mesh.vertData[s->objs[0].mesh.indData[i * 3 + 2] * 4 + 2];
				z /= 3.0f;

				vec4 p = Vec4(x, y, z, 1.0f);
				p = mat4MulV(&mi, &p);
				p.x /= fabs(p.z);
				p.y /= fabs(p.z);

				if (p.z < 0.0)
					continue;

				p.z = 0.0;
				p.w = 0.0;

				if (kset && s->objs[0].faces[i].vNum == 1) {
					if (((p.x < mp.x && p.x > selx) || (p.x > mp.x && p.x < selx)) && ((p.y < mp.y && p.y > sely) || (p.y > mp.y && p.y < sely))) {
						s->objs[0].faces[i].boundary = 1;
					}
				}

				vec4Sub(&p, &mp);

				float cz = vec4Len3(&p);

				if (i == s->selected)
					continue;

				if (cz < minz) {
					minz = cz;
					ind = i;
				}
			}
			if (kset) {
				kset = 0;
			}

			printf("face index: %i\n", ind);

			if (ind >= 0) {
				s->selected = ind;

				printf("face v: ");
				vec3Print(&s->objs[0].faces[ind].v);
				printf("face pGrad: ");
				vec3Print(&s->objs[0].faces[ind].pGrad);
				printf("face p: %f\n", s->objs[0].faces[ind].p);
			}
		}
	}
	else {
		lmbDown = 0;
	}

	float ms = r->delta * r->rs * 0.1f; // mov spd
	float rs = r->delta * 0.03f; // rot spd

	if (fabs(scroll) > 0.0f) { // mouse scroll = "zoom"
		r->camPos.z -= 1000.0f * scroll * cos(r->camRot.y) * r->rs * 0.01f;
		r->camPos.y -= 1000.0f * scroll * sin(r->camRot.x) * r->rs * 0.01f;
		r->camPos.x += 1000.0f * scroll * sin(r->camRot.y) * r->rs * 0.01f;
		scroll = 0.0f;
	}

	if (glfwGetKey(r->window.window, GLFW_KEY_W)) {
		r->camPos.z += ms * cos(r->camRot.y);
		r->camPos.x -= ms * sin(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_S)) {
		r->camPos.z -= ms * cos(r->camRot.y);
		r->camPos.x += ms * sin(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_D)) {
		r->camPos.x += ms * cos(r->camRot.y);
		r->camPos.z += ms * sin(r->camRot.y);
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_A)) {
		r->camPos.x -= ms * cos(r->camRot.y);
		r->camPos.z -= ms * sin(r->camRot.y);
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

	if (glfwGetKey(r->window.window, GLFW_KEY_7)) {
		if (!n7Down) {
			n7Down = 1;
			r->vis = setBit(r->vis, 7, !getBit(r->vis, 7));
		}
	}
	else {
		n7Down = 0;
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

	if (glfwGetKey(r->window.window, GLFW_KEY_B)) {
		s->debugFlag = 1;
	}
	if (glfwGetKey(r->window.window, GLFW_KEY_K)) {
		selx = msx;
		sely = msy;
		kset = 1;
	}
}

#endif //INPUT_H
