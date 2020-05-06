/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 Springhead2/src/tests/Graphics/GRVertexBlending/main.cpp

【概要】
  ・VertexBlending のテストプログラム

【内容】
  ・各頂点座標に対し、変換行列を与え、ウェイトで合成する。
  ・それぞれのオブジェクトは３つのQuadから構成され、
	Blue Quad には右に１ずらした変換行列を与え、Yellow Quad には左に１ずらした変換行列を与える。
  ・また、左から順に、GRVertexElement::VFP3fB4f、GRVertexElement::VFC3fP3fB4f、GRVertexElement::VFC4bP3fB4f のテストであり、
  　レンダリング結果は、
  　	GRVertexElement::VFP3fB4f		　	：白からなる３つのブロック
		GRVertexElement::VFC3fP3fB4f		：青緑黄からなる３つのブロック
		GRVertexElement::VFC4bP3fB4f		：青緑黄からなる３つのブロック
	となる。

 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <GL/glew.h>
#include <Springhead.h>
#include <GL/glut.h>
using namespace Spr;

#define EXIT_TIMER	4000
const int MAX_BLEND_MATRIX = 2;												

UTRef<GRSdkIf> grSdk;
GRRenderIf* render;
GRDeviceGLIf* grDevice;

std::vector<GRVertexElement::VFP3fB4f> vtxP3fB4f;							// data
std::vector<GRVertexElement::VFC3fP3fB4f> vtxC3fP3fB4f;
std::vector<GRVertexElement::VFC4bP3fB4f> vtxC4bP3fB4f;

unsigned int listP3fB4f, listC3fP3fB4f, listC4bP3fB4f;						// display list
GRHandler shaderP3fB4f, shaderC3fP3fB4f, shaderC4bP3fB4f;					// shader program
SFBlendLocation locP3fB4f, locC3fP3fB4f, locC4bP3fB4f;		// shader location

std::vector<size_t> faces;													// data indices

struct VertexC3f
{
	float r, g, b;
	float x, y, z;
	float blends[MAX_BLEND_MATRIX];
	float matrixIndices[MAX_BLEND_MATRIX];
	float numMatrix;				
};
VertexC3f verticesC3f[] =
{
	// r    g    b      x    y    z      w0   w1    mi0   mi1    nb
	{ 0.0f,0.0f,1.0f,  1.0f,6.0f,0.0f,  1.0f,0.0f,  0.0f,0.0f,  1.0f },
	{ 0.0f,0.0f,1.0f, -1.0f,6.0f,0.0f,  1.0f,0.0f,  0.0f,0.0f,  1.0f },
	{ 0.0f,0.0f,1.0f, -1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f }, // Blue Quad （上）
	{ 0.0f,0.0f,1.0f,  1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },

	{ 0.0f,1.0f,0.0f,  1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 0.0f,1.0f,0.0f, -1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 0.0f,1.0f,0.0f, -1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f }, // Green Quad（中）
	{ 0.0f,1.0f,0.0f,  1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },

	{ 1.0f,1.0f,0.0f,  1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 1.0f,1.0f,0.0f, -1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 1.0f,1.0f,0.0f, -1.0f,0.0f,0.0f,  1.0f,0.0f,  1.0f,0.0f,  1.0f }, // Yellow Quad（下）
	{ 1.0f,1.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,0.0f,  1.0f,0.0f,  1.0f }
};

struct VertexC4f
{
	float r, g, b, a;
	float x, y, z;
	float blends[MAX_BLEND_MATRIX];
	float matrixIndices[MAX_BLEND_MATRIX];
	float numMatrix;				
};
VertexC4f verticesC4f[] =
{
	// r    g    b    a        x    y    z     w0   w1    mi0   mi1    nb
	{ 0.0f,0.0f,1.0f,1.0f,   1.0f,6.0f,0.0f,  1.0f,0.0f,  0.0f,0.0f,  1.0f },
	{ 0.0f,0.0f,1.0f,1.0f,  -1.0f,6.0f,0.0f,  1.0f,0.0f,  0.0f,0.0f,  1.0f },
	{ 0.0f,0.0f,1.0f,1.0f,  -1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f }, // Blue Quad （上）
	{ 0.0f,0.0f,1.0f,1.0f,   1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },

	{ 0.0f,1.0f,0.0f,1.0f,   1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 0.0f,1.0f,0.0f,1.0f,  -1.0f,4.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 0.0f,1.0f,0.0f,1.0f,  -1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f }, // Green Quad（中）
	{ 0.0f,1.0f,0.0f,1.0f,   1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },

	{ 1.0f,1.0f,0.0f,1.0f,   1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 1.0f,1.0f,0.0f,1.0f,  -1.0f,2.0f,0.0f,  0.5f,0.5f,  0.0f,1.0f,  2.0f },
	{ 1.0f,1.0f,0.0f,1.0f,  -1.0f,0.0f,0.0f,  1.0f,0.0f,  1.0f,0.0f,  1.0f }, // Yellow Quad（下）
	{ 1.0f,1.0f,0.0f,1.0f,   1.0f,0.0f,0.0f,  1.0f,0.0f,  1.0f,0.0f,  1.0f }
};


