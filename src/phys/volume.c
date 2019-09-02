#include "volume.h"
#include "util/util.h"

// triangle cross product area
void computeArea(struct Face *f)
{
	float vx = f->verts[4] - f->verts[0];
	float vy = f->verts[5] - f->verts[1];
	float vz = f->verts[6] - f->verts[2];

	float ux = f->verts[8] - f->verts[0];
	float uy = f->verts[9] - f->verts[1];
	float uz = f->verts[10] - f->verts[2];

	float x = vy * uz - vz * uy;
	float y = -vx * uz + vz * ux;
	float z = vx * uy - vy * ux;
	
	float cross = x * x + y * y + z * z;
	cross = sqrtf(cross);

	f->area = cross * 0.5f;
}

// centroid
void computeFaceCent(struct Face *f)
{
	float x = f->verts[0] + f->verts[4] + f->verts[8];
	float y = f->verts[1] + f->verts[5] + f->verts[9];
	float z = f->verts[2] + f->verts[6] + f->verts[10];

	x /= 3.0f;
	y /= 3.0f;
	z /= 3.0f;

	f->centroid = Vec3(x, y, z);
}

// face unit normal
void computeNormal(struct Face *f)
{
	vec3 v = Vec3(f->verts[4] - f->verts[0],
					f->verts[5] - f->verts[1],
					f->verts[6] - f->verts[2]);

	vec3 u = Vec3(f->verts[8] - f->verts[0],
					f->verts[9] - f->verts[1],
					f->verts[10] - f->verts[2]);

	f->normal = vec3Cross(&v, &u);

	vec3Normalize(&f->normal);
}

// normal and area from normal
void computeNormalArea(struct Face *f)
{
	vec3 v = Vec3(f->verts[4] - f->verts[0],
					f->verts[5] - f->verts[1],
					f->verts[6] - f->verts[2]);

	vec3 u = Vec3(f->verts[8] - f->verts[0],
					f->verts[9] - f->verts[1],
					f->verts[10] - f->verts[2]);

	f->normal = vec3Cross(&v, &u);

	f->area = vec3Len(&f->normal) * 0.5f;

	vec3Normalize(&f->normal);
}

void connects(struct Face *f, int **fl, int *fln, int i, int j)
{	
	int onei = -1;
	int twoi = -1;
	
	int onej = -1;
	int twoj = -1;
	
	for (int a = 0; a < 3; ++a) {
		for (int b = 0; b < 3; ++b) {
			if (f[i].inds[a] == f[j].inds[b]) {
				if (onei < 0) {
					onei = i;
				}
				else {
					twoi = i;
				}
				if (onej < 0) {
					onej = j;
				}
				else {
					twoj = j;
				}
			}
		}
	}
	
	if (onej < 0 || twoj < 0)
		return;
	
	if (onei < 0 || twoi < 0)
		return;
	
	if (!contains(fl[i], onej, fln[i])) {
		fl[i] = intAppend(fl[i], onej, fln[i]);
		++fln[i];
	}
	
	if (!contains(fl[i], twoj, fln[i])) {
		fl[i] = intAppend(fl[i], twoj, fln[i]);
		++fln[i];
	}
	
	if (!contains(fl[j], onei, fln[j])) {
		fl[j] = intAppend(fl[j], onei, fln[j]);
		++fln[j];
	}
	
	if (!contains(fl[j], twoi, fln[j])) {
		fl[j] = intAppend(fl[j], twoi, fln[j]);
		++fln[j];
	}
}

void computeConnections(struct Face *f, const int fn)
{
	int *fl[fn];
	int fln[fn];

	for (int i = 0; i < fn; ++i) {
		fl[i] = NULL;
		fln[i] = 0;
	}
	
	for (int i = 0; i < fn; ++i) {
		for (int j = i + 1; j < fn; ++j) {
			connects(f, fl, fln, i, j);
		}
	}

	for (int i = 0; i < fn; ++i) {
		printf("%s ", "CON");
		for (int j = 0; j < fln[i]; ++j) {
			printf("%i ", fl[i][j]);
		}
		printf("\n");
	}
	
	for (int i = 0; i < fn; ++i) {
		f[i].connecting = malloc(sizeof(struct Face *) * fln[i]);
		f[i].conNum = fln[i];
		
		for (int j = 0; j < fln[i]; ++j) {
			f[i].connecting[j] = &f[fl[i][j]];
		}
		
		free(fl[i]);
	}
}

struct Face *p_loadFaces(Mesh *m, int *faceNum)
{
	struct Face *f;
	
