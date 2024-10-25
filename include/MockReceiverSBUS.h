#ifndef _MOCK_RECEIVERSBUS_H_
#define _MOCK_RECEIVERSBUS_H_

#include "IReceiver.h"

class MockReceiverSBUS : public IReceiver {
    void read(TDataRC *data) {

    };
    void write(TDataRC *data){

    };
    TDataRC getData(){

    };
};


#endif