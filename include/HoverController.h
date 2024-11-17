#ifndef _HOVERCONTROLLER_H_
#define _HOVERCONTROLLER_H_

#include <Arduino.h>
#include "IController.h"
#include "Utils.h"
#include <data_struct.h>
#include "globals.h"

class HoverController : public IController<TDataHover>
{
public:
    HoverController();

    void read(TDataHover &data) override;

private:
    // ISensor<TSensorPMW3901> *pmw3901;
    TSBUSGlobalDefaultValues globalValues;
};

#endif