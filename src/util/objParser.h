#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdlib.h>
#include <stdio.h>

#include "phys/mesh.h"
#include "util.h"

#define OBJ_MAX 0x100000

// creates a mesh object and loads obj
Mesh objParser(char *path)
{
	Mesh mesh;

	p_meshInit(&mesh);

	FILE *fp;
	char *data;
	size_t fsize;

	fp = fopen(path, "r");

	if (!fp) {
		exit(1);
	}

	data = (char *) malloc(OBJ_MAX);
	fsize = fread(data, 1, OBJ_MAX, fp);

	fclose(fp);

	char **words = calloc(OBJ_MAX, sizeof(char *));

	for (int i = 0; i < OBJ_MAX; ++i) {
		words[i] = calloc(1, sizeof(char));
	}

	int wordNum = 0;

	for (int i = 0; data[i]; ++i) { // separate into words
		char read = data[i];

		if (read != ' ' && read != '\n' && read != '\r') {
			words[wordNum] = strAppend(words[wordNum], read);
		}
		else {
			++wordNum;
		}
	}

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

			verts = floatAppend(verts, v0, vn);
			++vn;

			verts = floatAppend(verts, v1, vn);
			++vn;

			verts = floatAppend(verts, v2, vn);
			++vn;
		}

		if (strcmp(words[i], "vt") == 0) { // texes
			float v0 = atof(words[i + 1]);
			float v1 = atof(words[i + 2]);
			float v2 = atof(words[i + 3]);

			texes = floatAppend(texes, v0, tn);
			++tn;

			texes = floatAppend(texes, v1, tn);
			++tn;

			texes = floatAppend(texes, v2, tn);
			++tn;
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
				int ti = atoi(ts) - 1;

				int exists = 0;
				int t = 0;

				for (; !exists && t < vin; ++t) { // optimize meshes
					if (vi == vertis[t] && ti == texis[t]) {
						exists = 1;
						t -= 1;
					}
				}

				if (exists) {
					mesh.indData = intAppend(mesh.indData, t, mesh.indNum);
					++mesh.indNum;
				}
				else {
					vertis = intAppend(vertis, vi, vin);
					++vin;
					texis = intAppend(texis, ti, tin);
					++tin;

					int l = vin - 1;

					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3], mesh.vertNum);
					++mesh.vertNum;
					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3 + 1], mesh.vertNum);
					++mesh.vertNum;
					mesh.vertData = floatAppend(mesh.vertData, verts[vertis[l] * 3 + 2], mesh.vertNum);
					++mesh.vertNum;
					mesh.vertData = floatAppend(mesh.vertData, 0.0f, mesh.vertNum);
					++mesh.vertNum;

					mesh.texData = floatAppend(mesh.texData, texes[texis[l] * 2], mesh.texNum);
					++mesh.texNum;
					mesh.texData = floatAppend(mesh.texData, -texes[texis[l] * 2 + 1], mesh.texNum);
					++mesh.texNum;
					mesh.texData = floatAppend(mesh.texData, 1.0f, mesh.texNum);
					++mesh.texNum;
					mesh.texData = floatAppend(mesh.texData, 1.0f, mesh.texNum);
					++mesh.texNum;

					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum);
					++mesh.normNum;
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum);
					++mesh.normNum;
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum);
					++mesh.normNum;
					mesh.normData = floatAppend(mesh.normData, 0.0f, mesh.normNum);
					++mesh.normNum;

					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum);
					++mesh.colNum;
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum);
					++mesh.colNum;
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum);
					++mesh.colNum;
					mesh.colData = floatAppend(mesh.colData, 1.0f, mesh.colNum);
					++mesh.colNum;

					mesh.indData = intAppend(mesh.indData, l, mesh.indNum);
					++mesh.indNum;
				}
			}
		}
	}

	for (int i = 0; i < wordNum; ++i) {
		free(words[i]);
	}

	free(vertis);
	free(texis);

	free(verts);
	free(texes);

	free(words);
	free(data);

	return mesh;
}

#endif
