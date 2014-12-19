#include <boost/python.hpp>
#include <boost/cstdint.hpp>

#include <string>
#include <iostream>
#include <assert.h>
#include "base.h"

// Using =======================================================================
using namespace boost::python;

#include "bbase.h"

static void translator(BadPositionException const *x) {
    PyErr_SetString(PyExc_UserWarning, "BadPosition");
}

void add_BadPositionException() {
    register_exception_translator<BadPositionException*>(&translator);
}
