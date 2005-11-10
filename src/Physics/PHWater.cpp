#include "Physics.h"
#pragma hdrstop
#include "PHWater.h"
#include <Graphics/GRMaterial.h>
#include <ImpD3D/D3Render.h>
#include <ImpD3D/D3Mesh.h>
#include <ImpD3D/D3Material.h>
#include <Graphics/GRCamera.h>
#include <GraphicsGL/GLRender.h>
#include <GraphicsGL/GLMesh.h>
#include <Physics/PHSolid.h>
#include <gl/gl.h>
#include <gl/glu.h>

#pragma hdrstop

namespace Spr{;

////////////////////////////////////////////////////////////////
// PHWaterEngine

SGOBJECTIMP(PHWaterEngine, PHSolverBase);
    // insert new phwater object to the end of the vector
bool PHWaterEngine::AddChildObject(SGObject* o, SGScene* s){
	if (DCAST(PHWater, o)){
		waters.push_back((PHWater*)o);
		return true;
	}
	return false;
}

    // delete the specific phwater object
bool PHWaterEngine::DelChildObject(SGObject* o, SGScene* s){
	PHWater* w = DCAST(PHWater, o);
	if(w){
		waters.Erase(w);
		return true;
	}
	return false;
}

    // call the step function of all phwater objects
    // this method updates information of all phwater objects in the waters vector
void PHWaterEngine::Step(SGScene* s){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Step(s);
	}
}

    // call clearforce method of all phwater objects in the waters vector
void PHWaterEngine::ClearForce(){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->ClearForce();
	}
}

void PHWaterEngine::Clear(SGScene* s){
	waters.clear();
}

void PHWaterEngine::Loaded(SGScene* scene){
	for(PHWaters::iterator it = waters.begin(); it != waters.end(); ++it){
		(*it)->Loaded(scene);
	}
}


/////////////////////////////////////////////////////////////////////////////////
// CDWater
SGOBJECTIMP(CDWater, CDGeometry);

int CDWater::GeometryID(){
	return WATER;
}

void CDWater::CalcBBox(Vec3f& bbMin, Vec3f& bbMax){
	bbMin.element_min(Vec3f(-water->rx, -water->ry, -water->dh*10));
	bbMax.element_max(Vec3f(water->rx, water->ry, water->dh*10));
}

/////////////////////////////////////////////////////////////////////////////////
// PHWater

SGOBJECTIMP(PHWater, GRVisual);
    // constructor
PHWater::PHWater(){
	mx = my = 0;
	dh = 0.0;
	depth = 0.0;
	density = 1.0;
	loss = 0.99;
	bound.x = 3;
	bound.y = 3;
	edgeWidth = 0.0f;
}

    // this function adds new child data to the end of the vector
    // object of SGFrame or PHSolid can be inserted
    // this function returns the result of insertion as a boolean
bool PHWater::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(PHWaterTrackTarget, o)){
		targets = (PHWaterTrackTarget*)o;
		return true;
	}
	if(DCAST(GRMaterial, o)){
		material = (GRMaterial*)o;
		return true;
	}
	return false;
}

    // refer the object
    // if object exists
    // this function returns 1, otherwise returns 0
size_t PHWater::NReferenceObjects(){
	return 0;
}

    // refer the object
    // if i is not 0, return NULL
    // if i is zero and object exists,
    // return object
SGObject* PHWater::ReferenceObject(size_t i){
	return NULL;
}

    // this method calls the init function with the given scene
void PHWater::Loaded(SGScene* scene){
	Init(scene);
}

    // this function initializes local variables
void PHWater::Init(SGScene* scene){
    //xo = -(MX - 1) / 2. * dh;	//x原点	
    //yo = -(MY - 1) / 2. * dh;	//y原点
	int i, j;

    // total number of cells
	mxy = mx * my;

    // dh = width of grid
    // mx, my = number of divisions
	dhinv = 1/dh;
	rx = (mx-1) * dh / 2.0;
	ry = (my-1) * dh / 2.0;

	//その内bad_allocのcatch実装
	height.resize(mx, my);
    htmp.resize(mx, my);
    u.resize(mx, my);
    v.resize(mx, my);
    utmp.resize(mx, my);
    vtmp.resize(mx, my);
    p.resize(mx, my);
    normal.resize(mx, my);
    tvec.resize(mx, my);
    //color.resize(mx, my);

    // initialize height matrix array
	height.clear();

    // temporary height variable
	htmp.clear();

    // initialize normal matrix array
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        normal[i][j].clear();

    // calculate refracted vectors
	for (j = 0; j < my; j++)for(i = 0; i < mx; i++)
        tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);

    // clear velocity variables and pressure
	u.clear();
	utmp.clear();
	v.clear();
	vtmp.clear();
	p.clear();

	//マテリアルの指定がなかった場合、デフォルトを作成
	if(!material){
		material = new GRMaterial;
		material->ambient  = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->specular = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->diffuse  = Vec4d(1.0, 1.0, 1.0, 1.0);
		material->emissive = Vec4d(0.5, 0.5, 0.5, 1.0);
		material->power = 0.5;
		//vlight = Vec3d(0.0, 0.0, -1.0);
	}
	//	描画用の頂点