	const int fn = m->indNum / 3;
	*faceNum = fn;

	printf("%s %i\n", "FACES", fn);

	f = (struct Face *) malloc(sizeof(struct Face) * fn);

	for (int i = 0; i < fn; ++i) {
		f[i].index = i;
		f[i].vNum = 0;
		
		f[i].inds[0] = m->indData[i * 3 + 0];
		f[i].inds[1] = m->indData[i * 3 + 1];
		f[i].inds[2] = m->indData[i * 3 + 2];

		for (int j = 0; j < 3; ++j) {
			f[i].verts[j * 4 + 0] = m->vertData[f[i].inds[j] * 4 + 0];
			f[i].verts[j * 4 + 1] = m->vertData[f[i].inds[j] * 4 + 1];
			f[i].verts[j * 4 + 2] = m->vertData[f[i].inds[j] * 4 + 2];
			f[i].verts[j * 4 + 3] = m->vertData[f[i].inds[j] * 4 + 3];
		}

		computeNormalArea(&f[i]);

		computeFaceCent(&f[i]);
	}

	computeConnections(f, fn);

	return f;
}

void computeVolume(struct Volume *v)
{
	// a . (b x c) / 6
	vec3 v0 = Vec3(v->faces[0]->verts[0],
					 v->faces[0]->verts[1],
					 v->faces[0]->verts[2]);
	vec3 v1 = Vec3(v->faces[0]->verts[4],
					 v->faces[0]->verts[5],
					 v->faces[0]->verts[6]);
	vec3 v2 = Vec3(v->faces[0]->verts[8],
					 v->faces[0]->verts[9],
					 v->faces[0]->verts[10]);

	vec3 u0 = Vec3(v->faces[1]->verts[0],
					 v->faces[1]->verts[1],
					 v->faces[1]->verts[2]);
	vec3 u1 = Vec3(v->faces[1]->verts[4],
	 				 v->faces[1]->verts[5],
					 v->faces[1]->verts[6]);
	vec3 u2 = Vec3(v->faces[1]->verts[8],
					 v->faces[1]->verts[9],
					 v->faces[1]->verts[10]);

	vec3 w;

	if (v->faces[1]->inds[0] != v->faces[0]->inds[0] &&
		v->faces[1]->inds[0] != v->faces[0]->inds[1] &&
		v->faces[1]->inds[0] != v->faces[0]->inds[2])
		w = u0;
	else if (v->faces[1]->inds[1] != v->faces[0]->inds[0] &&
			 v->faces[1]->inds[1] != v->faces[0]->inds[1] &&
			 v->faces[1]->inds[1] != v->faces[0]->inds[2])
		w = u1;
	else
		w = u2;
	
	vec3Sub(&v1, &v0);
	vec3Sub(&v2, &v0);
	vec3Sub(&w, &v0);

	vec3 cross = vec3Cross(&v2, &w);

	v->vol = fabs((1.0f / 6.0f) * vec3Dot(&v1, &cross));

	printf("V %f\n", v->vol);
}

void computeVolumeCent(struct Volume *v)
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	
	for (int i = 0; i < 4; ++i) {
		x += v->faces[i]->centroid.x;
		y += v->faces[i]->centroid.y;
		z += v->faces[i]->centroid.z;
	}

	x /= 4.0f;
	y /= 4.0f;
	z /= 4.0f;

	v->centroid = Vec3(x, y, z);
}

int containsFace(struct Face **f, struct Face *c, int n)
{
	for (int i = 0; i < n; ++i) {
		if (f[i] == c) {
			return 1;
		}
	}
	return 0;
}

void computeTopology(struct Volume *v, const int vn)
{
	for (int i = 0; i < vn; ++i)
		v[i].neiNum = 0;
	
	for (int i = 0; i < vn; ++i) {
		for (int j = i + 1; j < vn; ++j) {
			int cf = 0;
			
			if (containsFace(v[i].faces, v[j].faces[0], 4)) {
				cf = 1;
			}
			if (containsFace(v[i].faces, v[j].faces[1], 4)) {
				cf = 1;
			}
			if (containsFace(v[i].faces, v[j].faces[2], 4)) {
				cf = 1;
			}
			if (containsFace(v[i].faces, v[j].faces[3], 4)) {
				cf = 1;
			}

			for (int k = 0; k < v[i].neiNum; ++k) {
				if (&v[j] == v[i].neighbours[k]) {
					cf = 0;
				}
			}
			
			if (cf) {
				if (v[i].neiNum < 4) {
					v[i].neighbours[v[i].neiNum] = &v[j];
					++v[i].neiNum;
				}
				
				if (v[j].neiNum < 4) {
					v[j].neighbours[v[j].neiNum] = &v[i];
					++v[j].neiNum;
				}
			}
		}
	}
}

