/**
 * Implementation of string class
 *
 * $Id: NiceString.cpp 492 2012-04-02 14:49:41Z andrew $
 */


#ifndef MAKEDEPEND
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <ctype.h>
#endif

#include "NiceString.h"
#include "growList.h"

/** access for static global "nul" string */
NiceString NiceString::nul("");

void
NiceString::staticCleanup()
{
	NiceString::nul.data->unref();
	NiceString::nul.data = NULL;
}


/**
 * Constructor for internal ref-counted storage block
 */
NiceString::intlString_::intlString_(const char *value)
{
	s = strdup(value);
	sLen = strlen(value);
}

/**
 * Destructor for internal ref-counted storage block
 */
NiceString::intlString_::~intlString_()
{
	if (s != NULL)
		free(s);
}


/*
 * Incorporate the char * as a new managed value
 */
NiceString::NiceString(const char *value)
{
	data = new intlString_(value);

	/** check against out of memory */
	assert(data != NULL);
	data->ref();
}

/* add a reference to existing internal storage */
NiceString::NiceString(const NiceString &sibling)
{
	if (sibling.data != NULL)
		++sibling.data->referenceCount;
	data = sibling.data;
}

/* add a reference to existing internal storage of NUL */
NiceString::NiceString()
{
	if (nul.data != NULL)
		++nul.data->referenceCount;
	data = nul.data;
}

/* knock one off the internal storage reference */
NiceString::~NiceString()
{
	if (data != NULL) {
		data->unref();
	}
}

NiceString &
NiceString::operator= (const NiceString &sibling)
{
	if (sibling.data != NULL)
		++sibling.data->referenceCount;

	if (data != NULL) {
		data->unref();
	}
	data = sibling.data;

	return *this;
}

int
operator== (
		const NiceString &s1,
		const NiceString &s2
	)
{
	if (s1.data == s2.data)
		return 1;
	return !strcmp(s1.data->s, s2.data->s);
}

int
operator!= (
		const NiceString &s1,
		const NiceString &s2
	)
{
	if (s1.data == s2.data)
		return 0;
	return strcmp(s1.data->s, s2.data->s);
}

int
operator>= (
		const NiceString &s1,
		const NiceString &s2
	)
{
	int result;
	if (s1.data == s2.data)
		return 1;
	result = strcmp(s1.data->s, s2.data->s);
	if (result >= 0)
		return 1;
	return 0;
}

int
operator<= (
		const NiceString &s1,
		const NiceString &s2
	)
{
	int result;
	if (s1.data == s2.data)
		return 1;
	result = strcmp(s1.data->s, s2.data->s);
	if (result <= 0)
		return 1;
	return 0;
}

int
operator> (
		const NiceString &s1,
		const NiceString &s2
	)
{
	int result;
	if (s1.data == s2.data)
		return 0;
	result = strcmp(s1.data->s, s2.data->s);
	if (result > 0)
		return 1;
	return 0;
}

int
operator< (
		const NiceString &s1,
		const NiceString &s2
	)
{
	int result;
	if (s1.data == s2.data)
		return 0;
	result = strcmp(s1.data->s, s2.data->s);
	if (result < 0)
		return 1;
	return 0;
}

NiceString
operator+ (
		const NiceString &s1,
		const NiceString &s2
	)
{
	NiceString result;
	result.sprintf("%s%s", s1.getValue(), s2.getValue());
	return result;
}

int
operator== (
		const NiceString &s1,
		const char *s2
	)
{
	return !strcmp(s1.data->s, s2);
}

int
operator!= (
		const NiceString &s1,
		const char *s2
	)
{
	return strcmp(s1.data->s, s2);
}

int
operator>= (
		const NiceString &s1,
		const char *s2
	)
{
	int result;
	result = strcmp(s1.data->s, s2);
	if (result >= 0)
		return 1;
	return 0;
}

int
operator<= (
		const NiceString &s1,
		const char *s2
	)
{
	int result;
	result = strcmp(s1.data->s, s2);
	if (result <= 0)
		return 1;
	return 0;
}

int
operator> (
		const NiceString &s1,
		const char *s2
	)
{
	int result;
	result = strcmp(s1.data->s, s2);
	if (result > 0)
		return 1;
	return 0;
}

