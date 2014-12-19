// SinkWorld CharacterFragmentException class.
// Copyright 2005 Neil Hodgson
// File written in C~ subset of C++ for automatic translation to Java and C#.

/// This exception is thrown when access to buffers occurs that would split up the
/// bytes of a character or retrieve partial characters.
/// For example in UTF-8, a character may be encoded as \xe6\x9b\xa8 with the bits
/// in the first byte indicating that there are two treiling bytes. Inserting the value
/// '\x40' between the '\xe6' and 'x9b' would break this character and make the
/// buffer invalid.

#ifdef SW_CXX
class CharacterFragmentException {
};
#endif

#ifdef SW_CS
class CharacterFragmentException : System.Exception {
};
#endif

#ifdef SW_JAVA
public class CharacterFragmentException extends RuntimeException {
};
#endif
