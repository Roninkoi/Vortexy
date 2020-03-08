#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "phys/mesh.h"
#include "util.h"

#define OBJ_MAX 0x100000

// creates a mesh object and loads .obj
Mesh objParser(char *path, int optimize, int loadTex)
{
	Mesh mesh;

	p_meshInit(&mesh);

	int wordNum = 0;
	char **words = wordsFromFile(path, OBJ_MAX, &wordNum);

	float *verts = NULL;
	float *texes = NULL;

	int vn = 0;
	int tn = 0;

	int *vertis = NULL;
	int *texis = NULL;

	int vin = 0;
	int tin = 0;

	for (int i = 0; i < wordNum; ++i) {
		//printf("%s\n", words[i]);

		if (strcmp(words[i], "v") == 0) { // verts
			float v0 = atof(words[i + 1]);
			float v1 = atof(words[i + 2]);
			float v2 = atof(words[i + 3]);

			verts = floatAppend(verts, v0, vn++);

			verts = floatAppend(verts, v1, vn++);

			verts = floatAppend(verts, v2, vn++);
		}

		if (strcmp(words[i], "vt") == 0) { // texes
			float v0 = atof(words[i + 1]);
			float v1 = atof(words[i + 2]);

			texes = floatAppend(texes, v0, tn++);

			texes = floatAppend(texes, v1, tn++);
		}

		if (strcmp(words[i], "f") == 0) { // faces
			for (int f = 0; f < 3; ++f) {
				char *face = words[i + f + 1];
				char *vs = NULL;
				char *ts = NULL;
				int vb = 1;

				for (int j = 0; j < strlen(face) && (face[j] != '/' || vb); ++j) { // parse faces
					if (face[j] != '/') {
						if (vb) {
							vs = strAppend(vs, face[j]);
						}
						else {
							ts = strAppend(ts, face[j]);
						}
					}
					else {
						vb = 0;
					}
				}

				int vi = atoi(vs) - 1; // obj starts at 1
				int ti = 0;
				if (ts != NULL)
					ti = atoi(ts) - 1;

				if (!loadTex) {
					ti = 0;
				}

				int exists = 0;
				int t = 0;

				for (; !exists && t < vin; ++t) { // optimize meshes
					if (vi == vertis[t] && ti == texis[t]) {
						exists = 1;
						t -= 1;
					}
				}

				if (!optimize) {
					exists = 0;
				}

				if (exists) {
					mesh.indData = intAppend(mesh.indData, t, mesh.indNum++);
				}
				else {
					vertis = intAppend(vertis, vi, vin++);
					texis = intAppend(texis, ti, tin++);

					int l = vin - 1;

					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3], mesh.vertNum++);
					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3 + 1], mesh.vertNum++);
					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3 + 2], mesh.vertNum++);
					mesh.vertData = floatAppend(mesh.vertData, 0.0f, mesh.vertNum++);

					float tdx = 0.0f;
					float tdy = 0.0f;

					if (texes != NULL) {
						tdx = texes[texis[l] * 2];
						tdy = texes[texis[l] * 2 + 1]; // -1?
					}

					mesh.texData = floatAppend(mesh.texData, tdx, mesh.texNum++);
					mesh.texData = floatAppend(mesh.texData, tdy, mesh.texNum++);
					mesh.texData = floatAppend(mesh.texData, 1.0f, mesh.texNum++);
					mesh.texData = floatAppend(mesh.texData, 1.0f, mesh.texNum++);

					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum++);
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum++);
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum++);
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum++);

					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum++);
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum++);
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum++);
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum++);

					mesh.indData = intAppend(mesh.indData, l, mesh.indNum++);
				}
			}
		}
	}

	freeStrArr(words, OBJ_MAX);
	
	free(vertis);
	free(texis);

	free(verts);
	free(texes);

	return mesh;
}

#endif
