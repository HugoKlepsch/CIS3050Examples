/**
 * A string management class
 *
 * $Id: NiceString.h 492 2012-04-02 14:49:41Z andrew $
 */


#ifndef		__SELF_REFERENCING_STRING_HANDLE_HEADER__
#define		__SELF_REFERENCING_STRING_HANDLE_HEADER__

#ifndef MAKDEPEND
# include <stdio.h>
# include <stdarg.h>
#endif

#include "CountedMemoryReference.h"

/**
CLASS
		NiceString

	A smart self-reffing string.  This class can be
	created as a local variable and treated as a
	string -- the class itself is a wrapper to an
	allocated array object which contains the actual
	data.

	<p/>
	Reference-counting is managed internally so that
	the actual char buffer is deallocated when the last
	reference to this object disappears; the strings
	can therefore safely be locally created and destroyed,
	without the need to new/delete or ref()/unref()
	them.  Essentially, objects of type NiceString
	can be treated as simple basetype objects (such as
	int, float, <i>etc</i>)as far as allocation,
	deletion and assignment are concerned.

	<p/>
	To format one of these objects, the sprintf() interface
	is provided.
*/
class NiceString
{
public:

	////////////////////////////////////////
	// Construct with an initial data value;
	// the <b>stringVal</b> argument is copied
	// and managed internally.
	NiceString(const char *stringVal);

	////////////////////////////////////////
	// Construct as an empty string
	NiceString();

	////////////////////////////////////////
	// Copy constructor -- internal string
	// storage will be common between the
	// two siblings
	NiceString(const NiceString &sibling);

	////////////////////////////////////////
	// Destructor.  Internal string is only
	// deleted if this is the last handle
	// to it.
	virtual ~NiceString();

	////////////////////////////////////////
	// Make this object share the internal
	// storage of <b>sibling</b>, managing
	// the reference count for these objects
	// intelligently.
	NiceString& operator= (const NiceString &sibling);

	////////////////////////////////////////
	// Equality
	friend int operator== (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Equality
	friend int operator== (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Inequality
	friend int operator!= (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Inequality
	friend int operator!= (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator<= (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator>= (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator< (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator> (
			const NiceString &s1,
			const NiceString &s2
		);


	////////////////////////////////////////
	// Ordinal comparison
	friend int operator<= (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator>= (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator< (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Ordinal comparison
	friend int operator> (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Concatenate to new string
	friend NiceString operator+ (
			const NiceString &s1,
			const NiceString &s2
		);

	////////////////////////////////////////
	// Concatenate to new string
	friend NiceString operator+ (
			const NiceString &s1,
			const char *s2
		);

	////////////////////////////////////////
	// Convert on the fly to a char *
	operator const char *() const;

	////////////////////////////////////////
	// Accessor for internal char * buffer
	char *getValue() const;

	////////////////////////////////////////
	// Return length of stored string
	int getLength() const;

	////////////////////////////////////////
	// Throw away the old internal representation
	// and set it to be a copy of <b>newValue</b>
	void setValue(const char *newValue);

	////////////////////////////////////////
	// Throw away the old internal representation
	// and set it to be shared with <b>newValue</b>
	void setValue(NiceString newValue);

	////////////////////////////////////////
	// Get a version of the contents
	// quoted and ready for inclusion
	// in an escape protected file.
	// <p>
	// There will be double quotes at the ends
	// of this new string, and all the following
	// characters will be preceeded by '\'
	// characters:
	// <ul>
	// <li>'\'
	// <li>"
	// <li>[TAB]
	// <li>[NEWLINE]
	// </ul>
	NiceString getQuoted() const;

	////////////////////////////////////////
	// Set the value from the sort of string
	// generated by the getQuoted() function.
	// <p>
	// The resultant internal represenation
	// will have deflated the escape sequences
	// introduced by getQuoted().
	void setQuoted(NiceString s);

	////////////////////////////////////////
	// Set the value from the sort of string
	// generated by the getQuoted() function
	// <p>
	// The resultant internal represenation
	// will have deflated the escape sequences
	// introduced by getQuoted().
	void setQuoted(const char *s);

	////////////////////////////////////////
	// return the first index of any of the
	// characters in <b>match</b>
	// or (-1) if no match
	int containsAny(const char *match) const;

	////////////////////////////////////////
	// return the index of the
	// substring in <b>match</b>
	// or (-1) if no match
	int contains(const char *match) const;

	////////////////////////////////////////
	// return the index of the
	// character in <b>match</b>
	// or (-1) if no match
	int contains(char match) const;

	////////////////////////////////////////
	// format interface.  The results of the
	// sprintf end up in the NiceString the
	// function is called on.  Space is internally
	// allocated for all arguments.
	// <p>
	// The major part of the format specifier
	// set for standard printf() are supported,
	// including:
	// <ul>
	// <li>width
	// <li>precision
	// <li>'*' operators
	// <li>The integer types d, i, o, u, x, X,
	//     character type c,
	//     floating types f, g, G, e, E,
	//     and string types s, and an extra string type 'S'
	//     for arguments of
	//     type NiceString *.
	// </ul>
	int sprintf(const char *formatString, ...);

	////////////////////////////////////////
	// format interface.  Used by sprintf()
	// above, as well as available for use
	// like vsprintf() etc in libc.
	int vsprintf(const char *formatString, va_list args);

protected:
		/** internal class not visible outside of this class */
	class intlString_ : public CountedMemoryReference
		{
				public:
				char *s;
				int sLen;

				intlString_(const char *data);
				intlString_(const char *data, int max);

				virtual ~intlString_();

				friend class NiceString;
		};

	// the object we are a handle to
	intlString_ *data;

public:
	////////////////////////////////////////
	// Statically declared string which is
	// guaranteed to be 'nul'.  This is
	// provided for assignment and comparison
	// purposes.
	static NiceString nul;

protected:
	////////////////////////////////////////
	// clean up static refs
	static void staticCleanup();

};

/*
 * Inline declarations for simple functions
 */
inline char *NiceString::getValue() const
{
	if (data != NULL)
		return data->s;
	return (char *) "<unknown>";
}

inline int NiceString::getLength() const
{
	if (data != NULL)
		return data->sLen;
	return 0;
}

inline void NiceString::setValue(NiceString s)
{
	*this = s;
}

inline NiceString::operator const char * () const
{
	return getValue();
}

#endif /* __SELF_REFERENCING_STRING_HANDLE_HEADER__ */

