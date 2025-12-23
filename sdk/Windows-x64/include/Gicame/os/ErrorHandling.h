#ifndef __GICAME__ERRORHANDLING_H__
#define __GICAME__ERRORHANDLING_H__


#include <string>
#include "../common.h"


namespace Gicame::Error {

    GICAME_API int getLastCode();
    GICAME_API std::string getLastErrorAsString();

};


#endif