/*	vtxs.resize(mxy);
	for(int x = 0; x < mx - 1; x++)for(int y = 0; y < my - 1; y++){
		vtxs[y*mx + x].pos = Vec3f(x*dh, y*dh, 0);
		vtxs[y*mx + x].normal = Vec3f(0, 0, 0);
	}
*/	//マテリアル
	materialD3 = new D3Material;
	*materialD3 = *material;
}
double PHWater::LerpHeight(double x, double y){
	static double nx, ny, h0, h1;

    if(x < -rx || x > rx || y < -ry || y > ry)
		return 0.0;

	//座標から格子インデックスを算出
	nx = (x + rx) / dh;
	ny = (y + ry) / dh;
	int ix = floor(nx), iy = floor(ny);
	nx -= (double)ix;
	ny -= (double)iy;
	ix = (ix+bound.x)%mx, iy = (iy+bound.y)%my;
    
    h0 = height[ix       ][iy] * (1.0 - ny) + height[ix       ][(iy+1)%my] * ny;
    h1 = height[(ix+1)%mx][iy] * (1.0 - ny) + height[(ix+1)%mx][(iy+1)%my] * ny;

    return h0 * (1.0 - nx) + h1 * nx;
}
#if 0

double PHWater::LerpVelocityU(double x, double y){
	x += dh/2;
	static double nx, ny, h0, h1;

    if(x < -rx || x > rx || y < -ry || y > ry)
		return 0.0;

	//座標から格子インデックスを算出
	nx = (x + rx) / dh;
	ny = (y + ry) / dh;
	int ix = floor(nx), iy = floor(ny);
	nx -= (double)ix;
	ny -= (double)iy;
	ix = (ix+bound.x)%mx, iy = (iy+bound.y)%my;
    
    h0 = u[ix       ][iy] * (1.0 - ny) + u[ix       ][(iy+1)%my] * ny;
    h1 = u[(ix+1)%mx][iy] * (1.0 - ny) + u[(ix+1)%mx][(iy+1)%my] * ny;

    return h0 * (1.0 - nx) + h1 * nx;
}
double PHWater::LerpVelocityV(double x, double y){
	y += dh/2;
	static double nx, ny, h0, h1;

    if(x < -rx || x > rx || y < -ry || y > ry)
		return 0.0;

	//座標から格子インデックスを算出
	nx = (x + rx) / dh;
	ny = (y + ry) / dh;
	int ix = floor(nx), iy = floor(ny);
	nx -= (double)ix;
	ny -= (double)iy;
	ix = (ix+bound.x)%mx, iy = (iy+bound.y)%my;
    
    h0 = v[ix       ][iy] * (1.0 - ny) + v[ix       ][(iy+1)%my] * ny;
    h1 = v[(ix+1)%mx][iy] * (1.0 - ny) + v[(ix+1)%mx][(iy+1)%my] * ny;

    return h0 * (1.0 - nx) + h1 * nx;
}

#endif

void PHWater::Render(SGFrame* fr, GRRender* render){
	//renderの種類を判定	
	if (render->drawState&GRRender::DRAW_OPAQUE == 0) return;
	CalcNormal();
	if(DCAST(D3Render, render))
		RenderD3(fr, DCAST(D3Render, render));
	if(DCAST(GLRender, render))
		RenderGL(fr, DCAST(GLRender, render));
}

DWORD PHWater::GetColor(float h){
	float hMax = 0.05f;
	float hMin = -0.05f;
	if (h>hMax) h = hMax;
	if (h<hMin) h = hMin;
	float n = h / ((hMax-hMin)/2);
	bool bPlus = n>0;
	if (!bPlus) n *= -1;
	int a = n*255;
	int b = (1-n)*255;
	if (bPlus){
		return D3DCOLOR_ARGB(0xa0,a,b,0);
	}
	return D3DCOLOR_ARGB(0xa0,0,b,a);
}
// draw by Direct3D
const float nmul = 1.0;

