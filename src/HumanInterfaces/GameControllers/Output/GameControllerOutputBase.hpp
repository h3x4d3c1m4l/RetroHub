#include "../../ControllerState.h"

class GameControllerOutputBase
{
public:
    virtual bool SendState(ControllerState controller_state);
};