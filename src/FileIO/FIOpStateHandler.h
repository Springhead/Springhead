#ifndef FIOPSTATEHANDLER_H
#define FIOPSTATEHANDLER_H

#include <shlobj.h> 
#include <string.h>
#include <Foundation/Object.h>
#include <FileIO\SprFIOpStateHandler.h>
#include <Physics\PHOpObj.h>


namespace Spr{ ;


class FIOpStateHandler: public Object
{
	SPR_OBJECTDEF(FIOpStateHandler);

public:

	void saveToFile(ObjectIf* objif, char *filename, float fileVersion);
	void loadFromFile(ObjectIf* objif, char *filename, bool checkName, float fileVersion);


};


}
#endif