void PHWater::RenderD3(SGFrame* fr, D3Render* render){
	render->cr.Enter();
    // copy the head address of the varialbe height array to pheight
	double* pheight = &height[0][0];
    // copy the head address of the variable normal array to pnormal
	Vec3d*  pnormal = &normal[0][0];
	
	if ( (materialD3->bOpaque && render->drawState & GRRender::DRAW_OPAQUE)
		|| (!materialD3->bOpaque && render->drawState & GRRender::DRAW_TRANSPARENT) ){
		//	通常の描画
		if (!render->bDrawDebug){
//			Vec3f viewPoint;
//			viewPoint = GetPosture().inv() * render->camera->data_hase.view.inv().Pos();
			// this function sets the texture
			materialD3->Render(fr, render);
			
			render->device->SetFVF(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2);
			struct VtxFVF{
				Vec3f pos;
				Vec3f normal;
				Vec2f tex;
			};
			const float hmul = 1.0f;
			VtxFVF* buf= new VtxFVF[std::max(mx,my)*2];
			float xo = -rx, yo = -ry;
			float dxinv = 1/rx;
			float dyinv = 1/ry;
			int boundY_1 = (bound.y-1+my)%my;
			for(int y=0; y<my; ++y){
				double py;
				if (y < boundY_1) py = yo + (y-bound.y+my)%my*dh; 
				else if (y>boundY_1) py = yo + (y-bound.y+my)%my*dh;
				else continue;
				int start1 = y*mx;
				int start2 = ((y+1)%my) * mx;
				double left = xo;
				double px = xo;
				for(int x=bound.x; x<mx; ++x){
					buf[(x-bound.x)*2+1].pos	= Vec3f(px, py, pheight[start1+x]*hmul);
					buf[(x-bound.x)*2+1].normal	= pnormal[start1+x];
					buf[(x-bound.x)*2].pos		= Vec3f(px, py+dh, pheight[start2+x]*hmul);
					buf[(x-bound.x)*2].normal	= pnormal[start2+x];
					px += dh;
				}
				int offset = mx - bound.x;
				for(int x=0; x<bound.x; ++x){
					buf[(x+offset)*2+1].pos		= Vec3f(px, py, pheight[start1+x]*hmul);
					buf[(x+offset)*2+1].normal	= pnormal[start1+x];
					buf[(x+offset)*2].pos		= Vec3f(px, py+dh, pheight[start2+x]*hmul);
					buf[(x+offset)*2].normal	= pnormal[start2+x];
					px += dh;
				}
				for(int i=0; i<2*mx; ++i){
#if 1
					buf[i].tex.x = (buf[i].pos.x+texOffset.x*dh)*0.1f
						+  buf[i].normal.x*nmul + 0.5f;
					buf[i].tex.y = (buf[i].pos.y+texOffset.y*dh)*0.1f
						+  buf[i].normal.y*nmul + 0.5f;
#else
					Vec3f dir = (buf[i].pos-viewPoint).unit();
					dir -= 2*dir*buf[i].normal*buf[i].normal;
					buf[i].tex.x = /*(buf[i].pos.x+texOffset.x*dh)*0.1f + */
						dir.x*0.5f + 0.5f;
					buf[i].tex.y = /*(buf[i].pos.y+texOffset.y*dh)*0.1f + */
						dir.y*0.5f + 0.5f;
#endif
				}
				render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
			}
			//	縁を付ける
			if (edgeWidth){
				float dhOut = (rx+edgeWidth)*2 / (mx-1);
				//	上側の縁を付ける．
				for(int i=0; i<2; ++i){
					float pOut = -rx-edgeWidth;
					float px = -rx;
					int cy = (bound.y+1-i*3+my)%my;
					float y1 = i? ry : -ry-edgeWidth;
					float y2 = i? ry+edgeWidth : -ry;
					for(int ix=0; ix<mx; ix++){
						int cx = (ix+bound.x) % mx;
						buf[ix*2+1].pos		= Vec3f(i?px:pOut, y1, height[cx][cy]*hmul);
						buf[ix*2+1].normal	= normal[cx][cy];
						buf[ix*2].pos		= Vec3f(i?pOut:px, y2, height[cx][cy]*hmul);
						buf[ix*2].normal	= normal[cx][cy];
						px += dh;
						pOut += dhOut;
					}
					for(int i=0; i<2*mx; ++i){
						buf[i].tex.x = (buf[i].pos.x+texOffset.x*dh)*0.1f
							+  buf[i].normal.x*nmul + 0.5f;
						buf[i].tex.y = (buf[i].pos.y+texOffset.y*dh)*0.1f
							+  buf[i].normal.y*nmul + 0.5f;
					}
					render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
				}
				dhOut = (ry+edgeWidth)*2 / (my-1);
				//	左側の縁を付ける．
				for(int i=0; i<2; ++i){
					float pOut = -ry-edgeWidth;
					float py = -ry;
					int cx = (bound.x+1-i*3+mx)%mx;
					float x1 = i? rx : -rx-edgeWidth;
					float x2 = i? rx+edgeWidth : -rx;
					for(int iy=0; iy<my; iy++){
						int cy = (iy+bound.y) % my;
						buf[iy*2].pos		= Vec3f(x1, i?py:pOut, height[cx][cy]*hmul);
						buf[iy*2].normal	= normal[cx][cy];
						buf[iy*2+1].pos		= Vec3f(x2, i?pOut:py, height[cx][cy]*hmul);
						buf[iy*2+1].normal	= normal[cx][cy];
						py += dh;
						pOut += dhOut;
					}
					for(int i=0; i<2*my; ++i){
						buf[i].tex.x = (buf[i].pos.x+texOffset.x*dh)*0.1f
							+  buf[i].normal.x*nmul + 0.5f;
						buf[i].tex.y = (buf[i].pos.y+texOffset.y*dh)*0.1f
							+  buf[i].normal.y*nmul + 0.5f;
					}
					render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (my-1)*2, buf, sizeof(buf[0]));
				}
			}
			delete buf;
			render->SetTexture(NULL);
		}else{ //	色で圧力を表現
			render->SetMaterial(GRMaterialData(Vec4f(0,0,0,1),0));
			render->device->SetRenderState(D3DRS_LIGHTING, false);
			render->device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
			struct VtxFVF{
				Vec3f pos;
				DWORD color;
			};
			VtxFVF* buf= new VtxFVF[mx*2];
			float xo = -rx, yo = -ry;
			float dxinv = 1/rx;
			float dyinv = 1/ry;
			int boundY_1 = (bound.y-1+my)%my;
			for(int y=0; y<my; ++y){
				double py;
				if (y < boundY_1) py = yo + (y-bound.y+my)%my*dh; 
				else if (y>boundY_1) py = yo + (y-bound.y+my)%my*dh;
				else continue;
				int start1 = y*mx;
				int start2 = ((y+1)%my) * mx;
				double left = xo;
				double px = xo;
				for(int x=bound.x; x<mx; ++x){
					buf[(x-bound.x)*2+1].pos	= Vec3f(px, py, 0);
					buf[(x-bound.x)*2+1].color	= GetColor(pheight[start1+x]);
					buf[(x-bound.x)*2].pos		= Vec3f(px, py+dh, 0);
					buf[(x-bound.x)*2].color	= GetColor(pheight[start2+x]);
					px += dh;
				}
				int offset = mx - bound.x;
				for(int x=0; x<bound.x; ++x){
					buf[(x+offset)*2+1].pos		= Vec3f(px, py, 0);
					buf[(x+offset)*2+1].color	= GetColor(pheight[start1+x]);
					buf[(x+offset)*2].pos		= Vec3f(px, py+dh, 0);
					buf[(x+offset)*2].color		= GetColor(pheight[start2+x]);
					px += dh;
				}
				render->device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, (mx-1)*2, buf, sizeof(buf[0]));
			}	
			render->device->SetRenderState(D3DRS_LIGHTING, true);
			delete buf;
			//	流速ベクトルを表示
			render->SetLineWidth(2);
			Vec3f vtx[2];
			vtx[0] = Vec3f(0,0,0);
			vtx[1] = Vec3f(rx,0,0);
			render->SetMaterial(GRMaterialData(Vec4f(0.1f,0,0,1),2));
			render->DrawDirect(GRRender::LINES, vtx, vtx+2);
			vtx[0] = Vec3f(0,0,0);
			vtx[1] = Vec3f(0,ry,0);
			render->SetMaterial(GRMaterialData(Vec4f(0,0.1f,0,1),2));
			render->DrawDirect(GRRender::LINES, vtx, vtx+2);
			render->SetLineWidth(3);
			vtx[0] = Vec3f(0,0,0);
			vtx[1] = Vec3f(velocity.x,velocity.y,0);
			render->SetMaterial(GRMaterialData(Vec4f(1,1,1,1),2));
			render->DrawDirect(GRRender::LINES, vtx, vtx+2);
			for(int i=0; i<2; ++i) vtx[i].x+=0.01f;
			render->DrawDirect(GRRender::LINES, vtx, vtx+2);
			for(int i=0; i<2; ++i) vtx[i].x+=0.01f;
			render->DrawDirect(GRRender::LINES, vtx, vtx+2);
		}
