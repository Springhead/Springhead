/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "GraphicsD3D.h"
#include <d3dx9.h>
#include <algorithm>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;


//----------------------------------------------------------------------------
//	GRVertexElement関連
// 頂点宣言に変換
inline void CreateDeclFromVertexFormat(const GRVertexElement* e, LPDIRECT3DDEVICE9 d3ddevice, LPDIRECT3DVERTEXDECLARATION9* pDecl, size_t* pStride)
{
	D3DVERTEXELEMENT9 decl[MAXD3DDECLLENGTH+1];
	for(int i=0; ; ++i){
		if(i!=0 && memcmp(&e[i], &GRVertexElement(), sizeof(GRVertexElement))==0){
			static const D3DVERTEXELEMENT9 decl_end = D3DDECL_END();
			decl[i] = decl_end;
			break;
		}
		else if(decl[i].Usage==D3DDECLUSAGE_TEXCOORD && decl[i].Method==D3DDECLMETHOD_UV){
			decl[i].Method = D3DDECLMETHOD_DEFAULT;
		}
	}
	assert( SUCCEEDED(d3ddevice->CreateVertexDeclaration(decl, pDecl)) );
	*pStride = D3DXGetDeclVertexSize(decl, 0);
}
// 頂点配列をFVFの順番にあわせつつコピー
static void VFP3f_toFVF(void* p, void* vtx, int count)
{
	memcpy(p, vtx, sizeof(float)*3*count);
}
static void VFC4bP3f_toFVF(void* p, void* vtx, int count)
{
	struct fvf{ Vec3f p; DWORD c; };  fvf* dest = (fvf*)p;
	GRVertexElement::VFC4bP3f* src = (GRVertexElement::VFC4bP3f*)vtx;
	for(; count>0; ++dest, ++src, --count){ dest->p=src->p; dest->c=src->c; }
}
static void VFN3fP3f_toFVF(void* p, void* vtx, int count)
{
	struct fvf{ Vec3f p; Vec3f n; };  fvf* dest = (fvf*)p;
	GRVertexElement::VFN3fP3f* src = (GRVertexElement::VFN3fP3f*)vtx;
	for(; count>0; ++dest, ++src, --count){ dest->p=src->p; dest->n=src->n; }
}
static void VFT2fP3f_toFVF(void* p, void* vtx, int count)
{
	struct fvf{ Vec3f p; Vec2f t; };  fvf* dest = (fvf*)p;
	GRVertexElement::VFT2fP3f* src = (GRVertexElement::VFT2fP3f*)vtx;
	for(; count>0; ++dest, ++src, --count){ dest->p=src->p; dest->t=src->t; }
}
static void VFT2fC4bP3f_toFVF(void* p, void* vtx, int count)
{
	struct fvf{ Vec3f p; DWORD c; Vec2f t; };  fvf* dest = (fvf*)p;
	GRVertexElement::VFT2fC4bP3f* src = (GRVertexElement::VFT2fC4bP3f*)vtx;
	for(; count>0; ++dest, ++src, --count){ dest->p=src->p; dest->c=src->c; dest->t=src->t; }
}
static void VFT2fN3fP3f_toFVF(void* p, void* vtx, int count)
{
	struct fvf{ Vec3f p; Vec3f n; Vec2f t; };  fvf* dest = (fvf*)p;
	GRVertexElement::VFT2fN3fP3f* src = (GRVertexElement::VFT2fN3fP3f*)vtx;
	for(; count>0; ++dest, ++src, --count){ dest->p=src->p; dest->n=src->n; dest->t=src->t; }
}

