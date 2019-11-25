
Self-Referencing String Class
-----------------------------

This is a string class based on one I have used in several projects
over the last few years.

It uses a reference counting mechanism for its memory management,
to allowa an allocated buffer containing the string data to be passed
around and shared between instances of the wrapper class.

This magic is handled largely by the '=' operator, the constructors
and the destructor.

    memRef.h: a reference-counting base class, including inline
		implementation of the ref() and unref() functions which
		handle the actual counting.  Note the protected virtual
		destructor.

    tlRef.cpp: reference counter body


    srString.h: header file for the self-referencing string class,
		including inline implementation of several functions.  This
		class defines an internal class that holds the string data;
		the named srString class manages instances of this internal
		class by ensuring that ref()/unref() are called appropriately

    srString.cpp: body for this string class


	growList.h: header file for the buffer reallocation algorithm
		used from srString::printf().  Note that this algorithm is
		written in C, not C++.

	growList.c: C language implementation of buffer growth