#if 0
		render->SetDepthTest(false);
		GRMaterialData mate(Vec4f(1,0,0,1), 4);
		render->SetMaterial(mate);
		std::vector<Vec3f> lines;
		lines.push_back(Vec3f(-rx, -ry, 0));
		lines.push_back(Vec3f(rx, -ry, 0));
		lines.push_back(Vec3f(rx, -ry, 0));
		lines.push_back(Vec3f(rx, ry, 0));
		lines.push_back(Vec3f(rx, ry, 0));
		lines.push_back(Vec3f(-rx, ry, 0));
		lines.push_back(Vec3f(-rx, ry, 0));
		lines.push_back(Vec3f(-rx, -ry, 0));
		render->DrawDirect(GRRender::LINES, &*(lines.begin()), &*(lines.end()));
#endif
		if (render->bDrawDebug){
			render->device->SetRenderState(D3DRS_LIGHTING, false);
			render->device->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
			struct VtxFVF{
				Vec3f pos;
				DWORD color;
			} vtx[4];
			vtx[0].color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
			vtx[1].color = D3DCOLOR_ARGB(0x10, 0xff, 0xff, 0xff);
			vtx[2].color = D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff);
			vtx[3].color = D3DCOLOR_ARGB(0x10, 0xff, 0xff, 0xff);
			for(int iy=0; iy<my; ++iy){
				for(int ix=0; ix<mx; ++ix){
					int cx = (ix+bound.x)%mx;
					int cy = (iy+bound.y)%my;
#if 0	//	速度の格子点に流速を表示
					vtx[0].pos = Vec3f((ix+0.5f)*dh-rx, (iy)*dh-ry, 0);
					float vel =  u[cx][cy] + velocity.x;
					vel *= dh;
					vtx[1].pos = Vec3f((ix+0.5f)*dh-rx + vel, (iy)*dh-ry, 0);

					vtx[2].pos = Vec3f((ix)*dh-rx, (iy+0.5f)*dh-ry, 0);
					vel =  v[cx][cy] + velocity.y;
					vel *= dh;
					vtx[3].pos = Vec3f((ix)*dh-rx, (iy+0.5f)*dh-ry + vel, 0);

					render->device->DrawPrimitiveUP(D3DPT_LINELIST, 4, vtx, sizeof(vtx[0]));
#else	//	高さの格子点に流速を表示
					vtx[0].pos = Vec3f(ix*dh-rx, iy*dh-ry, 0);
					int cx_ = (cx+mx-1)%mx;
					int cy_ = (cy+my-1)%my;
					Vec2f vel = dh * (0.5f * Vec2f(u[cx][cy] + u[cx_][cy], v[cx][cy] + v[cx][cy_]) + velocity);
					vtx[1].pos = Vec3f(ix*dh-rx + vel.x, iy*dh-ry + vel.y, 0);
					render->device->DrawPrimitiveUP(D3DPT_LINELIST, 2, vtx, sizeof(vtx[0]));
#endif
				}
			}
			render->device->SetRenderState(D3DRS_LIGHTING, true);
		}
		render->SetDepthTest(true);
		//	テクスチャを戻す．
		render->device->SetTexture(0,NULL);
	}
	render->cr.Leave();
}

