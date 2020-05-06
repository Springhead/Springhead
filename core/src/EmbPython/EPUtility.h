#ifndef EPUTILITY_H
#define EPUTILITY_H

#define EP_ASSERT(_Expression) (void)( (!!(_Expression)) || (throw EPAssertException(#_Expression, __FILE__, __LINE__), 0) )

#endif
