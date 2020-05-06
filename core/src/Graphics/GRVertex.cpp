/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/SprGRVertex.h>

namespace Spr{;

GRVertexElement::GRVertexElement(){
	stream=0;
	offset=0;
	type=GRVET_UNUSED;
	method=0;
	usage=0;
	usageIndex=0;
}
GRVertexElement GRVertexElement::Position2f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT2;
	rv.usage = GRVEU_POSITION;
	return rv;
}
GRVertexElement GRVertexElement::Position3f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_POSITION;
	return rv;
}
GRVertexElement GRVertexElement::Position4f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_POSITION;
	return rv;
}
GRVertexElement GRVertexElement::Normal3f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_NORMAL;
	return rv;
}
GRVertexElement GRVertexElement::TexCoords2f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT2;
	rv.method = GRVEM_UV;
	rv.usage= GRVEU_TEXCOORD;
	return rv;
}
GRVertexElement GRVertexElement::TexCoords4f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT4;
	rv.method = GRVEM_STRQ;
	rv.usage= GRVEU_TEXCOORD;
	return rv;
}
GRVertexElement GRVertexElement::Color4b(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_UBYTE4;
	rv.usage = GRVEU_COLOR;
	return rv;
}
GRVertexElement GRVertexElement::Color4f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_COLOR;
	return rv;
}
GRVertexElement GRVertexElement::Color3f(size_t o){
	GRVertexElement rv;
	rv.offset = (short)o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_COLOR;
	return rv;
}	

GRVertexElement GRVertexElement::BlendNf(size_t o, size_t n){
	GRVertexElement rv;
	rv.offset = (short)o;
	if (n==1) rv.type = GRVET_FLOAT1;
	if (n==2) rv.type = GRVET_FLOAT2;
	if (n==3) rv.type = GRVET_FLOAT3;
	if (n==4) rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_BLENDWEIGHT;
	return rv;
}
/*
 *  初期化処理
 *	オフセット値を引数で与え、頂点バッファの各メンバに対するオフセット値、データ型などを設定する
 */
const GRVertexElement GRVertexElement::vfP2f[] = {
	GRVertexElement::Position2f(0),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfP3f[] = {
	GRVertexElement::Position3f(0),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfC4bP3f[] = {
	GRVertexElement::Color4b(0),
	GRVertexElement::Position3f(sizeof(char)*4),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfN3fP3f[] = {
	GRVertexElement::Normal3f(0),
	GRVertexElement::Position3f(sizeof(float)*3),
	GRVertexElement(),	
};
const GRVertexElement GRVertexElement::vfC4fN3fP3f[] = {
	GRVertexElement::Color4f(sizeof(float)*0),
	GRVertexElement::Normal3f(sizeof(float)*4),
	GRVertexElement::Position3f(sizeof(float)*7),
	GRVertexElement(),	
};
const GRVertexElement GRVertexElement::vfT2fP3f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Position3f(sizeof(float)*2),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfT4fP4f[] = {
	GRVertexElement::TexCoords4f(0),
	GRVertexElement::Position4f(sizeof(float)*4),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfT2fC4bP3f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4b(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*2+sizeof(char)*4),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfT2fN3fP3f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Normal3f(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*5),
	GRVertexElement(),	
};
const GRVertexElement GRVertexElement::vfT2fC4fN3fP3f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4f(sizeof(float)*2),
	GRVertexElement::Normal3f(sizeof(float)*6),
	GRVertexElement::Position3f(sizeof(float)*9),
	GRVertexElement(),			
};

const GRVertexElement GRVertexElement::vfT4fC4fN3fP4f[] = {
	GRVertexElement::TexCoords4f(0),
	GRVertexElement::Color4f(sizeof(float)*4),
	GRVertexElement::Normal3f(sizeof(float)*8),
	GRVertexElement::Position4f(sizeof(float)*11),
	GRVertexElement(),			
};
const GRVertexElement* GRVertexElement::typicalFormats[] = {
	GRVertexElement::vfP3f,				GRVertexElement::vfC4bP3f, 
	GRVertexElement::vfN3fP3f,			GRVertexElement::vfC4fN3fP3f, 
	GRVertexElement::vfT2fP3f,			GRVertexElement::vfT4fP4f,
	GRVertexElement::vfT2fC4bP3f,		GRVertexElement::vfT2fN3fP3f,
	GRVertexElement::vfT2fC4fN3fP3f, 	GRVertexElement::vfT4fC4fN3fP4f,
	NULL
};
	
const GRVertexElement GRVertexElement::vfP3fB4f[] = {
	GRVertexElement::Position3f(0),
	GRVertexElement::Blend4f(sizeof(float)*3),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfC4bP3fB4f[] = {
	GRVertexElement::Color4b(0),
	GRVertexElement::Position3f(sizeof(char)*4),
	GRVertexElement::Blend4f(sizeof(char)*4+sizeof(float)*3),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfC3fP3fB4f[] = {
	GRVertexElement::Color3f(0),
	GRVertexElement::Position3f(sizeof(float)*3),
	GRVertexElement::Blend4f(sizeof(float)*6),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfN3fP3fB4f[] = {
	GRVertexElement::Normal3f(0),
	GRVertexElement::Position3f(sizeof(float)*3),
	GRVertexElement::Blend4f(sizeof(float)*6),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),	
};
const GRVertexElement GRVertexElement::vfC4fN3fP3fB4f[] = {
	GRVertexElement::Color4f(sizeof(float)*0),
	GRVertexElement::Normal3f(sizeof(float)*4),
	GRVertexElement::Position3f(sizeof(float)*7),
	GRVertexElement::Blend4f(sizeof(float)*10),		
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),	
};	
const GRVertexElement GRVertexElement::vfT2fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Position3f(sizeof(float)*2),
	GRVertexElement::Blend4f(sizeof(float)*5),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfT2fC4bP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4b(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*2+sizeof(char)*4),
	GRVertexElement::Blend4f(sizeof(float)*5+sizeof(char)*4),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfT2fN3fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Normal3f(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*5),
	GRVertexElement::Blend4f(sizeof(float)*8),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),	
};	
const GRVertexElement GRVertexElement::vfT2fC4fN3fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4f(sizeof(float)*2),
	GRVertexElement::Normal3f(sizeof(float)*6),
	GRVertexElement::Position3f(sizeof(float)*9),
	GRVertexElement::Blend4f(sizeof(float)*12),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),	GRVertexElement(),	
};	

const GRVertexElement* GRVertexElement::typicalBlendFormats[] = {
	GRVertexElement::vfP3fB4f,			GRVertexElement::vfC4bP3fB4f,
	GRVertexElement::vfC3fP3fB4f,		GRVertexElement::vfN3fP3fB4f,
	GRVertexElement::vfC4fN3fP3fB4f,	GRVertexElement::vfT2fP3fB4f,
	GRVertexElement::vfT2fC4bP3fB4f,	GRVertexElement::vfT2fN3fP3fB4f,
	GRVertexElement::vfT2fC4fN3fP3fB4f,
	NULL
};
bool CompareVertexFormat(const GRVertexElement* e1, const GRVertexElement* e2){
	int i;
	for(i=0; e1[i].type!=GRVET_UNUSED && e2[i].type!=GRVET_UNUSED; ++i){
		if (memcmp(&e1[i], &e2[i], sizeof(e1[i])) != 0) return false;
	}
	return e1[i].type==GRVET_UNUSED && e2[i].type==GRVET_UNUSED;
}

}