// draw objects by OpenGL
void PHWater::RenderGL(SGFrame* fr, GLRender* render){
    int i = 0, j = 0;
    double xo = -rx, yo = -ry;
    double x = xo, y = yo;
	float dxinv = 1/rx;
	float dyinv = 1/ry;
   
    // rendering is done twice
    // first one is for opaque objects
    // second one is for transparent objects
    if ((material->IsOpaque() && render->drawState & GRRender::DRAW_OPAQUE)
        ||(!material->IsOpaque() && render->drawState & GRRender::DRAW_TRANSPARENT)){
        // texture mapping
        material->Render(fr, render);
        
        // boundY - 1
		int boundY_1 = (bound.y-1+my)%my;
        int y_temp;
        for(int y = 0; y < my; ++y){
	        glBegin(GL_QUAD_STRIP);
            double py;
            y_temp = (y + 1)%my;
                        if (y < boundY_1) py = yo + (y-bound.y+my)%my*dh; 
                        else if (y>boundY_1) py = yo + (y-bound.y+my)%my*dh;
                        else continue;
            
            double px = xo;
            for(int x = bound.x; x < mx; ++x){
                glNormal3f(normal[x][y_temp].x, normal[x][y_temp].y ,normal[x][y_temp].z);
				glTexCoord2f( (px+texOffset.x*dh)*dxinv/4 + normal[x][y_temp].x*nmul + 0.5f,
					(py+dh+texOffset.y*dh)*dyinv/4 + normal[x][y_temp].y*nmul + 0.5f);
				glVertex3f(px, py+dh, height[x][y_temp]);
                
                glNormal3f(normal[x][y].x ,normal[x][y].y ,normal[x][y].z);
				glTexCoord2f( (px+texOffset.x*dh)*dxinv/4 + normal[x][y].x*nmul + 0.5f,
					(py+texOffset.y*dh)*dyinv/4 + normal[x][y].y*nmul + 0.5f);
                glVertex3f(px, py, height[x][y]);
                
                px+= dh;
            }
            
            for(int x = 0; x < bound.x; ++x){
                glNormal3f(normal[x][y_temp].x ,normal[x][y_temp].y ,normal[x][y_temp].z);
				glTexCoord2f( (px+texOffset.x*dh)*dxinv/4 + normal[x][y_temp].x*nmul + 0.5f,
					(py+dh+texOffset.y*dh)*dyinv/4 + normal[x][y_temp].y*nmul + 0.5f);
                glVertex3f(px, py+dh, height[x][y_temp]);
                
                glNormal3f(normal[x][y].x ,normal[x][y].y ,normal[x][y].z);
				glTexCoord2f( (px+texOffset.x*dh)*dxinv/4 + normal[x][y].x*nmul + 0.5f,
					(py+texOffset.y*dh)*dyinv/4 + normal[x][y].y*nmul + 0.5f);
                glVertex3f(px, py, height[x][y]);
                
                px+= dh;
            }
	        glEnd();
        }
        

        // disable the environmental mapping
        glDisable(GL_TEXTURE_GEN_T); 
        glDisable(GL_TEXTURE_GEN_S);
        
        // release the texture
        render->SetTexture(NULL);
    }
}

    // this function adjusts the boundary problem
void PHWater::Bound(){
	int bx1_ = (bound.X()+1) % mx;
	int bx1 = (bound.X()) % mx;
	int bx2 = (bound.X()-1+mx) % mx;
	int bx2_ = (bound.X()-2+mx) % mx;

	int by1_ = (bound.Y()+1) % my;
	int by1 = (bound.Y()) % my;
	int by2 = (bound.Y()-1+my) %my;
	int by2_ = (bound.Y()-2+my) % my;
	
	//行方向がx方向、列方向がy方向なことに注意
	u.row(bx1) = u.row(bx1_);
	u.row(bx2) = u.row(bx2_);
    u.col(by1) = u.col(by1_);
	u.col(by2) = u.col(by2_);

	v.row(bx1) = v.row(bx1_);
	v.row(bx2) = v.row(bx2_);
    v.col(by1) = v.col(by1_);
	v.col(by2) = v.col(by2_);

    // update the height of the wave
/*	height.row(bx1) = height.row(bx1_);
	height.row(bx2) = height.row(bx2_);
	height.col(by1) = height.col(by1_);
	height.col(by2) = height.col(by2_);
	*/
	height.row(bx1).clear();
	height.row(bx2).clear();
	height.col(by1).clear();
	height.col(by2).clear();
}

    // calculate refracted vectors
Vec3d PHWater::calRefracVec(Vec3d n, Vec3d v, double ra) {
    static Vec3d vd, vd_n, tvec;
    static double kf, vn, d;

    vd = v * (1.0 / -abs(v * n));
	vd_n = vd + n;
	kf = ra * ra * vd.square() - vd_n.square();
    if(kf < 0.0) {
        tvec.clear();
        return tvec;
    }
    kf = 1.0/sqrt(kf);

	tvec = kf * vd_n - n;
	tvec.unitize();
    
    return(tvec);
}
 
    // this function calculates and updates the information
    // such as boundary problem, normal vectors,
    // refracted vectors, x-y velocities,
    // and height array by using integrate function 
