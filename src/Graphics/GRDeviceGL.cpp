#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#ifdef _MSC_VER
# include <WinBasis/WinBasis.h>
# include <windows.h>
#endif
#include <GL/glut.h>



namespace Spr {;
//----------------------------------------------------------------------------
//	GRDeviceGL
IF_OBJECT_IMP(GRDeviceGL, GRDevice);
/// 初期設定
void GRDeviceGL::Init(){
	glDrawBuffer(GL_BACK);
	nLights = 0;
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glDisable(GL_CULL_FACE);

	// 視点行列の設定
	viewMatrix.Pos() = Vec3f(0.0, 0.0, 1.0);	                        // eye
	viewMatrix.LookAtGL(Vec3f(0.0, 0.0, 0.0), Vec3f(0.0, 1.0, 0.0));	// center, up 
	viewMatrix = viewMatrix.inv();			
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
}
/// バッファクリア
void GRDeviceGL::ClearBuffer(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
/// レンダリングの開始前に呼ぶ関数
void GRDeviceGL::BeginScene(){
  /// GLでレンダリングする場合、特に処理する必要なし
}
/// レンダリングの終了後に呼ぶ関数
void GRDeviceGL::EndScene(){
  /// ダブルバッファモード時、カレントウィンドウのバッファ交換を行う
	glutSwapBuffers();
}
/// モデル行列をかける
void GRDeviceGL::MultModelMatrix(const Affinef& afw){	
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(afw);
}
/// モデル行列の行列スタックをPush
void GRDeviceGL::PushModelMatrix(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}
/// モデル行列の行列スタックをPop
void GRDeviceGL::PopModelMatrix(){
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
/// モデル行列を設定
void GRDeviceGL::SetModelMatrix(const Affinef& afw){
	modelMatrix = afw;		// モデル行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix * modelMatrix);
}
///	視点行列を設定
void GRDeviceGL::SetViewMatrix(const Affinef& afv){   
	viewMatrix  = afv;		// 視点行列の保存
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(viewMatrix);
}
/// 投影行列を設定
void GRDeviceGL::SetProjectionMatrix(const Affinef& afp){  
	projectionMatrix = afp;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(projectionMatrix);
	glMatrixMode(GL_MODELVIEW);
}

/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::SetVertexFormat(const GRVertexElement* e){
	if (e == GRVertexElement::vfP3f) {
		vertexFormatGl = GL_V3F; 
		vertexSize = sizeof(float)*3;
	}else if (e == GRVertexElement::vfC4bP3f){
		vertexFormatGl = GL_C4UB_V3F;
		vertexSize = sizeof(float)*3+sizeof(char)*4;
	}else if (e == GRVertexElement::vfN3fP3f){
		vertexFormatGl = GL_N3F_V3F;
		vertexSize = sizeof(float)*6;
	}else if (e == GRVertexElement::vfC4fN3fP3f){
		vertexFormatGl = GL_C4F_N3F_V3F;
		vertexSize = sizeof(float)*10;
	}else if (e == GRVertexElement::vfT2fP3f){
		vertexFormatGl = GL_T2F_V3F;
		vertexSize = sizeof(float)*5;
	}else if (e == GRVertexElement::vfT2fC4bP3f){
		vertexFormatGl = GL_T2F_C4UB_V3F;
		vertexSize = sizeof(float)*5 + sizeof(char)*4;
	}else if (e == GRVertexElement::vfT2fN3fP3f){
		vertexFormatGl = GL_T2F_N3F_V3F;
		vertexSize = sizeof(float)*8;
	}else if (e == GRVertexElement::vfT2fC4fN3fP3f){
		vertexFormatGl = GL_T2F_C4F_N3F_V3F;
		vertexSize = sizeof(float)*12;
	}else {
		vertexFormatGl = 0;
		vertexSize = 0;
		assert(0);
	}
}
/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::SetVertexShader(void* s){
	assert(0);	//	To Be Implemented
}

/// 頂点座標を指定してプリミティブを描画
void GRDeviceGL::DrawDirect(TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	GLenum mode = GL_TRIANGLES;
	switch(ty) {
		case POINTS:			mode = GL_POINTS;			break;
		case LINES:				mode = GL_LINES;			break;
		case LINE_STRIP:		mode = GL_LINE_STRIP;		break;
		case TRIANGLES:			mode = GL_TRIANGLES;		break;
		case TRIANGLE_STRIP:	mode = GL_TRIANGLE_STRIP;	break;
		case TRIANGLE_FAN:		mode = GL_TRIANGLE_FAN;		break;
		case QUADS:				mode = GL_QUADS;			break;
		default:				/* DO NOTHING */			break;
	}
	if (!stride) stride = vertexSize;
	glInterleavedArrays(vertexFormatGl, stride, vtx);
	glDrawArrays(mode, 0, count);
}
/// 頂点座標とインデックスを指定してプリミティブを描画
void GRDeviceGL::DrawIndexed(TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	GLenum mode = GL_TRIANGLES;
	switch(ty) {
		case POINTS:			mode = GL_POINTS;			break;
		case LINES:				mode = GL_LINES;			break;
		case LINE_STRIP:		mode = GL_LINE_STRIP;		break;
		case TRIANGLES:			mode = GL_TRIANGLES;		break;
		case TRIANGLE_STRIP:	mode = GL_TRIANGLE_STRIP;	break;
		case TRIANGLE_FAN:		mode = GL_TRIANGLE_FAN;		break;
		case QUADS:				mode = GL_QUADS;			break;
		default:				/* DO NOTHING */			break;
	}
	if (!stride) stride = vertexSize;
	glInterleavedArrays(vertexFormatGl, stride, vtx);
	glDrawElements(mode, count, GL_UNSIGNED_INT, idx);
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし） .. Vec2f pos
void GRDeviceGL::DrawFont(Vec2f pos, const std::string str){
	glDisable(GL_LIGHTING);
	glRasterPos2fv(pos);
	std::string::const_iterator iter;
	for (iter = str.begin(); iter != str.end();	++iter) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *iter);
	}
	glEnable(GL_LIGHTING);
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし）.. Vec3f pos
void GRDeviceGL::DrawFont(Vec3f pos, const std::string str){
	glDisable(GL_LIGHTING);
	glRasterPos3fv(pos);
	std::string::const_iterator iter;
	for (iter = str.begin(); iter != str.end();	++iter) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *iter);
	}
	glEnable(GL_LIGHTING);
}
/// 3次元テキストの描画 ... Vec2f pos
void GRDeviceGL::DrawFont(Vec2f pos, const std::string str, const GRFont& font){
/// VC版のみfontをサポートする。
#ifdef _MSC_VER	
	GLuint	base = 0;							// DisplayList開始指標番号
	bool	findFont = false;				// fontListの検索結果
	GLsizei	range	 = 256;					//	生成するディスプレイリストの数
	unsigned long color=font.color;			//	フォントの色

	if (fontList.size() > 0) {		// fontListが空でない場合は、fontListの検索が必要
		// ストアサイズ10以上の場合、先頭を削除	
		if (fontList.size() > 10) {		
			fontList.erase(fontList.begin());	
		}
		// fontListを検索
		std::map<unsigned int, GRFont>::iterator itr = fontList.begin();
		while((itr != fontList.end()) && (findFont == false)) {
			if (((*itr).second.height == font.height)
				&& ((*itr).second.width == font.width)
				&& ((*itr).second.weight == font.weight)
				&& ((*itr).second.bItalic == font.bItalic)
				&& ((strcmp((*itr).second.face.c_str(),font.face.c_str()))==0)
				&& ((*itr).second.color == font.color)){
					findFont = true;
					base = (*itr).first;
					color = (*itr).second.color;
			} 
			++itr;
		}
	}
	if (findFont == false ) {		// 新規登録の場合
		// 引数で指定された特性を持つ論理フォントを作成する
		HFONT		hFont;			// フォントハンドル
		HFONT		hOldFont;	
		fontBase = glGenLists(range);
		// フォントの作成
		hFont = CreateFont(font.height,						//	フォントの高さ
							font.width,						//	平均文字幅
							0,								//	文字送り方向のX軸の角度
							0,								//	各文字とX軸の角度
							font.weight,					//	フォントの太さ
							font.bItalic,					//	イタリック体
							FALSE,							//	アンダーライン
							FALSE,							//	打ち消し線付
							ANSI_CHARSET,					//	フォント文字セット
							OUT_DEFAULT_PRECIS,				//	出力精度
							CLIP_DEFAULT_PRECIS,			//	クリッピング精度
							ANTIALIASED_QUALITY,			//	出力品質
							FF_DONTCARE | DEFAULT_PITCH,	//	ピッチとファミリ(文字間隔と書体)
							font.face.c_str());				//	タイプフェイス
		
		assert(hFont);
		HDC hDC = GetDC(0);
		// 0から256のコードの文字を、DisplayListのbase番目から登録.
		// wglUseFontBitmaps()関数、使用して、生成したフォントをディスプレイリストに割当てる.
		hOldFont = (HFONT)SelectObject(hDC, hFont);			
		wglUseFontBitmaps(hDC, 0, range, fontBase);		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);		
		// fontListへ登録
		fontList.insert(std::map<unsigned int, GRFont>::value_type(fontBase, font));	
		base = fontBase;	
	}

