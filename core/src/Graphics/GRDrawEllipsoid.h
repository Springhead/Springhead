#include <GL/glew.h>
#include "GRFrame.h"


using namespace std;

namespace Spr{
	;

	class DrawEllipsoid
	{
	public:
		float pi;


		DrawEllipsoid()
		{
			pi = 3.141592653f;
		}

		int getPoint2(float rx, float ry, float rz, float a, float b, Vec3f &p, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			p.x = (float)(rx*sin(a*pi / 180.0)*cos(b*pi / 180.0));
			p.y = (float)(ry*sin(a*pi / 180.0)*sin(b*pi / 180.0));
			p.z = (float)(rz*cos(a*pi / 180.0));
			p = tq * p;
			return 1;
		}


		Vec3f* getPointMatrix2(float rx, float ry, float rz, int slices, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			int i, j, w = 2 * slices, h = slices;
			float a = 0.0, b = 0.0;
			float hStep = (float)180.0 / (h - 1);
			float wStep = (float)360.0 / w;
			int length = w*h;
			Vec3f *matrix;
			matrix = (Vec3f *)malloc(length*sizeof(Vec3f));
			if (!matrix)return NULL;
			for (a = 0.0, i = 0; i<h; i++, a += hStep)
				for (b = 0.0, j = 0; j<w; j++, b += wStep)
					getPoint2(rx, ry, rz, a, b, matrix[i*w + j], tq);
			return matrix;
		}

		int getPoint2(float rx, float ry, float rz, float a, float b, Vec3f &p, Matrix3f dotcEM)
		{
			p.x = (float)(rx*sin(a*pi / 180.0)*cos(b*pi / 180.0));
			p.y = (float)(ry*sin(a*pi / 180.0)*sin(b*pi / 180.0));
			p.z = (float)(rz*cos(a*pi / 180.0));
			//p = dotcEM * p;
			//Vec3f tmp;
			//multi(tmp, dotcEM, p);
			/*MatrixExtension ma;
			p = ma.MatrixMultiVec3fRight(dotcEM, p);*/

			Vec3f result;
			result.x = dotcEM.xx * p.x + dotcEM.xy * p.y + dotcEM.xz * p.z;
			result.y = dotcEM.yx * p.x + dotcEM.yy * p.y + dotcEM.yz * p.z;
			result.z = dotcEM.zx * p.x + dotcEM.zy * p.y + dotcEM.zz * p.z;
			p = result;
			return 1;
		}
		Vec3f* getPointMatrix2(float rx, float ry, float rz, int slices, Matrix3f dotcEM)
		{
			int i, j, w = 2 * slices, h = slices;
			float a = 0.0, b = 0.0;
			float hStep = (float) 180.0 / (h - 1);
			float wStep = (float) 360.0 / w;
			int length = w*h;
			Vec3f *matrix;
			matrix = (Vec3f *)malloc(length*sizeof(Vec3f));
			if (!matrix)return NULL;
			for (a = 0.0, i = 0; i<h; i++, a += hStep)
				for (b = 0.0, j = 0; j<w; j++, b += wStep)
					getPoint2(rx, ry, rz, a, b, matrix[i*w + j], dotcEM);
			return matrix;
		}