void PHWater::Step(SGScene* s){
	double dt = s->GetTimeStep();
	Affinef posture = GetPosture();

	Vec3f velW = posture.Rot() * Vec3f(velocity.x, velocity.y, 0);
	posture.Pos() += velW * dt;
	
    // set the boundary condition
    Bound();
	if (targets && targets->targets.size()){
		SGFrame* target = targets->targets[0];
		Affinef af = posture.inv() * target->GetWorldPosture();
		Vec2f diff = af.Pos().sub_vector(0, Vec2f());
		bool bMove = false;
		if (diff.X() > dh){
			posture.Pos() += posture.Rot() * Vec3f(dh, 0, 0);
			bound.x = (bound.x+1) % mx;
			texOffset.x ++;
			bMove = true;
		}
		if (diff.X() < -dh){
			posture.Pos() -= posture.Rot() * Vec3f(dh, 0, 0);
			bound.x = (bound.x-1+mx) % mx;
			texOffset.x --;
			bMove = true;
		}
		if (diff.Y() > dh){
			posture.Pos() += posture.Rot() * Vec3f(0, dh, 0);
			bound.y = (bound.y+1) % my;
			texOffset.y ++;
			bMove = true;
		}
		if (diff.Y() < -dh){
			posture.Pos() -= posture.Rot() * Vec3f(0, dh, 0);
			bound.y = (bound.y-1+my) % my;
			texOffset.y --;
			bMove = true;
		}
		if (bMove){
			Bound();
		}
	}
	frame->SetWorldPosture(posture);

    
	// solve equation
	Integrate(dt);
}
void PHWater::CalcNormal(){
	//法線と屈折ベクトルを計算
	int i,j;
    static Vec3d vv1, vv2, vv;
	float hmul = dh/0.05f;
	for(j = 0; j < my - 1; j++)for(i = 0; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, (height[i][j] - height[i + 1][j    ])*hmul);
		vv2 = Vec3d(0.0, -dh, (height[i][j] - height[i    ][j + 1])*hmul);
		normal[i][j] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
//		tvec[i][j] = calRefracVec(normal[i][j], vlight, 1.3333);
    }
	for(j = 0; j < my - 1; j++){
		vv1 = Vec3d(-dh, 0.0, (height[mx-1][j] - height[0][j    ])*hmul);
		vv2 = Vec3d(0.0, -dh, (height[mx-1][j] - height[mx-1    ][j + 1])*hmul);
		normal[mx-1][j] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
//		tvec[mx-1][j] = calRefracVec(normal[mx-1][j], vlight, 1.3333);
	}
	for(i = 0; i < mx - 1; i++){
		vv1 = Vec3d(-dh, 0.0, (height[i][my-1] - height[i + 1][my-1    ])*hmul);
		vv2 = Vec3d(0.0, -dh, (height[i][my-1] - height[i    ][0])*hmul);
		normal[i][my-1] = (vv1 ^ vv2).unit();        
        // calculate refracted vectors
//		tvec[i][my-1] = calRefracVec(normal[i][my-1], vlight, 1.3333);
	}
	vv1 = Vec3d(-dh, 0.0, (height[mx-1][my-1] - height[0][my-1    ])*hmul);
	vv2 = Vec3d(0.0, -dh, (height[mx-1][my-1] - height[mx-1    ][0])*hmul);
	normal[mx-1][my-1] = (vv1 ^ vv2).unit();        
    // calculate refracted vectors
//	tvec[mx-1][my-1] = calRefracVec(normal[mx-1][my-1], vlight, 1.3333);
}

    // this method calcualtes heights and x-y velocities