	glDisable(GL_LIGHTING);
	glColor3f(((font.color >> 16) & 0xFF) / 255.0,
			  ((font.color >>  8) & 0xFF) / 255.0,
			  ((font.color >>  0) & 0xFF) / 255.0);
	glRasterPos2fv(pos);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base);											// 	ディスプレイリストを渡す.	
	glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());		// 文字列を渡す.
	glPopAttrib();
	glEnable(GL_LIGHTING);
#else	// _MSC_VER	
	// VC以外の環境では、fontをサポートしない。
	DrawFont(pos, str);
#endif	
}
/// 3次元テキストの描画  ... Vec3f pos
void GRDeviceGL::DrawFont(Vec3f pos, const std::string str, const GRFont& font){
/// VC版のみfontをサポートする。
#ifdef _MSC_VER		
	GLuint	base = 0;							// DisplayList開始指標番号
	bool	findFont = false;				// fontListの検索結果
	GLsizei	range	 = 256;					//	生成するディスプレイリストの数
	unsigned long color=font.color;			//	フォントの色

	if (fontList.size() > 0) {		// fontListが空でない場合は、fontListの検索が必要
		// ストアサイズ10以上の場合、先頭を削除	
		if (fontList.size() > 10) {		
			fontList.erase(fontList.begin());	
		}
		// fontListを検索
		std::map<unsigned int, GRFont>::iterator itr = fontList.begin();
		while((itr != fontList.end()) && (findFont == false)) {
			if (((*itr).second.height == font.height)
				&& ((*itr).second.width == font.width)
				&& ((*itr).second.weight == font.weight)
				&& ((*itr).second.bItalic == font.bItalic)
				&& ((strcmp((*itr).second.face.c_str(),font.face.c_str()))==0)
				&& ((*itr).second.color == font.color)){
					findFont = true;
					base = (*itr).first;
					color = (*itr).second.color;
			} 
			++itr;
		}
	}
	if (findFont == false ) {		// 新規登録の場合
		// 引数で指定された特性を持つ論理フォントを作成する
		HFONT		hFont;			// フォントハンドル
		HFONT		hOldFont;	
		fontBase = glGenLists(range);
		// フォントの作成
		hFont = CreateFont(font.height,						//	フォントの高さ
							font.width,						//	平均文字幅
							0,								//	文字送り方向のX軸の角度
							0,								//	各文字とX軸の角度
							font.weight,					//	フォントの太さ
							font.bItalic,					//	イタリック体
							FALSE,							//	アンダーライン
							FALSE,							//	打ち消し線付
							ANSI_CHARSET,					//	フォント文字セット
							OUT_DEFAULT_PRECIS,				//	出力精度
							CLIP_DEFAULT_PRECIS,			//	クリッピング精度
							ANTIALIASED_QUALITY,			//	出力品質
							FF_DONTCARE | DEFAULT_PITCH,	//	ピッチとファミリ(文字間隔と書体)
							font.face.c_str());				//	タイプフェイス
		
		assert(hFont);
		HDC hDC = GetDC(0);
		// 0から256のコードの文字を、DisplayListのbase番目から登録.
		// wglUseFontBitmaps()関数、使用して、生成したフォントをディスプレイリストに割当てる.
		hOldFont = (HFONT)SelectObject(hDC, hFont);			
		wglUseFontBitmaps(hDC, 0, range, fontBase);		
		SelectObject(hDC, hOldFont);
		DeleteObject(hFont);		
		// fontListへ登録
		fontList.insert(std::map<unsigned int, GRFont>::value_type(fontBase, font));	
		base = fontBase;	
	}

	glDisable(GL_LIGHTING);
	glColor3f(((font.color >> 16) & 0xFF) / 255.0,
			  ((font.color >>  8) & 0xFF) / 255.0,
			  ((font.color >>  0) & 0xFF) / 255.0);
	glRasterPos3fv(pos);
	glPushAttrib(GL_LIST_BIT);
	glListBase(base);											// 	ディスプレイリストを渡す.	
	glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());		// 文字列を渡す.
	glPopAttrib();
	glEnable(GL_LIGHTING);
