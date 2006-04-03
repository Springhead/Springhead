#ifndef SPR_PHENGINEIF_H
#define SPR_PHENGINEIF_H

namespace Spr{;
/** \addtogroup gpPhysics */
//@{

///	エンジンの基本クラス
struct PHEngineIf : public SceneObjectIf{
public:
	IF_DEF(PHEngine);
};

//@}
}
#endif
