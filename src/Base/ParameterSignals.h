
#ifndef BASE_PARAMETERSIGNALS_H
#define BASE_PARAMETERSIGNALS_H

#include <boost_signals2.hpp>

#include "Parameter.h"

struct ParameterManager::Signals {
    boost::signals2::signal<void(ParameterGrp* /*param*/,
                            ParamType /*type*/,
                            const char* /*name*/,
                            const char* /*value*/)>
                        paramChanged;

};

#endif