		int drawOval(float rx, float ry, float rz, int slices, TQuaternion<float> &tq)//TPose<float> &tp)  
		{
			int i = 0, j = 0, w = 2 * slices, h = slices;
			Vec3f *mx;
			Vec3f fn;
			mx = getPointMatrix2(rx, ry, rz, slices, tq);
			if (!mx)return 0;
			glBegin(GL_QUADS);
			for (; i<h - 1; i++)
			{
				for (j = 0; j<w - 1; j++)
				{
					fn.x = (mx[i*w + j].x + mx[i*w + j + 1].x + mx[(i + 1)*w + j + 1].x + mx[(i + 1)*w + j].x) / (2 * rx * rx);
					fn.y = (mx[i*w + j].y + mx[i*w + j + 1].y + mx[(i + 1)*w + j + 1].y + mx[(i + 1)*w + j].y) / (2 * ry * ry);
					fn.z = (mx[i*w + j].z + mx[i*w + j + 1].z + mx[(i + 1)*w + j + 1].z + mx[(i + 1)*w + j].z) / (2 * rz * rz);
					glNormal3f(fn.x, fn.y, fn.z);
					drawSlice(mx[i*w + j], mx[i*w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j]);
				}
				fn.x = (mx[i*w + j].x + mx[i*w].x + mx[(i + 1)*w].x + mx[(i + 1)*w + j].x) / (2 * rx * rx);
				fn.y = (mx[i*w + j].y + mx[i*w].y + mx[(i + 1)*w].y + mx[(i + 1)*w + j].y) / (2 * ry * ry);
				fn.z = (mx[i*w + j].z + mx[i*w].z + mx[(i + 1)*w].z + mx[(i + 1)*w + j].z) / (2 * rz * rz);
				glNormal3f(fn.x, fn.y, fn.z);
				drawSlice(mx[i*w + j], mx[i*w], mx[(i + 1)*w], mx[(i + 1)*w + j]);
			}
			free(mx);
			glEnd();
			return 1;
		}

		int drawOval(float rx, float ry, float rz, int slices, Matrix3f dotcEM)
		{
			int i = 0, j = 0, w = 2 * slices, h = slices;
			Vec3f *mx;
			Vec3f fn;
			mx = getPointMatrix2(rx, ry, rz, slices, dotcEM);
			if (!mx)return 0;
			glBegin(GL_QUADS);
			for (; i<h - 1; i++)
			{
				for (j = 0; j<w - 1; j++)
				{
					//calculate face normal
					glBegin(GL_QUADS);
					fn.x = (mx[i*w + j].x + mx[i*w + j + 1].x + mx[(i + 1)*w + j + 1].x + mx[(i + 1)*w + j].x) / (2 * rx * rx);
					fn.y = (mx[i*w + j].y + mx[i*w + j + 1].y + mx[(i + 1)*w + j + 1].y + mx[(i + 1)*w + j].y) / (2 * ry * ry);
					fn.z = (mx[i*w + j].z + mx[i*w + j + 1].z + mx[(i + 1)*w + j + 1].z + mx[(i + 1)*w + j].z) / (2 * rz * rz);
					glNormal3f(fn.x, fn.y, fn.z);
					drawSlice(mx[i*w + j], mx[i*w + j + 1], mx[(i + 1)*w + j + 1], mx[(i + 1)*w + j]);
					glEnd();
				}
				glBegin(GL_QUADS);
				fn.x = (mx[i*w + j].x + mx[i*w].x + mx[(i + 1)*w].x + mx[(i + 1)*w + j].x) / (2 * rx * rx);
				fn.y = (mx[i*w + j].y + mx[i*w].y + mx[(i + 1)*w].y + mx[(i + 1)*w + j].y) / (2 * ry * ry);
				fn.z = (mx[i*w + j].z + mx[i*w].z + mx[(i + 1)*w].z + mx[(i + 1)*w + j].z) / (2 * rz * rz);
				glNormal3f(fn.x, fn.y, fn.z);
				drawSlice(mx[i*w + j], mx[i*w], mx[(i + 1)*w], mx[(i + 1)*w + j]);
				glEnd();
			}
			free(mx);

			return 1;
		}

		void drawSlice(Vec3f &p1, Vec3f &p2, Vec3f &p3, Vec3f &p4)
		{


			glBegin(GL_QUADS);

			glColor3f(1.0f, 1.0f, 0);
			glVertex3f(p4.x, p4.y, p4.z);
			glVertex3f(p3.x, p3.y, p3.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p1.x, p1.y, p1.z);



			glEnd();
		}




	};


}//namespace