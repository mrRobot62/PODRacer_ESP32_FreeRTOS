#ifndef _HOVERCONTROLLER_H_
#define _HOVERCONTROLLER_H_

#include <Arduino.h>
#include "IController.h"
#include <data_struct.h>

class HoverController : public IController<TDataHover>{
    public:
        HoverController();

        void read(TDataHover &data) override;

    private:

};

#endif