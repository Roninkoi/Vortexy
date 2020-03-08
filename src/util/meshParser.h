//
// Created by rak on 2/12/20.
//

#ifndef VORTEXY_MESHPARSER_H
#define VORTEXY_MESHPARSER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "phys/mesh.h"
#include "util.h"

#define MESH_MAX 0x1000000

// creates a mesh object and loads .mesh
Mesh meshParser(char *path)
{
	Mesh mesh;

	p_meshInit(&mesh);

	int wordNum = 0;
	char **words = wordsFromFile(path, MESH_MAX, &wordNum);

	int vi = 0;
	int ii = 0;

	int mode = 0;

	for (int i = 0; i < wordNum; ++i) {
		if (strcmp(words[i], "Vertices") == 0) {
			mode = 1;
			mesh.vertNum = atoi(words[++i]) * 4;
			mesh.texNum = mesh.vertNum;
			mesh.colNum = mesh.vertNum;
			mesh.normNum = mesh.vertNum;

			mesh.vertData = (float *) malloc(sizeof(float) * mesh.vertNum * 4);
			mesh.texData = (float *) malloc(sizeof(float) * mesh.vertNum * 4);
			mesh.colData = (float *) malloc(sizeof(float) * mesh.vertNum * 4);
			mesh.normData = (float *) malloc(sizeof(float) * mesh.vertNum * 4);

		}
		if (strcmp(words[i], "Triangles") == 0) {
			mode = 2;
			mesh.indNum = atoi(words[++i]) * 3;
			mesh.indData = (int *) malloc(sizeof(int) * mesh.indNum * 3);
		}
		if (strcmp(words[i], "Edges") == 0) {
			mode = 3;
		}
		if (strcmp(words[i], "Quadrilaterals") == 0) {
			mode = 4;
		}
		if (strcmp(words[i], "Tetrahedra") == 0) {
			mode = 5;
		}

		if (mode == 1 && vi + 3 < mesh.vertNum) { // verts
			float v0 = atof(words[++i]);
			float v1 = atof(words[++i]);
			float v2 = atof(words[++i]);

			mesh.vertData[vi+0] = v0;
			mesh.vertData[vi+1] = v1;
			mesh.vertData[vi+2] = v2;
			mesh.vertData[vi+3] = 0.0f;

			mesh.texData[vi+0] = 0.0f;
			mesh.texData[vi+1] = 0.0f;
			mesh.texData[vi+2] = 1.0f;
			mesh.texData[vi+3] = 1.0f;

			mesh.colData[vi+0] = 1.0f;
			mesh.colData[vi+1] = 1.0f;
			mesh.colData[vi+2] = 1.0f;
			mesh.colData[vi+3] = 1.0f;

			mesh.normData[vi+0] = 0.0f;
			mesh.normData[vi+1] = 0.0f;
			mesh.normData[vi+2] = 0.0f;
			mesh.normData[vi+3] = 0.0f;

			vi += 4;
		}

		if (mode == 2 && ii + 2 < mesh.indNum) {
			int i0 = atoi(words[++i]);
			int i1 = atoi(words[++i]);
			int i2 = atoi(words[++i]);

			mesh.indData[ii++] = i0-1;
			mesh.indData[ii++] = i1-1;
			mesh.indData[ii++] = i2-1;
		}
	}

	/*for (int i = 0; i < mesh.indNum; i+=4) {
		printf("%i %i %i\n", mesh.indData[i], mesh.indData[i+1], mesh.indData[i+2]);
	}*/

	freeStrArr(words, MESH_MAX);

	return mesh;
}

#endif //VORTEXY_MESHPARSER_H