int sharesEdge(struct Face *f0, struct Face *f1, struct Face *f2, struct Face *f3)
{
	int a[3] = {-1, -1, -1};
	int b[3] = {-1, -1, -1};

	int fi = 0;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (f0->inds[i] == f1->inds[j]) {
				if (a[fi] == -1) {
					a[fi] = f0->inds[i];
				}
				else {
					b[fi] = f0->inds[i];
				}
			}
		}
	}
	
	fi = 1;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (f0->inds[i] == f2->inds[j]) {
				if (a[fi] == -1) {
					a[fi] = f0->inds[i];
				}
				else {
					b[fi] = f0->inds[i];
				}
			}
		}
	}

	fi = 2;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (f0->inds[i] == f3->inds[j]) {
				if (a[fi] == -1) {
					a[fi] = f0->inds[i];
				}
				else {
					b[fi] = f0->inds[i];
				}
			}
		}
	}

	int s = 0;

	if ((a[0] == a[1] && b[0] == b[1]) ||
		(a[0] == b[1] && b[0] == a[1]))
		++s;
	if ((a[0] == a[2] && b[0] == b[2]) ||
		(a[0] == b[2] && b[0] == a[2]))
		++s;
	if ((a[2] == a[1] && b[2] == b[1]) ||
		(a[2] == b[1] && b[2] == a[1]))
		++s;

	if (s > 2)
		return 1;
	
	return 0;
}

struct Volume *p_loadVolumes(struct Face *f, int faceNum, int *volNum)
{
	struct Volume *v = NULL;
	
	int **vi = NULL;
	int n = 0;

	printf("Starting volume (%i) computation...\n", faceNum);
	
	for (int i = 0; i < faceNum; ++i) {
		for (int j = i + 1; j < faceNum; ++j) {
			if (!containsFace(f[i].connecting, &f[j], f[i].conNum))
				continue;
			for (int k = j + 1; k < faceNum; ++k) {
				if (!containsFace(f[i].connecting, &f[k], f[i].conNum))
					continue;
				if (!containsFace(f[j].connecting, &f[k], f[j].conNum))
					continue;
				for (int l = k + 1; l < faceNum; ++l) {
					if (!containsFace(f[i].connecting, &f[l], f[i].conNum))
						continue;
					
					if (!containsFace(f[j].connecting, &f[l], f[j].conNum))
						continue;
					
					if (!containsFace(f[k].connecting, &f[l], f[k].conNum))
						continue;
					
					if (sharesEdge(&f[i], &f[j], &f[k], &f[l]))
						continue;
					
					int *t = malloc(sizeof(int) * 4);
					t[0] = i;
					t[1] = j;
					t[2] = k;
					t[3] = l;

					vi = intsAppend(vi, t, n);
					++n;
				}
			}
		}
		printf("%i / %i\n", i, faceNum);
	}

	*volNum = n;

	v = malloc(sizeof(struct Volume) * n);

	for (int i = 0; i < n; ++i) {
		v[i].index = i;
		
		v[i].faces[0] = &f[vi[i][0]];
		v[i].faces[1] = &f[vi[i][1]];
		v[i].faces[2] = &f[vi[i][2]];
		v[i].faces[3] = &f[vi[i][3]];

		v[i].faces[0]->thisVol[v[i].faces[0]->vNum++];
		v[i].faces[1]->thisVol[v[i].faces[1]->vNum++];
		v[i].faces[2]->thisVol[v[i].faces[2]->vNum++];
		v[i].faces[3]->thisVol[v[i].faces[3]->vNum++];

		computeVolumeCent(&v[i]);
		computeVolume(&v[i]);
		
		free(vi[i]);
	}

	free(vi);
	
	for (int i = 0; i < n; ++i) {
		printf("VOL %i %i %i %i\n", v[i].faces[0]->index,
			   v[i].faces[1]->index, v[i].faces[2]->index, v[i].faces[3]->index);
	}

	computeTopology(v, n);

	for (int i = 0; i < n; ++i) {
		printf("%s ", "NEI");
		for (int j = 0; j < v[i].neiNum; ++j)
			printf("%i ", v[i].neighbours[j]->index);
		printf("\n");
	}
	
	return v;
}
