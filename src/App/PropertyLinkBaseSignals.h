

#ifndef APP_PROPERTYLINKBASESIGNALS_H
#define APP_PROPERTYLINKBASESIGNALS_H

#include <string>

#include <boost/signals2.hpp>

namespace App {
struct PropertyLinkBase::Public {
    boost::signals2::signal<void(const std::string&, const std::string&)> updateElementRef;
};
}

#endif


