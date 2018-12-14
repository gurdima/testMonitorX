#pragma once
/*---------------------------------------------------------------------------*/
#include "Poco/Logger.h"
#include <string>
/*---------------------------------------------------------------------------*/
class Tracer
{
public:
    Tracer(Poco::Logger& logger, const std::string& func)
    : _logger(logger)
    , _func(func)
    {
        poco_trace(_logger, ">>> " + _func);
    }

    ~Tracer()
    {
        poco_trace(_logger, "<<< " + _func);
    }
private:
    Poco::Logger&       _logger;
    const std::string   _func;
};

#define TRACE()                 Tracer tr(_logger, __func__)
#define TRACE_L(LOGGER)         Tracer tr(LOGGER, __func__)
#define TRACE_F(FUNC)           Tracer tr(_logger, FUNC)
#define TRACE_LF(LOGGER, FUNC)  Tracer tr(LOGGER, FUNC)