//------------------------------------------------------------------------------------------------------


/**
 brief  	glutReshapeFuncで指定したコールバック関数
 param	 	<in/--> w　　幅
 param  	<in/--> h　　高さ
 return 	なし
 */
void SPR_CDECL reshape(int w, int h){
	render->Reshape(Vec2f(0,0), Vec2f(w,h));
}
/**
 brief     	glutDisplayFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL display(void) {
	render->ClearBuffer();
	glLoadIdentity();

	gluLookAt(0.0,3.0,5.0, 
		      0.0,3.0,-1.0,
			  0.0f,1.0f,0.0f);
	
	glPushMatrix();
	glTranslatef(-3.0, 0.0, 0.0);
	render->DrawList(listP3fB4f);
	glPopMatrix();

	render->DrawList(listC3fP3fB4f);

	glPushMatrix();
	glTranslatef(3.0, 0.0, 0.0);
	render->DrawList(listC4bP3fB4f);
	glPopMatrix();

	render->EndScene();
	glutSwapBuffers();
}
/**
 brief 		glutKeyboardFuncで指定したコールバック関数 
 param		<in/--> key　　 ASCIIコード
 param 		<in/--> x　　　 キーが押された時のマウス座標
 param 		<in/--> y　　　 キーが押された時のマウス座標
 return 	なし
 */
void SPR_CDECL keyboard(unsigned char key, int x, int y) {
	if (key == 27) 
		exit(0);
}
/**
 brief  	glutIdleFuncで指定したコールバック関数
 param	 	なし
 return 	なし
 */
void SPR_CDECL idle(){
	glutPostRedisplay();
	static int count=0;
	count++;
	if (count > EXIT_TIMER){
		DSTR << EXIT_TIMER << " count passed." << std::endl;
		exit(0);
	}
}
/**
 brief  	頂点データ設定関数
 param	 	なし
 return 	なし
 */
void SetVertex(){
	int nIndex=0;
	int nIndexOffset=0;	
	for( int j = 0; j < 3; ++j )
	{
		nIndexOffset = j * 4;
		for( int i = 0; i < 4; ++i )
		{
			nIndex = i + nIndexOffset;

			GRVertexElement::VFP3fB4f tmpP3fB4f;
			tmpP3fB4f.p.x = verticesC3f[nIndex].x;
			tmpP3fB4f.p.y = verticesC3f[nIndex].y;
			tmpP3fB4f.p.z = verticesC3f[nIndex].z;
			tmpP3fB4f.b.data[0]  = verticesC3f[nIndex].blends[0];
			tmpP3fB4f.b.data[1]  = verticesC3f[nIndex].blends[1];
			tmpP3fB4f.mi.data[0] = verticesC3f[nIndex].matrixIndices[0];
			tmpP3fB4f.mi.data[1] = verticesC3f[nIndex].matrixIndices[1];
			tmpP3fB4f.nb.data[0] = verticesC3f[nIndex].numMatrix;
			vtxP3fB4f.push_back(tmpP3fB4f);

			GRVertexElement::VFC3fP3fB4f tmpC3fP3fB4f;			
			tmpC3fP3fB4f.c.x = verticesC3f[nIndex].r;
			tmpC3fP3fB4f.c.y = verticesC3f[nIndex].g;
			tmpC3fP3fB4f.c.z = verticesC3f[nIndex].b;
			tmpC3fP3fB4f.p.x = verticesC3f[nIndex].x;
			tmpC3fP3fB4f.p.y = verticesC3f[nIndex].y;
			tmpC3fP3fB4f.p.z = verticesC3f[nIndex].z;
			tmpC3fP3fB4f.b.data[0]  = verticesC3f[nIndex].blends[0];
			tmpC3fP3fB4f.b.data[1]  = verticesC3f[nIndex].blends[1];
			tmpC3fP3fB4f.mi.data[0] = verticesC3f[nIndex].matrixIndices[0];
			tmpC3fP3fB4f.mi.data[1] = verticesC3f[nIndex].matrixIndices[1];
			tmpC3fP3fB4f.nb.data[0] = verticesC3f[nIndex].numMatrix;
			vtxC3fP3fB4f.push_back(tmpC3fP3fB4f);

			GRVertexElement::VFC4bP3fB4f tmpC4bP3fB4f;			
			tmpC4bP3fB4f.c = ((unsigned char)(verticesC4f[nIndex].r*255)) |
			 		   ((unsigned char)(verticesC4f[nIndex].g*255) << 8) |
					   ((unsigned char)(verticesC4f[nIndex].b*255) << 16) |
					   ((unsigned char)(verticesC4f[nIndex].a*255) << 24);
			tmpC4bP3fB4f.p.x = verticesC4f[nIndex].x;
			tmpC4bP3fB4f.p.y = verticesC4f[nIndex].y;
			tmpC4bP3fB4f.p.z = verticesC4f[nIndex].z;
			tmpC4bP3fB4f.b.data[0]  = verticesC4f[nIndex].blends[0];
			tmpC4bP3fB4f.b.data[1]  = verticesC4f[nIndex].blends[1];
			tmpC4bP3fB4f.mi.data[0] = verticesC4f[nIndex].matrixIndices[0];
			tmpC4bP3fB4f.mi.data[1] = verticesC4f[nIndex].matrixIndices[1];
			tmpC4bP3fB4f.nb.data[0] = verticesC4f[nIndex].numMatrix;
			vtxC4bP3fB4f.push_back(tmpC4bP3fB4f);

		}
	}
	
	// 頂点フォーマットのデータ型に対するインデックス設定
	for (size_t index=0; index<12; ++index){
		faces.push_back(index);
	}
}

