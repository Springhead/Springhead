#include "FINodeHandler.h"
namespace Spr{;

#define REGISTER(x)	\
	FINodeHandlers::theNodeHandlers->push_back(DBG_NEW FI##x##Handler);

UTRef<FINodeHandlers> FINodeHandlers::theNodeHandlers;

void RegisterNodeHandlers(){
	if (!FINodeHandlers::theNodeHandlers)
		FINodeHandlers::theNodeHandlers = DBG_NEW FINodeHandlers;
	//REGISTER();
}


}
