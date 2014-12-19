// SinkWorld ConfigurationException class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// This exception is thrown when a lexer is misconfigured.

#ifdef SW_CXX
class ConfigurationException {
};
#endif

#ifdef SW_CS
class ConfigurationException : System.Exception {
};
#endif

#ifdef SW_JAVA
public class ConfigurationException extends RuntimeException {
};
#endif
