#ifndef __GICAME__CONCURRENCY__WAITRESULT_H__
#define __GICAME__CONCURRENCY__WAITRESULT_H__


#include "../common.h"


namespace Gicame::Concurrency {

	enum class WaitResult {
		OK, TIMEOUT_ELAPSED, FAILED
	};

};

#endif
