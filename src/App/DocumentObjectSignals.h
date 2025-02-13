#ifndef APP_DOCUMENTOBJECTSIGNALS_H
#define APP_DOCUMENTOBJECTSIGNALS_H

#include <boost/signals2.hpp>

#include "DocumentObject.h"

namespace App {

struct DocumentObject::Signals {
    /// signal before changing a property of this object
    boost::signals2::signal<void(const App::DocumentObject&, const App::Property&)> beforeChange;
    /// signal on changed  property of this object
    boost::signals2::signal<void(const App::DocumentObject&, const App::Property&)> changed;
    /// signal on changed property of this object before document scoped signalChangedObject
    boost::signals2::signal<void(const App::DocumentObject&, const App::Property&)> earlyChanged;
};

}  // namespace App

#endif
