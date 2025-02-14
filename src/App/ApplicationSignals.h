#ifndef APP_APPLICATIONSIGNALS_H
#define APP_APPLICATIONSIGNALS_H

#include <boost_signals2.hpp>

#include "Application.h"

namespace App {

struct Application::Signals {
    // NOLINTBEGIN
    // clang-format off
    /** @name Signals of the Application */
    //@{
    /// signal on new Document
    boost::signals2::signal<void (const Document&, bool)> newDocument;
    /// signal on document getting deleted
    boost::signals2::signal<void (const Document&)> deleteDocument;
    /// signal on already deleted Document
    boost::signals2::signal<void ()> deletedDocument;
    /// signal on relabeling Document (user name)
    boost::signals2::signal<void (const Document&)> relabelDocument;
    /// signal on renaming Document (internal name)
    boost::signals2::signal<void (const Document&)> renameDocument;
    /// signal on activating Document
    boost::signals2::signal<void (const Document&)> activeDocument;
    /// signal on saving Document
    boost::signals2::signal<void (const Document&)> saveDocument;
    /// signal on starting to restore Document
    boost::signals2::signal<void (const Document&)> startRestoreDocument;
    /// signal on restoring Document
    boost::signals2::signal<void (const Document&)> finishRestoreDocument;
    /// signal on pending reloading of a partial Document
    boost::signals2::signal<void (const Document&)> pendingReloadDocument;
    /// signal on starting to save Document
    boost::signals2::signal<void (const Document&, const std::string&)> startSaveDocument;
    /// signal on saved Document
    boost::signals2::signal<void (const Document&, const std::string&)> finishSaveDocument;
    /// signal on undo in document
    boost::signals2::signal<void (const Document&)> undoDocument;
    /// signal on application wide undo
    boost::signals2::signal<void ()> undo;
    /// signal on redo in document
    boost::signals2::signal<void (const Document&)> redoDocument;
    /// signal on application wide redo
    boost::signals2::signal<void ()> redo;
    /// signal before close/abort active transaction
    boost::signals2::signal<void (bool)> beforeCloseTransaction;
    /// signal after close/abort active transaction
    boost::signals2::signal<void (bool)> closeTransaction;
    /// signal on show hidden items
    boost::signals2::signal<void (const Document&)> showHidden;
    /// signal on start opening document(s)
    boost::signals2::signal<void ()> startOpenDocument;
    /// signal on finished opening document(s)
    boost::signals2::signal<void ()> finishOpenDocument;
    //@}


    /** @name Signals of the document
     * This signals are an aggregation of all document. If you only
     * the signal of a special document connect to the document itself
     */
    //@{
    /// signal before change of doc property
    boost::signals2::signal<void (const App::Document&, const App::Property&)> beforeChangeDocument;
    /// signal on changed doc property
    boost::signals2::signal<void (const App::Document&, const App::Property&)> changedDocument;
    /// signal on new Object
    boost::signals2::signal<void (const App::DocumentObject&)> newObject;
    //boost::signals2::signal<void (const App::DocumentObject&)>     m_sig;
    /// signal on deleted Object
    boost::signals2::signal<void (const App::DocumentObject&)> deletedObject;
    /// signal on changed Object
    boost::signals2::signal<void (const App::DocumentObject&, const App::Property&)> beforeChangeObject;
    /// signal on changed Object
    boost::signals2::signal<void (const App::DocumentObject&, const App::Property&)> changedObject;
    /// signal on relabeled Object
    boost::signals2::signal<void (const App::DocumentObject&)> relabelObject;
    /// signal on activated Object
    boost::signals2::signal<void (const App::DocumentObject&)> activatedObject;
    /// signal before recomputed document
    boost::signals2::signal<void (const App::Document&)> beforeRecomputeDocument;
    /// signal on recomputed document
    boost::signals2::signal<void (const App::Document&)> recomputed;
    /// signal on recomputed document object
    boost::signals2::signal<void (const App::DocumentObject&)> objectRecomputed;
    // signal on opened transaction
    boost::signals2::signal<void (const App::Document&, std::string)> openTransaction;
    // signal a committed transaction
    boost::signals2::signal<void (const App::Document&)> commitTransaction;
    // signal an aborted transaction
    boost::signals2::signal<void (const App::Document&)> abortTransaction;
    //@}

    /** @name Signals of property changes
     * These signals are emitted on property additions or removal.
     * The changed object can be any sub-class of PropertyContainer.
     */
    //@{
    /// signal on adding a dynamic property
    boost::signals2::signal<void (const App::Property&)> appendDynamicProperty;
    /// signal on about removing a dynamic property
    boost::signals2::signal<void (const App::Property&)> removeDynamicProperty;
    /// signal on about changing the editor mode of a property
    boost::signals2::signal<void (const App::Document&, const App::Property&)> changePropertyEditor;
    //@}

    /** @name Signals of extension changes
     * These signals are emitted on dynamic extension addition. Dynamic extensions are the ones added by python (c++ ones are part
     * of the class definition, hence not dynamic)
     * The extension in question is provided as parameter.
     */
    //@{
    /// signal before adding the extension
    boost::signals2::signal<void (const App::ExtensionContainer&, std::string extension)> beforeAddingDynamicExtension;
    /// signal after the extension was added
    boost::signals2::signal<void (const App::ExtensionContainer&, std::string extension)> addedDynamicExtension;
    //@}
    // clang-format off
    // NOLINTEND
};

}  // end namespace

#endif