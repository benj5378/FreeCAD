/***************************************************************************
 *   Copyright (c) 2009 Werner Mayer <wmayer[at]users.sourceforge.net>     *
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


#include "PreCompiled.h"
#ifndef _PreComp_
#include <functional>
#endif

#include <CXX/Objects.hxx>
#include "Application.h"
#include "ApplicationSignals.h"
#include "Document.h"
#include "DocumentObject.h"
#include "DocumentObserverPython.h"
#include <Base/Interpreter.h>


using namespace App;
namespace sp = std::placeholders;

std::vector<DocumentObserverPython*> DocumentObserverPython::_instances;

void DocumentObserverPython::addObserver(const Py::Object& obj)
{
    _instances.push_back(new DocumentObserverPython(obj));
}

void DocumentObserverPython::removeObserver(const Py::Object& obj)
{
    DocumentObserverPython* obs = nullptr;
    for (std::vector<DocumentObserverPython*>::iterator it = _instances.begin();
         it != _instances.end();
         ++it) {
        if ((*it)->inst == obj) {
            obs = *it;
            _instances.erase(it);
            break;
        }
    }

    delete obs;
}

DocumentObserverPython::DocumentObserverPython(const Py::Object& obj)
    : inst(obj)
{
#define FC_PY_ELEMENT_ARG0(_name1, _name2)                                                         \
    do {                                                                                           \
        FC_PY_GetCallable(obj.ptr(), "slot" #_name1, py##_name1.py);                               \
        if (!py##_name1.py.isNone())                                                               \
            py##_name1.slot = App::GetApplication().signals->_name2.connect(                        \
                std::bind(&DocumentObserverPython::slot##_name1, this));                           \
    } while (0);


#define FC_PY_ELEMENT_ARG1(_name1, _name2)                                                         \
    do {                                                                                           \
        FC_PY_GetCallable(obj.ptr(), "slot" #_name1, py##_name1.py);                               \
        if (!py##_name1.py.isNone())                                                               \
            py##_name1.slot = App::GetApplication().signals->_name2.connect(                        \
                std::bind(&DocumentObserverPython::slot##_name1, this, sp::_1));                   \
    } while (0);

    // NOLINTBEGIN
#define FC_PY_ELEMENT_ARG2(_name1, _name2)                                                         \
    do {                                                                                           \
        FC_PY_GetCallable(obj.ptr(), "slot" #_name1, py##_name1.py);                               \
        if (!py##_name1.py.isNone())                                                               \
            py##_name1.slot = App::GetApplication().signals->_name2.connect(                        \
                std::bind(&DocumentObserverPython::slot##_name1, this, sp::_1, sp::_2));           \
    } while (0);

    FC_PY_ELEMENT_ARG1(CreatedDocument, newDocument)
    FC_PY_ELEMENT_ARG1(DeletedDocument, deleteDocument)
    FC_PY_ELEMENT_ARG1(RelabelDocument, relabelDocument)
    FC_PY_ELEMENT_ARG1(ActivateDocument, activeDocument)
    FC_PY_ELEMENT_ARG1(UndoDocument, undoDocument)
    FC_PY_ELEMENT_ARG1(RedoDocument, redoDocument)
    FC_PY_ELEMENT_ARG2(BeforeChangeDocument, beforeChangeDocument)
    FC_PY_ELEMENT_ARG2(ChangedDocument, changedDocument)
    FC_PY_ELEMENT_ARG1(CreatedObject, newObject)
    FC_PY_ELEMENT_ARG1(DeletedObject, deletedObject)
    FC_PY_ELEMENT_ARG2(BeforeChangeObject, beforeChangeObject)
    FC_PY_ELEMENT_ARG2(ChangedObject, changedObject)
    FC_PY_ELEMENT_ARG1(RecomputedObject, objectRecomputed)
    FC_PY_ELEMENT_ARG1(BeforeRecomputeDocument, beforeRecomputeDocument)
    FC_PY_ELEMENT_ARG1(RecomputedDocument, recomputed)
    FC_PY_ELEMENT_ARG2(OpenTransaction, openTransaction)
    FC_PY_ELEMENT_ARG1(CommitTransaction, commitTransaction)
    FC_PY_ELEMENT_ARG1(AbortTransaction, abortTransaction)
    FC_PY_ELEMENT_ARG0(Undo, undo)
    FC_PY_ELEMENT_ARG0(Redo, redo)
    FC_PY_ELEMENT_ARG1(BeforeCloseTransaction, beforeCloseTransaction)
    FC_PY_ELEMENT_ARG1(CloseTransaction, closeTransaction)
    FC_PY_ELEMENT_ARG2(StartSaveDocument, startSaveDocument)
    FC_PY_ELEMENT_ARG2(FinishSaveDocument, finishSaveDocument)
    FC_PY_ELEMENT_ARG1(AppendDynamicProperty, appendDynamicProperty)
    FC_PY_ELEMENT_ARG1(RemoveDynamicProperty, removeDynamicProperty)
    FC_PY_ELEMENT_ARG2(ChangePropertyEditor, changePropertyEditor)
    FC_PY_ELEMENT_ARG2(BeforeAddingDynamicExtension, beforeAddingDynamicExtension)
    FC_PY_ELEMENT_ARG2(AddedDynamicExtension, addedDynamicExtension)
    // NOLINTEND
}

DocumentObserverPython::~DocumentObserverPython() = default;

void DocumentObserverPython::slotCreatedDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyCreatedDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotDeletedDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyDeletedDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotRelabelDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyRelabelDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotActivateDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyActivateDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotUndoDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyUndoDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}


void DocumentObserverPython::slotRedoDocument(const App::Document& Doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        Base::pyCall(pyRedoDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotUndo()
{
    Base::PyGILStateLocker lock;
    try {
        Base::pyCall(pyUndo.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotRedo()
{
    Base::PyGILStateLocker lock;
    try {
        Base::pyCall(pyRedo.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotBeforeCloseTransaction(bool abort)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::Boolean(abort));
        Base::pyCall(pyBeforeCloseTransaction.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotCloseTransaction(bool abort)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::Boolean(abort));
        Base::pyCall(pyCloseTransaction.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotBeforeChangeDocument(const App::Document& Doc,
                                                      const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = Doc.getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyBeforeChangeDocument.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotChangedDocument(const App::Document& Doc,
                                                 const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(Doc).getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = Doc.getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyChangedDocument.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotCreatedObject(const App::DocumentObject& Obj)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::DocumentObject&>(Obj).getPyObject()));
        Base::pyCall(pyCreatedObject.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotDeletedObject(const App::DocumentObject& Obj)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::DocumentObject&>(Obj).getPyObject()));
        Base::pyCall(pyDeletedObject.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotBeforeChangeObject(const App::DocumentObject& Obj,
                                                    const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::DocumentObject&>(Obj).getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = Obj.getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyBeforeChangeObject.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotChangedObject(const App::DocumentObject& Obj,
                                               const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::DocumentObject&>(Obj).getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = Obj.getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyChangedObject.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotRecomputedObject(const App::DocumentObject& Obj)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::DocumentObject&>(Obj).getPyObject()));
        Base::pyCall(pyRecomputedObject.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotRecomputedDocument(const App::Document& doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        Base::pyCall(pyRecomputedDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotBeforeRecomputeDocument(const App::Document& doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        Base::pyCall(pyBeforeRecomputeDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotOpenTransaction(const App::Document& doc, std::string str)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        args.setItem(1, Py::String(str));
        Base::pyCall(pyOpenTransaction.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotCommitTransaction(const App::Document& doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        Base::pyCall(pyCommitTransaction.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotAbortTransaction(const App::Document& doc)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(1);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        Base::pyCall(pyAbortTransaction.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotAppendDynamicProperty(const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        auto container = Prop.getContainer();
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(container->getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = container->getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyAppendDynamicProperty.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotRemoveDynamicProperty(const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        auto container = Prop.getContainer();
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(container->getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = container->getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyRemoveDynamicProperty.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotChangePropertyEditor(const App::Document&,
                                                      const App::Property& Prop)
{
    Base::PyGILStateLocker lock;
    try {
        auto container = Prop.getContainer();
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(container->getPyObject()));
        // If a property is touched but not part of a document object then its name is null.
        // In this case the slot function must not be called.
        const char* prop_name = container->getPropertyName(&Prop);
        if (prop_name) {
            args.setItem(1, Py::String(prop_name));
            Base::pyCall(pyChangePropertyEditor.ptr(), args.ptr());
        }
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotStartSaveDocument(const App::Document& doc,
                                                   const std::string& file)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        args.setItem(1, Py::String(file));
        Base::pyCall(pyStartSaveDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotFinishSaveDocument(const App::Document& doc,
                                                    const std::string& file)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::Document&>(doc).getPyObject()));
        args.setItem(1, Py::String(file));
        Base::pyCall(pyFinishSaveDocument.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotBeforeAddingDynamicExtension(
    const App::ExtensionContainer& extcont,
    std::string extension)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::ExtensionContainer&>(extcont).getPyObject()));
        args.setItem(1, Py::String(extension));
        Base::pyCall(pyBeforeAddingDynamicExtension.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}

void DocumentObserverPython::slotAddedDynamicExtension(const App::ExtensionContainer& extcont,
                                                       std::string extension)
{
    Base::PyGILStateLocker lock;
    try {
        Py::Tuple args(2);
        args.setItem(0, Py::asObject(const_cast<App::ExtensionContainer&>(extcont).getPyObject()));
        args.setItem(1, Py::String(extension));
        Base::pyCall(pyAddedDynamicExtension.ptr(), args.ptr());
    }
    catch (Py::Exception&) {
        Base::PyException e;  // extract the Python error text
        e.ReportException();
    }
}
