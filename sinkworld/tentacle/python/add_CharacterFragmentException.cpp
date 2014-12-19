#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

static void translator(CharacterFragmentException const *x) {
    PyErr_SetString(PyExc_UserWarning, "CharacterFragmentException");
}

void add_CharacterFragmentException() {
    register_exception_translator<CharacterFragmentException*>(&translator);
}
