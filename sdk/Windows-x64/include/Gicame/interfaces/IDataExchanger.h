#ifndef __GICAME__IDATAEXCHANGER_H__
#define __GICAME__IDATAEXCHANGER_H__


#include "../common.h"
#include "./IReceiver.h"
#include "./ISender.h"


namespace Gicame {

    class GICAME_API IDataExchanger : public IReceiver, public ISender {
    
    public:
        virtual ~IDataExchanger() = default;
        virtual bool isConnected() const { return isReceiverConnected() && isSenderConnected(); }

    };

};


#endif
