#ifndef BASE_FILEEXCEPTION_H
#define BASE_FILEEXCEPTION_H

#include "Exception.h"
#include "FileInfo.h"


#ifdef _MSC_VER
#define THROWMF_FILEEXCEPTION(message, filenameorfileinfo)                                         \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __FUNCSIG__);                               \
        throw myexcp;                                                                              \
    }
#define THROWMFT_FILEEXCEPTION(message, filenameorfileinfo)                                        \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __FUNCSIG__);                               \
        myexcp.setTranslatable(true);                                                              \
        throw myexcp;                                                                              \
    }
#elif defined(__GNUC__)
#define THROWMF_FILEEXCEPTION(message, filenameorfileinfo)                                         \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __PRETTY_FUNCTION__);                       \
        throw myexcp;                                                                              \
    }
#define THROWMFT_FILEEXCEPTION(message, filenameorfileinfo)                                        \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __PRETTY_FUNCTION__);                       \
        myexcp.setTranslatable(true);                                                              \
        throw myexcp;                                                                              \
    }
#else
#define THROWMF_FILEEXCEPTION(message, filenameorfileinfo)                                         \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __func__);                                  \
        throw myexcp;                                                                              \
    }
#define THROWMFT_FILEEXCEPTION(message, filenameorfileinfo)                                        \
    {                                                                                              \
        FileException myexcp(message, filenameorfileinfo);                                         \
        myexcp.setDebugInformation(__FILE__, __LINE__, __func__);                                  \
        myexcp.setTranslatable(true);                                                              \
        throw myexcp;                                                                              \
    }
#endif


namespace Base {

/** File exception handling class
 * This class is specialized to go with exception thrown in case of File IO Problems.
 * @author Juergen Riegel
 */
class BaseExport FileException: public Exception
{
public:
    /// With massage and file name
    explicit FileException(const char* sMessage, const char* sFileName = nullptr);
    /// With massage and file name
    FileException(const char* sMessage, const FileInfo& File);
    /// standard construction
    FileException();
    FileException(const FileException&) = default;
    FileException(FileException&&) = default;
    /// Destruction
    ~FileException() noexcept override = default;
    /// Assignment operator
    FileException& operator=(const FileException&) = default;
    FileException& operator=(FileException&&) = default;

    /// Description of the exception
    const char* what() const noexcept override;
    /// Report generation
    void ReportException() const override;
    /// Get file name for use with translatable message
    std::string getFileName() const;
    /// returns a Python dictionary containing the exception data
    PyObject* getPyObject() override;
    /// returns sets the exception data from a Python dictionary
    void setPyObject(PyObject* pydict) override;

    PyObject* getPyExceptionType() const override;

protected:
    FileInfo file;
    // necessary   for what() legacy behaviour as it returns a buffer that
    // can not be of a temporary object to be destroyed at end of what()
    std::string _sErrMsgAndFileName;
    void setFileName(const char* sFileName = nullptr);
};

}  // namespace Base

#endif
