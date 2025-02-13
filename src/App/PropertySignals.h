

#ifndef APP_PROPERTYSIGNALS_H
#define APP_PROPERTYSIGNALS_H

#include <boost/signals2.hpp>

namespace App {
struct Property::Public {
    boost::signals2::signal<void (const App::Property&)> propertyChanged;
};
}

#endif
