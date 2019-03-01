#ifndef CRMOTIONPLANNER_H
#define CRMOTIONPLANNER_H

#include "CRMotionController.h"

namespace Spr {;

/**
  
*/

class CRMotionPlanner : public CRMotionController {
public:
	virtual bool Plan();
};
}

#endif // CRMOTIONPLANNER_H