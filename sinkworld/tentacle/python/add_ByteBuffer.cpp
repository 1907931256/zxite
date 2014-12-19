#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

void add_ByteBuffer() {
    class_< ByteBuffer, boost::noncopyable >("ByteBuffer", init<  >())
        .def(init< int >())
        .def("Resize", &ByteBuffer::Resize)
        .def("Length", &ByteBuffer::Length)
        //~ .def("Set", &ByteBuffer::Set)
        //~ .def("to_unsigned_char", &ByteBuffer::operator unsigned char*)
    ;

}
