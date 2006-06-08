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
	GRVertexElement::TexCoords2f(sizeof(float)*6+4),
	GRVertexElement::Color4b(sizeof(float)*6),
	GRVertexElement::Normal3f(sizeof(float)*3),
	GRVertexElement::Position3f(0),
	GRVertexElement(),	
};

const GRVertexElement* GRVertexElement::typicalFormats[] = {
	GRVertexElement::vfP3f,		GRVertexElement::vfC4bP3f, 
	GRVertexElement::vfN3fP3f,	GRVertexElement::vfC4fN3fP3f, 
	GRVertexElement::vfT2fP3f,		GRVertexElement::vfT2fC4bP3f, 
	GRVertexElement::vfT2fN3fP3f,	GRVertexElement::vfT2fC4fN3fP3f, 
};


}
