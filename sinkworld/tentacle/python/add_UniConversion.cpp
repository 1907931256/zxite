#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_UniConversion() {
    scope* UniConversion_scope = new scope(
    class_< UniConversion, boost::noncopyable >("UniConversion", no_init)
        .def("UTF8ByteLength", &UniConversion::UTF8ByteLength)
        .def("UTF8Validate", &UniConversion::UTF8Validate)
        .def("UTF32Validate", &UniConversion::UTF32Validate)
        .def("UTF8LengthFromUTF16Char", &UniConversion::UTF8LengthFromUTF16Char)
        .def("UTF8LengthFromUTF16", &UniConversion::UTF8LengthFromUTF16)
        .def("UTF8FromUTF16", &UniConversion::UTF8FromUTF16)
        .def("UTF16LengthFromUTF8", &UniConversion::UTF16LengthFromUTF8)
        .def("UTF16FromUTF8", &UniConversion::UTF16FromUTF8)
        .def("UTF8LengthFromUTF32Char", &UniConversion::UTF8LengthFromUTF32Char)
        .def("UTF8LengthFromUTF32", &UniConversion::UTF8LengthFromUTF32)
        .def("UTF8FromUTF32", &UniConversion::UTF8FromUTF32)
        .def("UTF32LengthFromUTF8", &UniConversion::UTF32LengthFromUTF8)
        .def("UTF32FromUTF8", &UniConversion::UTF32FromUTF8)
        .def("UTF16LengthFromUTF32Char", &UniConversion::UTF16LengthFromUTF32Char)
        .def("UTF16LengthFromUTF32", &UniConversion::UTF16LengthFromUTF32)
        .def("UTF16FromUTF32", &UniConversion::UTF16FromUTF32)
        .def("UTF32LengthFromUTF16", &UniConversion::UTF32LengthFromUTF16)
        .def("UTF32FromUTF16", &UniConversion::UTF32FromUTF16)
        .staticmethod("UTF16LengthFromUTF8")
        .staticmethod("UTF32FromUTF8")
        .staticmethod("UTF32LengthFromUTF8")
        .staticmethod("UTF16FromUTF32")
        .staticmethod("UTF8FromUTF16")
        .staticmethod("UTF8LengthFromUTF16Char")
        .staticmethod("UTF8LengthFromUTF32Char")
        .staticmethod("UTF32LengthFromUTF16")
        .staticmethod("UTF8LengthFromUTF32")
        .staticmethod("UTF16LengthFromUTF32Char")
        .staticmethod("UTF8Validate")
        .staticmethod("UTF16FromUTF8")
        .staticmethod("UTF32FromUTF16")
        .staticmethod("UTF16LengthFromUTF32")
        .staticmethod("UTF8ByteLength")
        .staticmethod("UTF8LengthFromUTF16")
        .staticmethod("UTF8FromUTF32")
        .staticmethod("UTF32Validate")
    );

    scope().attr("SURROGATE_LEAD_FIRST") = (int)UniConversion::SURROGATE_LEAD_FIRST;

    scope().attr("SURROGATE_TRAIL_FIRST") = (int)UniConversion::SURROGATE_TRAIL_FIRST;

    scope().attr("SURROGATE_TRAIL_LAST") = (int)UniConversion::SURROGATE_TRAIL_LAST;

    scope().attr("UNICODE_LAST") = (int)UniConversion::UNICODE_LAST;

    delete UniConversion_scope;

}
