#pragma once
//#include <Springhead.h>
#include <GL/glut.h>
//#include <stdio.h>
#//include <stdarg.h>

using namespace Spr;
using namespace std;

#define M_PI 3.1415926535897932384626433832795

static float width = 400;
static float height = 300;

/// 形状ID
enum ShapeID {
	SHAPE_BOX,
	SHAPE_CAPSULE,
	SHAPE_ROUNDCONE,
	SHAPE_SPHERE,
	SHAPE_ROCK,
	SHAPE_POLYSPHERE,
	SHAPE_DODECA
};

/**
brief     多面体の面(三角形)の法線を求める
param	   <in/out> normal　　  法線
param     <in/-->   base　　　 meshの頂点
param     <in/-->   face　　　 多面体の面
return     なし
*/
void genFaceNormal(Vec3f& normal, Vec3f* base, CDFaceIf* face) {
	Vec3f edge0, edge1;
	edge0 = base[face->GetIndices()[1]] - base[face->GetIndices()[0]];
	edge1 = base[face->GetIndices()[2]] - base[face->GetIndices()[0]];
	normal = edge0^edge1;
	normal.unitize();
}
static char text[256];

void BeginRend2D() {
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	//glLoadIdentity();
}
void EndRend2D() {//必ずセットで使う
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void RendText(float posx,float posy,char* format, ...) {
	
	va_list args;
	va_start(args, format);
	int count = vsprintf_s(text,256, format, args);//vsprintfじゃないとva_listに対応しない
	va_end(args);
	if (count <= 0) return;
	
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(posx, posy);
	for (int i = 0; i < count; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}

}

Vec3d ObjtoScreenPos(const Vec3d objPos) {
	double modelview[16];//モデルビュー行列取得
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

	double projection[16];//透視投影行列取得
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	int viewport[4];//ビューポート取得
	glGetIntegerv(GL_VIEWPORT, viewport);

	double winX, winY, winZ;//ウィンドウ座標格納用

	gluProject(objPos.X(), objPos.Y(), objPos.Z(), modelview, projection, viewport, &winX, &winY, &winZ); //座標変換の計算
	return Vec3d(winX/width, winY/height, winZ);
}

void SetGLMesh(CDShapeIf* shape,ShapeID id,Posed pose) {
	Affined affine;
	glPushMatrix();
	pose.ToAffine(affine);
	glMultMatrixd(affine);
	switch (id)
	{
	case SHAPE_BOX: {
		CDBoxIf* box = DCAST(CDBoxIf, shape);
		if (box) {
			Vec3f size = box->GetBoxSize();
			glMultMatrixf(Affinef::Scale(size.X(), size.Y(), size.Z()));
			glutSolidCube(1.0);
		}
		break;
	}
	case SHAPE_CAPSULE: {
		CDCapsuleIf* cap = DCAST(CDCapsuleIf, shape);
		if (cap) {
			float radius = cap->GetRadius();
			float height = cap->GetLength();
			//glMultMatrixd(Affined::Rot(M_PI*0.5,Vec3f(1.f,0,0)));
			glMultMatrixd(Affined::Trn(0.0f, 0.0f, -height / 2.0f));
			glutSolidCylinder(radius, height, 8, 1);
			glutSolidSphere(radius, 8, 8);
			glMultMatrixd(Affined::Trn(0.0f, 0.0f, height));
			glutSolidSphere(radius, 8, 8);
		}
		break;
	}
	case SHAPE_ROUNDCONE: {
		CDRoundConeIf* cone = DCAST(CDRoundConeIf, shape);
		if (cone) {
			Vec2f radius = cone->GetRadius();
			float height = cone->GetLength();
			//glMultMatrixd(Affined::Rot(M_PI*0.5, Vec3f(1.f, 0, 0)));
			glMultMatrixd(Affined::Trn(0.0f, 0.0f, -height / 2.0f));
			GLUquadricObj* cylinder;
			cylinder = gluNewQuadric();
			gluQuadricDrawStyle(cylinder, GLU_FILL);
			gluCylinder(cylinder, radius.X(), radius.Y(), height, 8, 1);
			glutSolidSphere(radius.X(), 8, 8);
			glMultMatrixd(Affined::Trn(0.0f, 0.0f, height));
			glutSolidSphere(radius.Y(), 8, 8);

		}
		break;
	}
	case SHAPE_SPHERE: {
		CDSphereIf* sphere = DCAST(CDSphereIf, shape);
		if (sphere) {
			glutSolidSphere(sphere->GetRadius(), 8, 8);
		}
		break;
	}
	case SHAPE_ROCK :
	case SHAPE_POLYSPHERE:
	case SHAPE_DODECA:{
		Vec3f normal;
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		if (mesh) {
			Vec3f* base = mesh->GetVertices();
			for (int f = 0; f < mesh->NFace(); ++f) {
				CDFaceIf* face = mesh->GetFace(f);

				glBegin(GL_POLYGON);
				genFaceNormal(normal, base, face);
				glNormal3fv(normal.data);
				for (int v = 0; v < face->NIndex(); ++v) {
					glVertex3fv(base[face->GetIndices()[v]].data);
				}
				glEnd();
			}
		}
		break;
	}
	default:
		break;
		
	}
	glPopMatrix();
}

vector<string> split(string& input, char delimiter)
{
	istringstream stream(input);
	string field;
	vector<string> result;
	while (getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}


vector<int> LoadTestCSV(string path)
{
	vector<int> shapes;
	ifstream ifs(path);
	string line;
	while (getline(ifs,line))
	{
		vector<string> word = split(line, ',');
		for (int i = 0; i < word.size();i++)
		{
			shapes.push_back(stoi(word[i]));
		}
	}
	return shapes;
}

class TestObj {
public:
	CDShapeIf* m_shape;
	ShapeID m_shapeID;
	Vec3f m_position;
	Quaternionf m_rotation;
private:
	PHSolidIf* m_solid;
public:
	void Init(PHSolidIf* solid, CDShapeIf* shape,ShapeID id,Vec3f pos=Vec3f::Zero(),Quaternionf rot = Quaternionf()) {
		m_solid = solid;
		m_shape = shape;
		m_shapeID = id;
		m_position = pos;
		m_rotation = rot;
		m_solid->ClearShape();
		m_solid->AddShape(m_shape);
	}

	void SetShape(CDShapeIf* shape, ShapeID id) {
		m_shape = shape;
		m_shapeID = id;
		m_solid->ClearShape();
		m_solid->AddShape(m_shape);
	}

	void SetPos(Vec3f pos) {
		m_position = pos;
		m_solid->SetFramePosition(m_position);
	}
	void Translate(Vec3f delta) {
		SetPos(m_position + delta);
	}

	void SetRot(Quaternionf rot) {
		m_rotation = rot;
		m_solid->SetOrientation(m_rotation);
	}
	void Rotate(Quaternionf delta) {
		SetRot(m_rotation*delta);
	}

	PHSolidIf* GetSolid() { return m_solid; }
};

class TestStage {
public:
	/// 作りおき形状
	CDBoxIf*				shapeBox;
	CDSphereIf*				shapeSphere;
	CDCapsuleIf*			shapeCapsule;
	CDRoundConeIf*			shapeRoundCone;
	CDConvexIf*				shapeRock;
	CDConvexIf*				shapePolySphere;
	CDConvexIf*				shapeDodecahedron;


	void Init(PHSdkIf *sdk) {
		// 形状の作成
		CDBoxDesc bd;
		bd.boxsize = Vec3f(3, 3, 3);
		shapeBox = sdk->CreateShape(bd)->Cast();

		CDSphereDesc sd;
		sd.radius = 2;
		shapeSphere = sdk->CreateShape(sd)->Cast();

		CDCapsuleDesc cd;
		cd.radius = 1;
		cd.length = 2;
		shapeCapsule = sdk->CreateShape(cd)->Cast();

		CDRoundConeDesc rcd;
		rcd.length = 3;
		//rcd.radius = Vec2f(1.f, 2.f);
		shapeRoundCone = sdk->CreateShape(rcd)->Cast();

		CDConvexMeshDesc md;
		int nv = 250;
		for (int i = 0; i < nv; ++i) {
			Vec3d v;
			for (int c = 0; c<3; ++c) {
				v[c] = ((rand() % 100) / 100.0 - 0.5) * 3;
			}
			md.vertices.push_back(v);
		}
		shapeRock = sdk->CreateShape(md)->Cast();

		
		CDConvexMeshDesc pd;
		int xnum = 15;
		xnum -= 1;
		int ynum = 15;
		double scale = 2;
		pd.vertices.push_back(Vec3d(0, scale*0.5, 0));
		pd.vertices.push_back(Vec3d(0, -scale*0.5, 0));
		for (int i = 0; i < ynum; ++i) {
			double yangle = (2*M_PI / ynum)*i;
			for (int j = 1; j<xnum; ++j) {
				double xangle = (M_PI / xnum)*j - (M_PI/2);
				Vec3d v = Vec3d(scale,0,0);
				Quaterniond rot = Quaterniond();
				rot.Euler(yangle,xangle, 0);
				v = rot*v;
				//rot = Quaterniond::Rot(xangle, Vec3d(0, 1, 0) ^ v);
				//v = rot*v;
				v.y *= 0.5;
				pd.vertices.push_back(v);
			}
			
		}
		shapePolySphere = sdk->CreateShape(pd)->Cast();
		
		pd.vertices.clear();
		pd.vertices.push_back(Vec3d(1, 1, 1));
		pd.vertices.push_back(Vec3d(-1, 1, 1));
		pd.vertices.push_back(Vec3d(1, -1, 1));
		pd.vertices.push_back(Vec3d(1, 1, -1));
		pd.vertices.push_back(Vec3d(-1, -1, 1));
		pd.vertices.push_back(Vec3d(1, -1, -1));
		pd.vertices.push_back(Vec3d(-1, 1, -1));
		pd.vertices.push_back(Vec3d(-1, -1, -1));
		//
		double gsi = (1 + sqrt(5)) / 2;
		double gsiInv = 1 / gsi;
		pd.vertices.push_back(Vec3d(0, gsiInv, gsi));
		pd.vertices.push_back(Vec3d(0, -gsiInv, gsi));
		pd.vertices.push_back(Vec3d(0, gsiInv, -gsi));
		pd.vertices.push_back(Vec3d(0, -gsiInv, -gsi));
		pd.vertices.push_back(Vec3d(gsi,0, gsiInv));
		pd.vertices.push_back(Vec3d(-gsi, 0, gsiInv));
		pd.vertices.push_back(Vec3d(gsi, 0, -gsiInv));
		pd.vertices.push_back(Vec3d(-gsi, 0, -gsiInv));
		pd.vertices.push_back(Vec3d(gsiInv, gsi, 0));
		pd.vertices.push_back(Vec3d(-gsiInv, gsi, 0));
		pd.vertices.push_back(Vec3d(gsiInv, -gsi, 0));
		pd.vertices.push_back(Vec3d(-gsiInv, -gsi, 0));

		shapeDodecahedron = sdk->CreateShape(pd)->Cast();



	}

	CDShapeIf* GetShape(ShapeID id) {
		switch (id)
		{
		case SHAPE_BOX:
			return shapeBox;
			break;
		case SHAPE_CAPSULE:
			return shapeCapsule;
			break;
		case SHAPE_ROUNDCONE:
			return shapeRoundCone;
			break;
		case SHAPE_SPHERE:
			return shapeSphere;
			break;
		case SHAPE_ROCK:
			return shapeRock;
			break;
		case SHAPE_POLYSPHERE:
			return shapePolySphere;
			break;
		case SHAPE_DODECA:
			return shapeDodecahedron;
			break;
		default:
			return shapeSphere;
			break;
		}
	}
};