/**
 brief  	シェーダ設定関数
 param	 	なし
 return 	なし
 */
void SetShader(){
	// シェーダの初期化
	render->InitShader();	

	// シェーダオブジェクト作成、ロケーション情報の取得
	render->SetShaderFormat(GRShaderFormat::shP3fB4f);
	shaderP3fB4f = render->CreateShader();
	render->GetShaderLocation(shaderP3fB4f, &locP3fB4f);

	render->SetShaderFormat(GRShaderFormat::shC3fP3fB4f);
	shaderC3fP3fB4f = render->CreateShader();
	render->GetShaderLocation(shaderC3fP3fB4f, &locC3fP3fB4f);

	render->SetShaderFormat(GRShaderFormat::shC4bP3fB4f);
	shaderC4bP3fB4f = render->CreateShader();
	render->GetShaderLocation(shaderC4bP3fB4f, &locC4bP3fB4f);

	// ブレンド変換行列の設定
	Affinef vMatrix0, vMatrix1;
	vMatrix0.data[3][0] = 1.0;		// 右に1だけ平行移動
	vMatrix1.data[3][0] = -1.0;		// 左に1だけ平行移動	
	render->ClearBlendMatrix();
	render->SetBlendMatrix(vMatrix0);
	render->SetBlendMatrix(vMatrix1);
}
/**
 brief		メイン関数
 param		<in/--> argc　　コマンドライン入力の個数
 param		<in/--> argv　　コマンドライン入力
 return		0 (正常終了)
 */
int SPR_CDECL main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	int window = glutCreateWindow("GRVertexBlending");

	grSdk = GRSdkIf::CreateSdk();
	render = grSdk->CreateRender();
	grDevice = grSdk->CreateDeviceGL();
	render->SetDevice(grDevice);
	grDevice->Init();

	// データ設定
	SetVertex();
	// シェーダ設定
	SetShader();

	// シェーダを適用したディスプレイリストを作成する
	listP3fB4f = render->StartList();
	render->SetVertexFormat(GRVertexElement::vfP3fB4f);
	render->SetShaderFormat(GRShaderFormat::shP3fB4f);
	render->DrawIndexed(GRRenderIf::QUADS, &*faces.begin(), &*vtxP3fB4f.begin(), faces.size());
	render->EndList();

	listC3fP3fB4f = render->StartList();
	render->SetVertexFormat(GRVertexElement::vfC3fP3fB4f);
	render->SetShaderFormat(GRShaderFormat::shC3fP3fB4f);
	
	//listC3fP3fB4f = render->CreateShaderIndexedList(shaderC3fP3fB4f, &locC3fP3fB4f, 
	//											GRRenderIf::QUADS, &*faces.begin(), &*vtxC3fP3fB4f.begin(), faces.size());	
	render->EndList();


	listC4bP3fB4f = render->StartList();
	render->SetVertexFormat(GRVertexElement::vfC4bP3fB4f);
	render->SetShaderFormat(GRShaderFormat::shC4bP3fB4f);	
	render->DrawIndexed(GRRenderIf::QUADS, &*faces.begin(), &*vtxC4bP3fB4f.begin(), faces.size());
	render->EndList();

 	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}

