#ifndef P3LUA4RS_H
#define P3LUA4RS_H

#include <plugins/rspqiservice.h>

#include "interface/L4RInterface.h"

const uint16_t RS_SERVICE_TYPE_L4R_PLUGIN   = 0x754c;
//const uint32_t CONFIG_TYPE_L4R_PLUGIN 		= 0x754c3461;

class p3Lua4RS : public RsPQIService, public L4RInterface
{
public:
    p3Lua4RS(RsPluginHandler* rph);

    // p3Config interface
protected:
    virtual bool saveList(bool& cleanup, std::list<RsItem*>& lst);
    virtual bool loadList(std::list<RsItem*>& load);
    RsSerialiser *setupSerialiser();

    // pqiService interface
public:
    virtual int tick();

    // Lua4RSInterface interface
public:
    LuaCore *getCore();
    virtual uint getTickIntervalInSeconds() const;
    virtual void setTickIntervalInSeconds(const uint &value);
    virtual uint getSecondsToStarUpEvent() const;
    virtual void setSecondsToStarUpEvent(const uint &value);    

private:
    // tick thread
    uint        _secondsToStarUpEvent;
    uint        _tickIntervalInSeconds;
    time_t      _lastRun;
    time_t      _initTime;
    bool        _startUpEventTriggered;
    LuaCore*    _luaCore;

    // pqiService interface
public:
    RsServiceInfo getServiceInfo();
};

#endif // P3LUA4RS_H
