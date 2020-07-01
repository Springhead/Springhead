#ifndef SPRFIOPSTATEHANDLER
#define SPRFIOPSTATEHANDLER

#include <Foundation/SprObject.h>

namespace Spr{
	;

//Op state file handler
struct FIOpStateHandlerIf : public ObjectIf{
	SPR_IFDEF(FIOpStateHandler);

	void saveToFile(ObjectIf* objif, char *filename, float fileVersion);
	void loadFromFile(ObjectIf* objif, char *filename, bool checkName, float fileVersion);


};

}//namespace

#endif