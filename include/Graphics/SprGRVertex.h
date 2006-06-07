#ifndef SPR_GRVERTEX_H
#define SPR_GRVERTEX_H

#include <float.h>
#include <Physics/SprPHSolid.h>
#include <Physics/SprPHScene.h>
#include <Collision/SprCDShape.h>

namespace Spr{;

enum GRVertexElementType {
    GRVET_FLOAT1 = 0,
    GRVET_FLOAT2 = 1,
    GRVET_FLOAT3 = 2,
    GRVET_FLOAT4 = 3,
    GRVET_D3DCOLOR = 4,
    GRVET_UBYTE4 = 5,
    GRVET_SHORT2 = 6,
    GRVET_SHORT4 = 7,
    GRVET_UBYTE4N = 8,
    GRVET_SHORT2N = 9,
    GRVET_SHORT4N = 10,
    GRVET_USHORT2N = 11,
    GRVET_USHORT4N = 12,
    GRVET_UDEC3 = 13,
    GRVET_DEC3N = 14,
    GRVET_FLOAT16_2 = 15,
    GRVET_FLOAT16_4 = 16,
    GRVET_UNUSED = 17
};
enum GRVertexElementMethod {
    GRVEM_DEFAULT = 0,
    GRVEM_PARTIALU = 1,
    GRVEM_PARTIALV = 2,
    GRVEM_CROSSUV = 3,
    GRVEM_UV = 4,
    GRVEM_LOOKUP = 5,
    GRVEM_LOOKUPPRESAMPLED = 6
};
enum GRVertexElementUssage{
    GRVEU_USAGE_POSITION = 0,
    GRVEU_USAGE_BLENDWEIGHT = 1,
    GRVEU_USAGE_BLENDINDICES = 2,
    GRVEU_USAGE_NORMAL = 3,
    GRVEU_USAGE_PSIZE = 4,
    GRVEU_USAGE_TEXCOORD = 5,
    GRVEU_USAGE_TANGENT = 6,
    GRVEU_USAGE_BINORMAL = 7,
    GRVEU_USAGE_TESSFACTOR = 8,
    GRVEU_USAGE_POSITIONT = 9,
    GRVEU_USAGE_COLOR = 10,
    GRVEU_USAGE_FOG = 11,
    GRVEU_USAGE_DEPTH = 12,
    GRVEU_USAGE_SAMPLE = 13
};

/**	頂点フォーマット．DirectX9の D3DVERTEXELEMENT を参照．
	GRRender->SetVertexFormat()に配列を渡す．
	GRRenderは，配列を見て，適当なシェーダーを設定する．
	シェーダーが見つからない場合は，NULLシェーダーを設定する＝描画されない．
	DrawDirect/DrawIndexed はシェーダーを使って描画する．
*/
struct GRVertexElement {
    short stream;
    short offset;
    unsigned char type;
    unsigned char method;
    unsigned char usage;
    unsigned char usageIndex;
};

}
#endif