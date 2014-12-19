#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

std::string literal_as_str(Literal const& self) {
	int lenUTF8 = self.LengthUTF8();
	SW_BYTE *bytes = self.AsUTF8();
	std::string rv((char *)bytes, lenUTF8);
	delete []bytes;
	return rv;
}

void add_Literal() {
    class_< Literal >("Literal", init<  >())
        .def(init< Literal*, int, int >())
        .def(init< Literal* >())
        .def(init< Literal*, int, Literal* >())
        .def(init< const char* >())
        .def(init< const Literal& >())
        .def(init< int >())
        .def("Value", &Literal::Value)
        .def("CharAt", &Literal::CharAt)
        .def("Length", &Literal::Length)
        //~ .def("AsUTF8", &Literal::AsUTF8)
        .def("LengthUTF8", &Literal::LengthUTF8)
        .def("Count", &Literal::Count)
        .def("Equals", &Literal::Equals)
        .def("Match", &Literal::Match)
        .def("EndsWith", &Literal::EndsWith)
        .def("Find", &Literal::Find)
        .def("IsIn", &Literal::IsIn)
        .def("Segment", &Literal::Segment, return_value_policy< manage_new_object >())
        .def("HashString", &Literal::HashString)
        .def("HashValue", &Literal::HashValue)
        .staticmethod("HashString")
	// Non-automatic
	.def("__str__", literal_as_str)
	.def("__unicode__", &Literal::AsWString)
    ;

}