//----------------------------------------------------------------------------
//	GRDeviceD3D
IF_SPR_OBJECTIMP(GRDeviceD3D, GRDevice);
/// D3Dデバイス。GRDeviceD3D::GetD3DDevice()で取得。
IDirect3DDevice9* GRDeviceD3D::s_d3ddevice;
/// 初期設定
void GRDeviceD3D::Init(){
	assert( GetActiveWindow()!=NULL );

	// Direct3Dの作成
	direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	assert( direct3D!=NULL );
	
	// プレゼンテーションパラメータの初期化
	{
		D3DDISPLAYMODE displayMode;
		RECT           clientRect;

		direct3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);
		GetClientRect(GetActiveWindow(), &clientRect);

		ZeroMemory(&pp, sizeof(D3DPRESENT_PARAMETERS));
		pp.Windowed                   = TRUE;
		pp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
		pp.BackBufferFormat           = displayMode.Format;
		pp.BackBufferWidth            = clientRect.right;
		pp.BackBufferHeight           = clientRect.bottom;
		pp.BackBufferCount            = 1;
		pp.EnableAutoDepthStencil     = TRUE;
		pp.AutoDepthStencilFormat     = D3DFMT_D16;
		pp.FullScreen_RefreshRateInHz = 0;
		pp.hDeviceWindow              = GetActiveWindow();
		pp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

		SetMultiSampleType();
	}

	// デバイスの作成
	D3DCAPS9 capsHAL;  direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &capsHAL);
	D3DCAPS9 capsREF;  direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, &capsREF);
	if((confirmDeviceFunc && !confirmDeviceFunc(&capsHAL, D3DCREATE_HARDWARE_VERTEXPROCESSING)) || FAILED( direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &d3ddevice) )){
	if((confirmDeviceFunc && !confirmDeviceFunc(&capsHAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING)) || FAILED( direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &d3ddevice) )){
	if((confirmDeviceFunc && !confirmDeviceFunc(&capsREF, D3DCREATE_SOFTWARE_VERTEXPROCESSING)) || FAILED( direct3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, pp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &pp, &d3ddevice) )){
		assert( false );
	}
	}
	}
	s_d3ddevice = d3ddevice;

	d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	d3ddevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	d3ddevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	nLights = 0;
	displayLists.push_back(DisplayList());		// ダミー(0番は無効なインデックス)
	textures.push_back(NULL);					// textures[0] = NULL

	deviceLost = false;
	shouldResetWindowRect = false;
}
/// アンチエイリアスを使用するための設定
void GRDeviceD3D::SetMultiSampleType(){
	if(!pp.Windowed){
		for(int n=D3DMULTISAMPLE_16_SAMPLES; n>=D3DMULTISAMPLE_2_SAMPLES; --n){
			if( SUCCEEDED(direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, pp.BackBufferFormat, pp.Windowed, (D3DMULTISAMPLE_TYPE)n, NULL)) ){
				pp.MultiSampleType = (D3DMULTISAMPLE_TYPE)n;
				return;
			}
		}
	}
	pp.MultiSampleType = D3DMULTISAMPLE_NONE;
}
/// デバイスがロストしたら呼ぶ
void GRDeviceD3D::LostDevice(){
	if(deviceLost) return;

	deviceLost = true;
	for(size_t i=0; i<lostDeviceListeners.size(); ++i) { lostDeviceListeners[i](); }
}
/// デバイスをリセットする
void GRDeviceD3D::ResetDevice(){
	if(!deviceLost) return;

	Affinef view, projection;
	d3ddevice->GetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
	d3ddevice->GetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection);
	std::vector<D3DLIGHT9> lights(nLights);
	for(int i=0; i<nLights; ++i) { d3ddevice->GetLight(i, &lights[i]); }
	DWORD ambient;
	d3ddevice->GetRenderState(D3DRS_AMBIENT, &ambient);

	HRESULT hr;
	hr = d3ddevice->TestCooperativeLevel();
	if(hr!=D3D_OK && hr!=D3DERR_DEVICENOTRESET){
		return;
	}
	if(shouldResetWindowRect){
		SetWindowPos(
			pp.hDeviceWindow, HWND_NOTOPMOST,
			windowRectStore.left, windowRectStore.top, windowRectStore.right-windowRectStore.left, windowRectStore.bottom-windowRectStore.top,
			SWP_SHOWWINDOW );
		shouldResetWindowRect = false;
	}
	hr = d3ddevice->Reset(&pp);
	if(hr!=D3D_OK){
		return;
	}
	deviceLost = false;

	d3ddevice->SetTransform(D3DTS_VIEW, (D3DMATRIX*)&view);
	d3ddevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&projection);
	d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	d3ddevice->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	d3ddevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	d3ddevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3ddevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	for(int i=0; i<nLights; ++i) { d3ddevice->SetLight(i, &lights[i]);  d3ddevice->LightEnable(i, TRUE); }
	d3ddevice->SetRenderState(D3DRS_AMBIENT, ambient);
	
	for(size_t i=0; i<resetDeviceListeners.size(); ++i) { resetDeviceListeners[i](); }
}
///	Viewport設定
void GRDeviceD3D::SetViewport(Vec2f pos, Vec2f sz){
	D3DVIEWPORT9 viewport = { (DWORD)pos.x, (DWORD)pos.y, (DWORD)sz.x, (DWORD)sz.y, 0, 1 };
	d3ddevice->SetViewport(&viewport);
	if(pp.BackBufferWidth==sz.x && pp.BackBufferHeight==sz.y)  return;
	pp.BackBufferWidth  = (UINT)sz.x;
	pp.BackBufferHeight = (UINT)sz.y;
	LostDevice();
}
/// バッファクリア
void GRDeviceD3D::ClearBuffer(){
	while(deviceLost){
		ResetDevice();
		if(!deviceLost) break;
		Sleep(100);
	}
	d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
}
/// レンダリングの開始前に呼ぶ関数
void GRDeviceD3D::BeginScene(){
	while(deviceLost){
		ResetDevice();
		if(!deviceLost) break;
		Sleep(100);
	}
	d3ddevice->BeginScene();
}
/// レンダリングの終了後に呼ぶ関数
void GRDeviceD3D::EndScene(){
	assert(!deviceLost);
	d3ddevice->EndScene();
	if(d3ddevice->Present(NULL,NULL,NULL,NULL) == D3DERR_DEVICELOST){
		LostDevice();
	}
}
///	カレントの視点行列をafvで置き換える
void GRDeviceD3D::SetViewMatrix(const Affinef& afv){   
	d3ddevice->SetTransform(D3DTS_VIEW, (const D3DMATRIX*)&afv);
}
/// カレントの投影行列をafpで置き換える
void GRDeviceD3D::SetProjectionMatrix(const Affinef& afp){  
	d3ddevice->SetTransform(D3DTS_PROJECTION, (const D3DMATRIX*)&afp);
}
/// カレントの投影行列を取得する
void GRDeviceD3D::GetProjectionMatrix(const Affinef& afp){  
	d3ddevice->GetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&afp);
}	
/// カレントのモデル行列をafwで置き換える
void GRDeviceD3D::SetModelMatrix(const Affinef& afw){
	d3ddevice->SetTransform(D3DTS_WORLD, (const D3DMATRIX*)&afw);
}
/// カレントのモデル行列に対してafwを掛ける
void GRDeviceD3D::MultModelMatrix(const Affinef& afw){	
	Affinef current;
	d3ddevice->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&current);
	SetModelMatrix(current * afw);
}
/// カレントのモデル行列をモデル行列スタックへ保存する
void GRDeviceD3D::PushModelMatrix(){
	Affinef current;
	d3ddevice->GetTransform(D3DTS_WORLD, (D3DMATRIX*)&current);
	modelMatrixStack.push(current);
}
/// モデル行列スタックから取り出し、カレントのモデル行列とする
void GRDeviceD3D::PopModelMatrix(){
	SetModelMatrix(modelMatrixStack.top());
	modelMatrixStack.pop();
}
/// ブレンド変換行列の全要素を削除する
void GRDeviceD3D::ClearBlendMatrix(){
	DSTR << "GRDeviceD3D::ClearBlendMatrix() is not implemented." << std::endl;
}	
/// ブレンド変換行列を設定する
bool GRDeviceD3D::SetBlendMatrix(const Affinef& afb){
	DSTR << "GRDeviceD3D::SetBlendMatrix() is not implemented." << std::endl;
	return GRDevice::SetBlendMatrix(afb);
}
/// ブレンド変換行列を設定する
bool GRDeviceD3D::SetBlendMatrix(const Affinef& afb, unsigned int id){
	DSTR << "GRDeviceD3D::SetBlendMatrix() is not implemented." << std::endl;
	return GRDevice::SetBlendMatrix(afb, id);
}		
/// 頂点フォーマットの指定
void GRDeviceD3D::SetVertexFormat(const GRVertexElement* e){	
	currentVertexFormat = e;
}
/// 頂点座標を指定してプリミティブを描画
void GRDeviceD3D::SetVertexShader(void* s){
	assert(0);	//	To Be Implemented
}
/// 頂点座標を指定してプリミティブを描画
void GRDeviceD3D::DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){
	assert( ty!=GRRenderBaseIf::QUADS );

	DisplayList dl;

	void (*to_fvf_func)(void*,void*,int);

	if(currentVertexFormat==GRVertexElement::vfP3f){			dl.fvf = D3DFVF_XYZ;								to_fvf_func = VFP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfC4bP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;				to_fvf_func = VFC4bP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfN3fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_NORMAL;				to_fvf_func = VFN3fP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_TEX1;					to_fvf_func = VFT2fP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fC4bP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;	to_fvf_func = VFT2fC4bP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fN3fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_TEX1;	to_fvf_func = VFT2fN3fP3f_toFVF; }
	else{
		dl.fvf = 0;
		to_fvf_func = NULL;
		CreateDeclFromVertexFormat(currentVertexFormat, d3ddevice, &dl.decl, &dl.stride);
	}

	if(dl.fvf!=0)  dl.stride = D3DXGetFVFVertexSize(dl.fvf);

	dl.vtxCount = count;

	/*
	d3ddevice->CreateVertexBuffer(dl.stride * dl.vtxCount, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &dl.vtxBuf, NULL);
	{
		void* p;
		dl.vtxBuf->Lock(0, 0, &p, 0);
		assert(p);
		if(to_fvf_func)  to_fvf_func(p, vtx, dl.vtxCount);
		else             memcpy(p, vtx, dl.stride * dl.vtxCount);
		dl.vtxBuf->Unlock();
	}
	*/
	std::vector<BYTE> vertices(dl.stride*dl.vtxCount);
	if(to_fvf_func)  to_fvf_func(&vertices[0], vtx, dl.vtxCount);
	else             memcpy(&vertices[0], vtx, dl.stride * dl.vtxCount);
	
	int cnt = dl.vtxCount;
	switch(ty) {
		case GRRenderBaseIf::POINTS:			dl.primType = D3DPT_POINTLIST;			dl.primCount = cnt;		break;
		case GRRenderBaseIf::LINES:				dl.primType = D3DPT_LINELIST;			dl.primCount = cnt/2;	break;
		case GRRenderBaseIf::LINE_STRIP:		dl.primType = D3DPT_LINESTRIP;			dl.primCount = cnt-1;	break;
		case GRRenderBaseIf::TRIANGLES:			dl.primType = D3DPT_TRIANGLELIST;		dl.primCount = cnt/3;	break;
		case GRRenderBaseIf::TRIANGLE_STRIP:	dl.primType = D3DPT_TRIANGLESTRIP;		dl.primCount = cnt-2;	break;
		case GRRenderBaseIf::TRIANGLE_FAN:		dl.primType = D3DPT_TRIANGLEFAN;		dl.primCount = cnt-2;	break;
		default:								assert(false);	break;
	}

	if(dl.decl)	d3ddevice->SetVertexDeclaration(dl.decl);
	else		d3ddevice->SetFVF(dl.fvf);
	//d3ddevice->SetStreamSource(0, dl.vtxBuf, 0, dl.stride);
	//d3ddevice->DrawPrimitive(dl.primType, 0, dl.primCount);
	d3ddevice->DrawPrimitiveUP(dl.primType, dl.primCount, &vertices[0], dl.stride);
}
/// 頂点座標とインデックスを指定してプリミティブを描画
void GRDeviceD3D::DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, GLuint* idx, void* vtx, size_t count, size_t stride){
	DSTR << "GRDeviceD3D::DrawIndexed() is not implemented." << std::endl;
}