int
operator< (
		const NiceString &s1,
		const char *s2
	)
{
	int result;
	result = strcmp(s1.data->s, s2);
	if (result < 0)
		return 1;
	return 0;
}

NiceString
operator+ (
		const NiceString &s1,
		const char *s2
	)
{
	NiceString result;
	result.sprintf("%s%s", s1.getValue(), s2);
	return result;
}

/*
 * create a new internal storage atom to hold this new
 * value; ensure that counting is appropriate for old
 * and new storage
 */
void
NiceString::setValue(const char *value)
{
	if (data != NULL) {
		data->unref();
	}

	data = new intlString_(value);

	/** out of memory */
	assert(data != NULL);

	if (data != NULL) {
		data->ref();
	}
}

void NiceString::setQuoted(const char *value)
{
	char *tmp;
	int len;
	int tmpIndex;
	int start, end;
	int i, ch;

	len = strlen(value);

	tmpIndex = 0;
	tmp = (char *) malloc(len + 1);

	/** out of memory */
	assert(tmp != NULL);

	if (value[0] == '"' && value[len-1] == '"') {
		start = 1;
		end = len-1;
	} else {
		start = 0;
		end = len;
	}

	for (i = start; i < end; i++) {
		ch = value[i++];
		if (ch == '\\') {
			if (isdigit(ch)) {

			int k = 0;
			int octalVal = (ch - '0');

			while (k++ < 3) {
				ch = value[i++];
				if ( ! isdigit(ch)) {
				break;
				}
				octalVal = (octalVal * 8 ) + (ch - '0');
			}
			tmp[tmpIndex++] = octalVal;

			} else {
			switch (ch) {
				case 'n':  tmp[tmpIndex++] = '\n'; break;
				case 't':  tmp[tmpIndex++] = '\t'; break;
				case 'b':  tmp[tmpIndex++] = '\b'; break;
				case 'r':  tmp[tmpIndex++] = '\r'; break;
				case 'f':  tmp[tmpIndex++] = '\f'; break;

				/** handle end of string semi-gracefully */
				case 0: 
				   tmp[tmpIndex++] = 0;
				   i--;
				   break;

				default: tmp[tmpIndex++] = ch;
			}
			}
		} else {
			tmp[tmpIndex++] = ch;
		}
	}
	tmp[tmpIndex++] = 0;

	setValue(tmp);
	free(tmp);
}

void NiceString::setQuoted(NiceString value)
{
	NiceString::setQuoted(value.getValue());
}

NiceString NiceString::getQuoted() const
{
	NiceString r;
	char *tmp;
	int i, j;

	tmp = (char *) malloc( (getLength() * 2) + 3);

	/** out of memory */
	assert(tmp != NULL);

	j = 0;
	tmp[j++] = '"';
	for (i = 0; i < getLength(); i++) {
		switch(data->s[i]) {
		case '\n':
			tmp[j++] = '\\';
			tmp[j++] = 'n';
			break;

		case '\t':
			tmp[j++] = '\\';
			tmp[j++] = 't';
			break;

		case '\\':
			tmp[j++] = '\\';
			tmp[j++] = '\\';
			break;

		case '"':
			tmp[j++] = '\\';
			tmp[j++] = '"';
			break;

		default:
			tmp[j++] = data->s[i];
		}
	}
	tmp[j++] = '"';
	tmp[j] = 0;

	r.setValue(tmp);
	free(tmp);

	return r;
}

int NiceString::contains(const char *substring) const
{
	const char *m;

	if (data == NULL)
		return -1;
	m = strstr(data->s, substring);
	if (m == NULL)
		return -1;

	return (int) (m - data->s);
}

int NiceString::contains(char match) const
{
	const char *m;

	if (data == NULL)
		return -1;
	m = strchr(data->s, match);
	if (m == NULL)
		return -1;

	return (int) (m - data->s);
}

int NiceString::containsAny(const char *match) const
{
	const char *m, *matchChar;

	if (data == NULL)
		return -1;
	matchChar = match;
	while (*matchChar != 0) {
		m = strchr(data->s, *matchChar);
		if (m != NULL)
			return (int) (m - data->s);
		matchChar++;
	}

	return -1;
}


