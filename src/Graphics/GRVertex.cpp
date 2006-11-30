/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
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
GRVertexElement GRVertexElement::Position3f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_POSITION;
	return rv;
}
GRVertexElement GRVertexElement::Normal3f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_NORMAL;
	return rv;
}
GRVertexElement GRVertexElement::TexCoords2f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT2;
	rv.method = GRVEM_UV;
	rv.usage= GRVEU_TEXCOORD;
	return rv;
}
GRVertexElement GRVertexElement::Color4b(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_UBYTE4;
	rv.usage = GRVEU_COLOR;
	return rv;
}
GRVertexElement GRVertexElement::Color4f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_COLOR;
	return rv;
}
GRVertexElement GRVertexElement::Color3f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT3;
	rv.usage = GRVEU_COLOR;
	return rv;
}	
GRVertexElement GRVertexElement::Blend4f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_BLENDWEIGHT;
	return rv;
}
GRVertexElement GRVertexElement::MatrixIndices4f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_BLENDWEIGHT;
	return rv;
}	
GRVertexElement GRVertexElement::NumBones4f(size_t o){
	GRVertexElement rv;
	rv.offset = o;
	rv.type = GRVET_FLOAT4;
	rv.usage = GRVEU_BLENDWEIGHT;
	return rv;
}	
/*
 *  初期化処理
 *	オフセット値を引数で与え、頂点バッファの各メンバに対するオフセット値、データ型などを設定する
 */
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
/*	
	GRVertexElement::TexCoords2f(sizeof(float)*6+4),
	GRVertexElement::Color4b(sizeof(float)*6),
	GRVertexElement::Normal3f(sizeof(float)*3),
	GRVertexElement::Position3f(0),
	GRVertexElement(),	
*/
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4f(sizeof(float)*2),
	GRVertexElement::Normal3f(sizeof(float)*6),
	GRVertexElement::Position3f(sizeof(float)*9),
	GRVertexElement(),			
};
const GRVertexElement* GRVertexElement::typicalFormats[] = {
	GRVertexElement::vfP3f,				GRVertexElement::vfC4bP3f, 
	GRVertexElement::vfN3fP3f,			GRVertexElement::vfC4fN3fP3f, 
	GRVertexElement::vfT2fP3f,			GRVertexElement::vfT2fC4bP3f, 
	GRVertexElement::vfT2fN3fP3f,		GRVertexElement::vfT2fC4fN3fP3f, 
};
	
const GRVertexElement GRVertexElement::vfP3fB4f[] = {
	GRVertexElement::Position3f(0),
	GRVertexElement::Blend4f(sizeof(float)*3),
	GRVertexElement::MatrixIndices4f(sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(float)*11),
	GRVertexElement(),
};
const GRVertexElement GRVertexElement::vfC4bP3fB4f[] = {
	GRVertexElement::Color4b(0),
	GRVertexElement::Position3f(sizeof(char)*4),
	GRVertexElement::Blend4f(sizeof(char)*4+sizeof(float)*3),
	GRVertexElement::MatrixIndices4f(sizeof(char)*4+sizeof(float)*7),
	GRVertexElement::NumBones4f(sizeof(char)*4+sizeof(float)*11),	
	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfC3fP3fB4f[] = {
	GRVertexElement::Color3f(0),
	GRVertexElement::Position3f(sizeof(float)*3),
	GRVertexElement::Blend4f(sizeof(float)*6),
	GRVertexElement::MatrixIndices4f(sizeof(float)*10),
	GRVertexElement::NumBones4f(sizeof(float)*14),
	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfN3fP3fB4f[] = {
	GRVertexElement::Normal3f(0),
	GRVertexElement::Position3f(sizeof(float)*3),
	GRVertexElement::Blend4f(sizeof(float)*6),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*10),	
	GRVertexElement::NumBones4f(sizeof(float)*14),	
	GRVertexElement(),	
};
const GRVertexElement GRVertexElement::vfC4fN3fP3fB4f[] = {
	GRVertexElement::Color4f(sizeof(float)*0),
	GRVertexElement::Normal3f(sizeof(float)*4),
	GRVertexElement::Position3f(sizeof(float)*7),
	GRVertexElement::Blend4f(sizeof(float)*10),		
	GRVertexElement::MatrixIndices4f(sizeof(float)*14),		
	GRVertexElement::NumBones4f(sizeof(float)*18),		
	GRVertexElement(),	
};	
const GRVertexElement GRVertexElement::vfT2fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Position3f(sizeof(float)*2),
	GRVertexElement::Blend4f(sizeof(float)*5),
	GRVertexElement::MatrixIndices4f(sizeof(float)*9),
	GRVertexElement::NumBones4f(sizeof(float)*13),
	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfT2fC4bP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4b(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*2+sizeof(char)*4),
	GRVertexElement::Blend4f(sizeof(float)*5+sizeof(char)*4),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*9+sizeof(char)*4),	
	GRVertexElement::NumBones4f(sizeof(float)*13+sizeof(char)*4),	
	GRVertexElement(),
};	
const GRVertexElement GRVertexElement::vfT2fN3fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Normal3f(sizeof(float)*2),
	GRVertexElement::Position3f(sizeof(float)*5),
	GRVertexElement::Blend4f(sizeof(float)*8),
	GRVertexElement::MatrixIndices4f(sizeof(float)*12),
	GRVertexElement::NumBones4f(sizeof(float)*16),
	GRVertexElement(),	
};	
const GRVertexElement GRVertexElement::vfT2fC4fN3fP3fB4f[] = {
	GRVertexElement::TexCoords2f(0),
	GRVertexElement::Color4f(sizeof(float)*2),
	GRVertexElement::Normal3f(sizeof(float)*6),
	GRVertexElement::Position3f(sizeof(float)*9),
	GRVertexElement::Blend4f(sizeof(float)*12),	
	GRVertexElement::MatrixIndices4f(sizeof(float)*16),	
	GRVertexElement::NumBones4f(sizeof(float)*20),	
	GRVertexElement(),		
};	

const GRVertexElement* GRVertexElement::typicalBlendFormats[] = {
	GRVertexElement::vfP3fB4f,			GRVertexElement::vfC4bP3fB4f,
	GRVertexElement::vfC3fP3fB4f,		GRVertexElement::vfN3fP3fB4f,
	GRVertexElement::vfC4fN3fP3fB4f,	GRVertexElement::vfT2fP3fB4f,
	GRVertexElement::vfT2fC4bP3fB4f,	GRVertexElement::vfT2fN3fP3fB4f,
	GRVertexElement::vfT2fC4fN3fP3fB4f,
};

}