void PHWater::Integrate(double dt){
    int i, j;
	double dt_dh = dt / dh;
	double C = gravity * (dt_dh);
#define UPDATE_UV(x0, x1, y0, y1)	\
    utmp[i][j] = loss * (u[x0][y0] - C * (height[x1][y0] - height[x0][y0]));	\
    vtmp[i][j] = loss * (v[x0][y0] - C * (height[x0][y1] - height[x0][y0]));

	//	セルはトーラス状につながっていると考える(上と下，右と左はつながっている)
    // calculate temporary velocities toward the z-axis
	//([0, mx-2][0, my-2])
	for(i = 0; i < mx-1; i++)for(j = 0; j < my-1; j++){
		UPDATE_UV(i, i+1, j, j+1);
    }
	//([0, mx-2][my-1]	last row refers first row
    for(i = 0; i < mx-1; i++){
		UPDATE_UV(i, i+1, my-1, 0);
    }
	//	last column refers first column
	for(j = 0; j < my-1; j++){
		UPDATE_UV(mx-1, 0, j, j+1);
    }
	//	right bottom cell
	UPDATE_UV(mx-1, 0, my-1, 0);

	/*
		x = [i-1, i], y = [j-1, j]の四角領域の高さをh[i][j]とすると
		辺x = i-1からの流入量は
			流速u[i-1][j] * 辺dh * 水深depth * 時間dt
		同様にx = i, y = j - 1, y = jの流入出量を総和したものが四角領域の水量の変化量。
		これに四角領域の面積dh * dhを割れば高さの変化量が得られる

		水深を一定値(depth)としているがこれをdepth + height[i][j]としてみたらどうか。
	 */

	C = depth * dt_dh;
#define UPDATE_H(x0, x1, y0, y1)	\
	htmp[x0][y0] = height[x0][y0] - C * (	\
		(utmp[x0][y0] - utmp[x1][y0]) +	\
		(vtmp[x0][y0] - vtmp[x0][y1]));

    // update temporal heights of all cells
    for(i = 1; i < mx; i++)for(j = 1; j < my; j++){
		UPDATE_H(i, i-1, j, j-1);
	}
    for(i = 1; i < mx; i++){
		UPDATE_H(i, i-1, 0, my-1);
	}
    for(j = 1; j< my; j++){
        UPDATE_H(0, mx-1, j, j-1);
	}
    UPDATE_H(0, mx-1, 0, my-1);

//	const double pass = 300.0;
	const double pass = 100.0;
	C = loss / (pass + 16);

#define LOWPASS(x0, x1, x2, y0, y1, y2)	\
	height[x0][y0] = C * (	\
		htmp[x0][y0] * (4 + pass) +					\
		2.0 * (htmp[x2][y0] + htmp[x1][y0] + htmp[x0][y2] + htmp[x0][y1]) +		\
			   htmp[x2][y2] + htmp[x1][y2] + htmp[x2][y1] + htmp[x1][y1]);

	static int count = 0;
	count ++;
	if (count >= 3){
		count = 0;
		//	ローパスフィルタ
//		for(i = 1; i < mx - 1; i++)for(j = 1; j < my - 1; j++){
//			LOWPASS(i, i+1, i-1, j, j+1, j-1);
//		}

		for(i = 1; i < bound.x-3 ; i++)for(j = 1; j < bound.y - 3; j++){
			LOWPASS(i, i+1, i-1, j, j+1, j-1);
		}
		for(i = 1; i < bound.x-3 ; i++)for(j = bound.y+2; j < my - 1; j++){
			LOWPASS(i, i+1, i-1, j, j+1, j-1);
		}

		for(i = bound.x+2; i<mx-1; i++)for(j = 1; j < bound.y - 3; j++){
			LOWPASS(i, i+1, i-1, j, j+1, j-1);
		}
		for(i = bound.x+2; i<mx-1; i++)for(j = bound.y+3; j < my - 1; j++){
			LOWPASS(i, i+1, i-1, j, j+1, j-1);
		}

		if (2 < bound.y && bound.y < my-3){
			for(i = 1; i < mx - 1; i++){
				LOWPASS(i, i+1, i-1, 0, 1, my-1);
				LOWPASS(i, i+1, i-1, my-1, 0, my-2);
			}
		}
		if (2 < bound.x && bound.x < mx-3){
			for(j = 1; j < my - 1; j++){
				LOWPASS(0, 1, mx-1, j, j+1, j-1);
				LOWPASS(mx-1, 0, mx-2, j, j+1, j-1);
			}
		}
		LOWPASS(0, 1, mx-1, 0, 1, my-1);
		LOWPASS(mx-1, 0, mx-2, 0, 1, my-1);
		LOWPASS(0, 1, mx-1, my-1, 0, my-2);
		LOWPASS(mx-1, 0, mx-2, my-1, 0, my-2);
	}else{
		memcpy(&height[0][0], &htmp[0][0], sizeof(height[0][0])*mxy);
	}

    //u[i][j] = utmp[i][j] * loss;
    //v[i][j] = vtmp[i][j] * loss;
	
	// update variables
	//memcpy(&height[0][0], &htmp[0][0], sizeof(height[0][0])*mxy);
	memcpy(&u[0][0], &utmp[0][0], sizeof(u[0][0])*mxy);
	memcpy(&v[0][0], &vtmp[0][0], sizeof(v[0][0])*mxy);

#undef UPDATE_UV
#undef UPDATE_H
#undef LOWPASS
}

///////////////////////////////////////////////////////////////////////////
// PHWaterTrackTarget

SGOBJECTIMP(PHWaterTrackTarget, SGObject);