///	インデックス形式による DiplayList の作成（マテリアル、テクスチャの設定も行う）	
/*
int GRDeviceD3D::CreateIndexedList(GRMaterialIf* mat,  
								  GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){
	displayLists.push_back(DisplayList());

	DisplayList& dl = displayLists.back();

	void (*to_fvf_func)(void*,void*,int);

	if(currentVertexFormat==GRVertexElement::vfP3f){			dl.fvf = D3DFVF_XYZ;								to_fvf_func = VFP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfC4bP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE;				to_fvf_func = VFC4bP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfN3fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_NORMAL;				to_fvf_func = VFN3fP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_TEX1;					to_fvf_func = VFT2fP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fC4bP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;	to_fvf_func = VFT2fC4bP3f_toFVF; }
	else if(currentVertexFormat==GRVertexElement::vfT2fN3fP3f){	dl.fvf = D3DFVF_XYZ | D3DFVF_NORMAL  | D3DFVF_TEX1;	to_fvf_func = VFT2fN3fP3f_toFVF; }
	else{
		dl.fvf = 0;
		to_fvf_func = NULL;
		CreateDeclFromVertexFormat(currentVertexFormat, d3ddevice, &dl.decl, &dl.stride);
	}

	if(dl.fvf!=0)  dl.stride = D3DXGetFVFVertexSize(dl.fvf);

	dl.vtxCount = *std::max_element(idx, idx+count) + 1;

	d3ddevice->CreateVertexBuffer(dl.stride * dl.vtxCount, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &dl.vtxBuf, NULL);
	{
		void* p;
		dl.vtxBuf->Lock(0, 0, &p, 0);
		assert(p);
		if(to_fvf_func)  to_fvf_func(p, vtx, dl.vtxCount);
		else             memcpy(p, vtx, dl.stride * dl.vtxCount);
		dl.vtxBuf->Unlock();
	}

	d3ddevice->CreateIndexBuffer(sizeof(WORD)*count, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &dl.idxBuf, NULL);
	{
		WORD* p;
		dl.idxBuf->Lock(0, 0, (void**)&p, 0);
		assert(p);
		std::copy(idx, idx+count, p);
		dl.idxBuf->Unlock();
	}
	
	int cnt = dl.idxBuf ? count : dl.vtxCount;
	switch(ty) {
		case GRRenderBaseIf::POINTS:			dl.primType = D3DPT_POINTLIST;			dl.primCount = cnt;		break;
		case GRRenderBaseIf::LINES:				dl.primType = D3DPT_LINELIST;			dl.primCount = cnt/2;	break;
		case GRRenderBaseIf::LINE_STRIP:		dl.primType = D3DPT_LINESTRIP;			dl.primCount = cnt-1;	break;
		case GRRenderBaseIf::TRIANGLES:			dl.primType = D3DPT_TRIANGLELIST;		dl.primCount = cnt/3;	break;
		case GRRenderBaseIf::TRIANGLE_STRIP:	dl.primType = D3DPT_TRIANGLESTRIP;		dl.primCount = cnt-2;	break;
		case GRRenderBaseIf::TRIANGLE_FAN:		dl.primType = D3DPT_TRIANGLEFAN;		dl.primCount = cnt-2;	break;
		default:								assert(false);	break;
	}

	dl.mat   = mat;
	dl.texid = LoadTexture( DCAST(GRMaterial,mat)->texname );

	return displayLists.size()-1;
}
*/
///	DisplayListの表示	
void GRDeviceD3D::DrawList(int list){
	if(list==0) return;

	if(displayLists[list].mat) SetMaterial(displayLists[list].mat);
	d3ddevice->SetTexture(0, textures[displayLists[list].texid]);
	if(displayLists[list].decl)	d3ddevice->SetVertexDeclaration(displayLists[list].decl);
	else						d3ddevice->SetFVF(displayLists[list].fvf);
	d3ddevice->SetStreamSource(0, displayLists[list].vtxBuf, 0, displayLists[list].stride);

	if(displayLists[list].idxBuf){
		d3ddevice->SetIndices(displayLists[list].idxBuf);
		d3ddevice->DrawIndexedPrimitive(displayLists[list].primType, 0, 0, displayLists[list].vtxCount, 0, displayLists[list].primCount);
	}
	else{
		d3ddevice->DrawPrimitive(displayLists[list].primType, 0, displayLists[list].primCount);
	}
}
///	DisplayListの解放	
void GRDeviceD3D::ReleaseList(int list){
	displayLists[list] = DisplayList();
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし） .. Vec2f pos
void GRDeviceD3D::DrawFont(Vec2f pos, const std::string str){
	DSTR << "GRDeviceD3D::DrawFont() is not implemented." << std::endl;
}
/// 3次元テキストの描画（GLオンリー版でfontは指定なし）.. Vec3f pos
void GRDeviceD3D::DrawFont(Vec3f pos, const std::string str){
	DSTR << "GRDeviceD3D::DrawFont() is not implemented." << std::endl;
}
/// 3次元テキストの描画 ... Vec2f pos
void GRDeviceD3D::DrawFont(Vec2f pos, const std::string str, const GRFont& font){
	DSTR << "GRDeviceD3D::DrawFont() is not implemented." << std::endl;
}
/// 3次元テキストの描画  ... Vec3f pos
void GRDeviceD3D::DrawFont(Vec3f pos, const std::string str, const GRFont& font){
	DSTR << "GRDeviceD3D::DrawFont() is not implemented." << std::endl;
}
/// 描画の材質の設定
void GRDeviceD3D::SetMaterial(const GRMaterialDesc& mat){
	D3DMATERIAL9 matd3d;
	matd3d.Diffuse  = *(D3DCOLORVALUE*)&mat.diffuse;
	matd3d.Specular = *(D3DCOLORVALUE*)&mat.specular;
	matd3d.Ambient  = *(D3DCOLORVALUE*)&mat.ambient;
	matd3d.Emissive = *(D3DCOLORVALUE*)&mat.emissive;
	matd3d.Power    = mat.power;
	d3ddevice->SetMaterial(&matd3d);
}
/// 描画する点・線の太さの設定
void GRDeviceD3D::SetLineWidth(float w){
	DSTR << "GRDeviceD3D::SetLineWidth() is not implemented." << std::endl;
}
/// 光源スタックをPush
void GRDeviceD3D::PushLight(const GRLightDesc& light){
	D3DLIGHT9 d3dlight = {};
	d3dlight.Type			= (light.position.w==0) ? D3DLIGHT_DIRECTIONAL : (light.spotCutoff==180) ? D3DLIGHT_POINT : D3DLIGHT_SPOT;
	d3dlight.Diffuse		= *(D3DCOLORVALUE*)&light.diffuse;
	d3dlight.Specular		= *(D3DCOLORVALUE*)&light.specular;
	d3dlight.Ambient		= *(D3DCOLORVALUE*)&light.ambient;
	d3dlight.Position		= *(D3DVECTOR*)&light.position;
	d3dlight.Direction		= (light.position.w==0) ? *(D3DVECTOR*)&(-light.position): *(D3DVECTOR*)&light.spotDirection;
	if(d3dlight.Type != D3DLIGHT_DIRECTIONAL){
		d3dlight.Range			= min(light.range, sqrt(FLT_MAX));
		d3dlight.Falloff		= light.spotFalloff;
		d3dlight.Attenuation0	= light.attenuation0;
		d3dlight.Attenuation1	= light.attenuation1;
		d3dlight.Attenuation2	= light.attenuation2;
		d3dlight.Theta			= Radf(light.spotInner);
		d3dlight.Phi			= Radf(light.spotCutoff);
	}
	d3ddevice->SetLight(nLights, &d3dlight);
	d3ddevice->LightEnable(nLights++, TRUE);
}
/// 光源スタックをPop
void GRDeviceD3D::PopLight(){
	d3ddevice->LightEnable(--nLights, FALSE);
}
/// デプスバッファへの書き込みを許可/禁止する
void GRDeviceD3D::SetDepthWrite(bool b){
	DSTR << "GRDeviceD3D::SetDepthWrite() is not implemented." << std::endl;
}
/// デプステストを有効/無効にする
void GRDeviceD3D::SetDepthTest(bool b){
	DSTR << "GRDeviceD3D::SetDepthTest() is not implemented." << std::endl;
}
/// デプスバッファ法に用いる判定条件を指定する
void GRDeviceD3D::SetDepthFunc(GRRenderBaseIf::TDepthFunc f){
	DSTR << "GRDeviceD3D::SetDepthFunc() is not implemented." << std::endl;
}
/// アルファブレンディングを有効/無効にする
void GRDeviceD3D::SetAlphaTest(bool b){
	DSTR << "GRDeviceD3D::SetAlphaTest() is not implemented." << std::endl;
}
/// アルファブレンディングのモード設定(SRCの混合係数, DEST混合係数)
void GRDeviceD3D::SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){
	DSTR << "GRDeviceD3D::SetAlphaMode() is not implemented." << std::endl;
}
/// テクスチャのロード（戻り値：テクスチャID）	
unsigned int GRDeviceD3D::LoadTexture(const std::string filename){
	if(filename.empty()) return 0;
	textures.push_back(NULL);
	if(FAILED(D3DXCreateTextureFromFile(d3ddevice, filename.c_str(), &textures.back()))){
		DSTR << "GRDeviceD3D::LoadTexture():  Failed to load '" << filename.c_str() << "'." << std::endl;
		textures.pop_back();
		return 0;
	}
	return textures.size()-1;
}
/// シェーダの初期化	
void GRDeviceD3D::InitShader(){
	DSTR << "GRDeviceD3D::InitShader() is not implemented." << std::endl;
}	
/// シェーダフォーマットの設定
void GRDeviceD3D::SetShaderFormat(GRShaderFormat::ShaderType type){
	DSTR << "GRDeviceD3D::SetShaderFormat() is not implemented." << std::endl;
}	
/// シェーダオブジェクトの作成	
bool GRDeviceD3D::CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler& shader){
	DSTR << "GRDeviceD3D::CreateShader() is not implemented." << std::endl;
	return GRDevice::CreateShader(vShaderFile, fShaderFile, shader);
}		
/// シェーダオブジェクトの作成、GRDeviceD3D::shaderへの登録（あらかじめShaderFile名を登録しておく必要がある）	
GRHandler GRDeviceD3D::CreateShader(){
	DSTR << "GRDeviceD3D::CreateShader() is not implemented." << std::endl;
	return GRDevice::CreateShader();
}	
/// シェーダのソースプログラムをメモリに読み込み、シェーダオブジェクトと関連付ける	
bool GRDeviceD3D::ReadShaderSource(GRHandler shader, std::string file){
	DSTR << "GRDeviceD3D::ReadShaderSource() is not implemented." << std::endl;
	return GRDevice::ReadShaderSource(shader, file);
}			
/// ロケーション情報の取得（SetShaderFormat()でシェーダフォーマットを設定しておく必要あり）
void GRDeviceD3D::GetShaderLocation(GRHandler shader, void* location){
	DSTR << "GRDeviceD3D::GetShaderLocation() is not implemented." << std::endl;
}	
/// ウインドウモード<->フルスクリーン 切り替え
void GRDeviceD3D::ToggleFullScreen(){
	if(pp.Windowed){
		d3ddevice->GetViewport(&viewportStore);
		GetWindowRect(pp.hDeviceWindow, &windowRectStore);
		HDC displayDC = CreateDC("DISPLAY", NULL, NULL, NULL);
		pp.Windowed                   = FALSE;
		pp.BackBufferWidth            = GetSystemMetrics(SM_CXSCREEN);
		pp.BackBufferHeight           = GetSystemMetrics(SM_CYSCREEN);
		pp.FullScreen_RefreshRateInHz = GetDeviceCaps(displayDC, VREFRESH);
		SetMultiSampleType();
		DeleteDC(displayDC);
	}
	else{
		pp.Windowed                   = TRUE;
		pp.BackBufferWidth            = (UINT)viewportStore.Width;
		pp.BackBufferHeight           = (UINT)viewportStore.Height;
		pp.FullScreen_RefreshRateInHz = 0;
		SetMultiSampleType();
		shouldResetWindowRect = true;
	}
	LostDevice();
	ResetDevice();
}


}	//	Spr

