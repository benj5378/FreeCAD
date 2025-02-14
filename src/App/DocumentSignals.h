/***************************************************************************
 *   Copyright (c) 2002 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#ifndef APP_DOCUMENTSIGNALS_H
#define APP_DOCUMENTSIGNALS_H
 
#include <boost_signals2.hpp>

#include "Document.h"


namespace App {
struct Document::Signals {
    /** @name Signals of the document */
    //@{
    // clang-format off
    /// signal before changing an doc property
    boost::signals2::signal<void(const App::Document&, const App::Property&)> beforeChange;
    /// signal on changed doc property
    boost::signals2::signal<void(const App::Document&, const App::Property&)> changed;
    /// signal on new Object
    boost::signals2::signal<void(const App::DocumentObject&)> newObject;
    /// signal on deleted Object
    boost::signals2::signal<void(const App::DocumentObject&)> deletedObject;
    /// signal before changing an Object
    boost::signals2::signal<void(const App::DocumentObject&, const App::Property&)> beforeChangeObject;
    /// signal on changed Object
    boost::signals2::signal<void(const App::DocumentObject&, const App::Property&)> changedObject;
    /// signal on manually called DocumentObject::touch()
    boost::signals2::signal<void(const App::DocumentObject&)> touchedObject;
    /// signal on relabeled Object
    boost::signals2::signal<void(const App::DocumentObject&)> relabelObject;
    /// signal on activated Object
    boost::signals2::signal<void(const App::DocumentObject&)> activatedObject;
    /// signal on created object
    boost::signals2::signal<void(const App::DocumentObject&, Transaction*)> transactionAppend;
    /// signal on removed object
    boost::signals2::signal<void(const App::DocumentObject&, Transaction*)> transactionRemove;
    /// signal on undo
    boost::signals2::signal<void(const App::Document&)> undo;
    /// signal on redo
    boost::signals2::signal<void(const App::Document&)> redo;
    /** signal on load/save document
     * this signal is given when the document gets streamed.
     * you can use this hook to write additional information in
     * the file (like the Gui::Document does).
     */
    boost::signals2::signal<void(Base::Writer&)> saveDocument;
    boost::signals2::signal<void(Base::XMLReader&)> restoreDocument;
    boost::signals2::signal<void(const std::vector<App::DocumentObject*>&, Base::Writer&)> exportObjects;
    boost::signals2::signal<void(const std::vector<App::DocumentObject*>&, Base::Writer&)> exportViewObjects;
    boost::signals2::signal<void(const std::vector<App::DocumentObject*>&, Base::XMLReader&)> importObjects;
    boost::signals2::signal<void(const std::vector<App::DocumentObject*>&, Base::Reader&,
                                 const std::map<std::string, std::string>&)> importViewObjects;
    boost::signals2::signal<void(const std::vector<App::DocumentObject*>&)> finishImportObjects;
    // signal starting a save action to a file
    boost::signals2::signal<void(const App::Document&, const std::string&)> startSave;
    // signal finishing a save action to a file
    boost::signals2::signal<void(const App::Document&, const std::string&)> finishSave;
    boost::signals2::signal<void(const App::Document&)> beforeRecompute;
    boost::signals2::signal<void(const App::Document&, const std::vector<App::DocumentObject*>&)> recomputed;
    boost::signals2::signal<void(const App::DocumentObject&)> recomputedObject;
    // signal a new opened transaction
    boost::signals2::signal<void(const App::Document&, std::string)> openTransaction;
    // signal a committed transaction
    boost::signals2::signal<void(const App::Document&)> commitTransaction;
    // signal an aborted transaction
    boost::signals2::signal<void(const App::Document&)> abortTransaction;
    boost::signals2::signal<void(const App::Document&, const std::vector<App::DocumentObject*>&)> skipRecompute;
    boost::signals2::signal<void(const App::DocumentObject&)> finishRestoreObject;
    boost::signals2::signal<void(const App::Document&, const App::Property&)> changePropertyEditor;
    boost::signals2::signal<void(std::string)> linkXsetValue;
    // clang-format on
    //@}
    // NOLINTEND
};

}  // namespace App

#endif
