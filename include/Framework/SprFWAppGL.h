#ifndef SPR_FWAPPGL_H
#define SPR_FWAPPGL_H
#include <Framework/SprFWApp.h>

namespace Spr{;

/** OpenGLのコンテキスト作成機能を持つアプリケーションクラス
 */
class FWAppGL : public FWApp{
public:
	/** @brief OpenGLを使用するための初期化処理を実行する 
	 */
	void CreateRender();
};


}

#endif