#else	// _MSC_VER	
	// VC以外の環境では、fontをサポートしない。
	DrawFont(pos, str);
#endif	
}
/// 描画の材質の設定
void GRDeviceGL::SetMaterial(const GRMaterialDesc& mat){
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat.ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat.diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat.specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,  mat.emissive);
	glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat.power);
}
/// 描画する点・線の太さの設定
void GRDeviceGL::SetLineWidth(float w){
	glLineWidth(w);
}
/// 光源スタックをPush
void GRDeviceGL::PushLight(const GRLightDesc& light){
	if (nLights < GL_MAX_LIGHTS) {
		glLightfv(GL_LIGHT0+nLights, GL_AMBIENT, light.ambient);
		glLightfv(GL_LIGHT0+nLights, GL_DIFFUSE, light.diffuse);
		glLightfv(GL_LIGHT0+nLights, GL_SPECULAR, light.specular);
		glLightfv(GL_LIGHT0+nLights, GL_POSITION, light.position);
		if (light.position.W()){
			// attenuation factor = 1/(GL_CONSTANT_ATTENUATION + GL_LINEAR_ATTENUATION * d + GL_QUADRATIC_ATTENUATION * d^2)
			glLightf(GL_LIGHT0+nLights, GL_CONSTANT_ATTENUATION, light.attenuation0);		// 	一定減衰率
			glLightf(GL_LIGHT0+nLights, GL_LINEAR_ATTENUATION, light.attenuation1);			//	線形減衰率
			glLightf(GL_LIGHT0+nLights, GL_QUADRATIC_ATTENUATION, light.attenuation2);		//	2次減衰率
			glLightfv(GL_LIGHT0+nLights, GL_SPOT_DIRECTION, light.spotDirection);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_EXPONENT, light.spotFalloff);
			glLightf(GL_LIGHT0+nLights, GL_SPOT_CUTOFF, light.spotCutoff);
		}
		glEnable(GL_LIGHT0+nLights);
	}
	nLights ++;
}
/// 光源スタックをPop
void GRDeviceGL::PopLight(){
	nLights--;
	if (nLights < GL_MAX_LIGHTS) glDisable(GL_LIGHT0+nLights);
}
/// デプスバッファへの書き込みを許可/禁止する
void GRDeviceGL::SetDepthWrite(bool b){
	glDepthMask(b);
}
/// デプステストを有効/無効にする
void GRDeviceGL::SetDepthTest(bool b){
if (b) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
}
/// デプスバッファ法に用いる判定条件を指定する
void GRDeviceGL::SetDepthFunc(TDepthFunc f){
	GLenum dfunc = GL_LESS;
	switch(f){
		case DF_NEVER:		dfunc = GL_NEVER;		break;
		case DF_LESS:		dfunc = GL_LESS;		break;
		case DF_EQUAL:		dfunc = GL_EQUAL;		break;
		case DF_LEQUAL:		dfunc = GL_LEQUAL;		break;
		case DF_GREATER:	dfunc = GL_GREATER;		break;
		case DF_NOTEQUAL:	dfunc = GL_NOTEQUAL;	break;
		case DF_GEQUAL:		dfunc = GL_GEQUAL;		break;
		case DF_ALWAYS:		dfunc = GL_ALWAYS;		break;
		default:			/* DO NOTHING */		break;

	}
	glDepthFunc(dfunc);
}
/// アルファブレンディングを有効/無効にする
void GRDeviceGL::SetAlphaTest(bool b){
if (b) glEnable(GL_BLEND);
	else glDisable(GL_BLEND);
}
/// アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
void GRDeviceGL::SetAlphaMode(TBlendFunc src, TBlendFunc dest){
	TBlendFunc  bffac[2] = { src, dest };
	GLenum glfac[2];

	for (int iCnt=0; iCnt<2; ++iCnt){
		switch(bffac[iCnt]) {
			case BF_ZERO:				glfac[iCnt] = GL_ZERO;					break;
			case BF_ONE:				glfac[iCnt] = GL_ONE;					break;
			case BF_SRCCOLOR:			glfac[iCnt] = GL_SRC_COLOR;				break;
			case BF_INVSRCCOLOR:		glfac[iCnt] = GL_ONE_MINUS_SRC_COLOR;	break;
			case BF_SRCALPHA:			glfac[iCnt] = GL_SRC_ALPHA;				break;
			case BF_INVSRCALPHA:		glfac[iCnt] = GL_ONE_MINUS_SRC_ALPHA;	break;
			case BF_DESTALPHA:			glfac[iCnt] = GL_DST_ALPHA;				break;
			case BF_INVDESTALPHA:		glfac[iCnt] = GL_ONE_MINUS_DST_ALPHA;	break;
			case BF_DESTCOLOR:			glfac[iCnt] = GL_DST_COLOR;				break;
			case BF_INVDESTCOLOR:		glfac[iCnt] = GL_ONE_MINUS_DST_COLOR;	break;
			case BF_SRCALPHASAT:		glfac[iCnt] = GL_SRC_ALPHA_SATURATE;	break;
			case BF_BOTHINVSRCALPHA:	glfac[iCnt] = 0;						break;
			case BF_BLENDFACTOR:		glfac[iCnt] = 0;						break;
			case BF_INVBLENDFACTOR:		glfac[iCnt] = 0;						break;
			default:					/* DO NOTHING */						break;
		}
	}
	glBlendFunc(glfac[0], glfac[1]);

}

}	//	Spr