bool PHWaterTrackTarget::AddChildObject(SGObject* o, SGScene* s){
	if(DCAST(SGFrame, o)){
		targets.push_back((SGFrame*)o);
		return true;
	}
	if(DCAST(PHSolid, o)){
		PHSolid* solid = (PHSolid*)o;
		if(solid->GetFrame())
			targets.push_back(solid->GetFrame());
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////
// LoadState / SaveState

class PHWaterState{
public:
	/*PHWater* solid;
	Vec3d pos;
	Quaterniond ori;
	Vec3d vel;
	Vec3d angVel;
	Vec3d force;
	Vec3d torque;*/
};

class PHWaterEngineState : public SGBehaviorState, public std::vector<PHWaterState>{
public:
	SGOBJECTDEF(PHWaterEngineState);
};

SGOBJECTIMP(PHWaterEngineState, SGBehaviorState);

void PHWaterEngine::LoadState(const SGBehaviorStates& states){
	PHWaterEngineState* pState = DCAST(PHWaterEngineState, states.GetNext());
	assert(pState);
	PHWaterEngineState& state = *pState;
	/*for(int i = 0; i < state.size(); i++){
		solids[i]->SetFramePosition( state[i].pos );
		solids[i]->SetOrientation( state[i].ori );
		solids[i]->SetVelocity( state[i].vel);
		solids[i]->SetAngularVelocity( state[i].angVel);
		solids[i]->SetForce( state[i].force);
		solids[i]->SetTorque( state[i].torque);
	}*/
}
void PHWaterEngine::SaveState(SGBehaviorStates& states) const{
	UTRef<PHWaterEngineState> state = new PHWaterEngineState;
	states.push_back(state);
	/*for(PHWaters::const_iterator it = solids.begin(); it != solids.end(); ++it){
		state->push_back(PHWaterState());
		state->back().pos = (*it)->GetFramePosition();
		state->back().ori = (*it)->GetOrientation();
		state->back().vel = (*it)->GetVelocity();
		state->back().angVel = (*it)->GetAngularVelocity();
		state->back().force = (*it)->GetForce();
		state->back().torque = (*it)->GetTorque();
	}*/
}

///////////////////////////////////////////////////////////////////////////
// Loader / Saver

DEF_RECORD(XWaterEngine, {
	GUID Guid(){ return WBGuid("a09e46e6-ed5b-4965-9ce6-34aa47f71265"); } 
});

DEF_RECORD(XWater, {
    GUID Guid(){ return WBGuid("ebb9188d-6c15-42aa-b15d-e1c89943ec0c"); } 
	DWORD mx;
	DWORD my;
	FLOAT dh;
	FLOAT depth;
	FLOAT gravity;
	FLOAT density;
	FLOAT loss;
	FLOAT vx;
	FLOAT vy;
	FLOAT edgeWidth;		//	描画時に付けるふちの幅
 });

DEF_RECORD(XWaterTrackTarget, {
	GUID Guid(){ return WBGuid("d1343031-d541-44f0-8ac7-d678837b65a6"); }
});

class PHWaterEngineLoader:public FIObjectLoader<PHWaterEngine>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterEngine* engine){
		//ClearForceを探す→まだ無かったら生成、追加→自身をClearForceに登録
		UTRef<PHSolidClearForce> clearForce;
		ctx->scene->GetBehaviors().Find(clearForce);
		if(!clearForce){
			clearForce= new PHSolidClearForce;
			ctx->scene->GetBehaviors().Add(clearForce);
		}
        // insert the engine to the clearForce variable
		clearForce->solvers.push_back(engine);
		return true;
	}
    // ?
	PHWaterEngineLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterEngine);
	}
};

class PHWaterEngineSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterEngine"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		//保有するPHWaterをSave
		PHWaterEngine* engine = (PHWaterEngine*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHWaterEngine", engine);

        // save doc to the array in the ctx class object
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
        
        // refer all phwater objects
		for(PHWaters::iterator it = engine->waters.begin(); it != engine->waters.end(); ++it){
			ctx->SaveRecursive(*it);
		}
        // delete the last element
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWaterEngine);

class PHWaterLoader:public FIObjectLoader<PHWater>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWater* water){
		XWater data;
		FIDocNodeBase* docWater = ctx->docs.Top();
		docWater->GetWholeData(data);
		water->mx = data.mx;
		water->my = data.my;
		water->dh = data.dh;
		water->depth = data.depth;
		water->gravity = data.gravity;
		water->density = data.density;
		water->loss = data.loss;
		water->velocity.x = data.vx;
		water->velocity.y = data.vy;
		water->edgeWidth = data.edgeWidth;
		water->frame = DCAST(SGFrame, ctx->objects.Top());
		
		UTRef<CDWater> cw = new CDWater;
		cw->water = water;
		water->frame->AddChildObject(cw, ctx->scene);		
		return true;
	}
	PHWaterLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_FIELD(DWORD);
		db->REG_FIELD(FLOAT);
		db->REG_RECORD_PROTO(XWater);
	}
};

class PHWaterSaver:public FIBaseSaver{
	virtual UTString GetType() const{return "PHWater";}
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		PHWater* water = (PHWater*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("Water", water);
		ctx->docs.back()->AddChild(doc);
		XWater data;
		data.mx = water->mx;
		data.my = water->my;
		data.dh = (FLOAT)water->dh;
		data.depth = (FLOAT)water->depth;
		data.gravity = (FLOAT)water->gravity;
		data.density = (FLOAT)water->density;
		data.loss = (FLOAT)water->loss;
		data.vx = water->velocity.x;
		data.vy = water->velocity.y;
		data.edgeWidth = water->edgeWidth;
		doc->SetWholeData(data);
	}
};
DEF_REGISTER_BOTH(PHWater);

class PHWaterTrackTargetLoader:public FIObjectLoader<PHWaterTrackTarget>{
public:
	virtual bool LoadData(FILoadScene* ctx, PHWaterTrackTarget* wtt){
		return true;
	}
	PHWaterTrackTargetLoader(){
		UTRef<FITypeDescDb> db = new FITypeDescDb;
		db->SetPrefix("X");
		db->REG_RECORD_PROTO(XWaterTrackTarget);
	}
};
class PHWaterTrackTargetSaver:public FIBaseSaver{
public:
	virtual UTString GetType() const{ return "PHWaterTrackTarget"; }
	virtual void Save(FISaveScene* ctx, SGObject* arg){
		//保有するPHWaterをSave
		PHWaterTrackTarget* wtt = (PHWaterTrackTarget*)arg;
		FIDocNodeBase* doc = ctx->CreateDocNode("PHWaterTrackTarget", wtt);
		ctx->docs.back()->AddChild(doc);
		ctx->docs.push_back(doc);
		for(SGFrames::iterator it = wtt->targets.begin(); it != wtt->targets.end(); ++it){
			ctx->SaveRecursive(*it);
		}
		ctx->docs.pop_back();
	}
};
DEF_REGISTER_BOTH(PHWaterTrackTarget);

}
