/**
 * Implementation of string class
 *
 * $Id: NiceString-printf.cpp 492 2012-04-02 14:49:41Z andrew $
 */


#ifndef MAKEDEPEND
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <ctype.h>
#endif

#include "NiceString.h"
#include "growList.h"


#define			MAX_NUMBER_LENGTH	128

#define FORMAT_TYPE_NOTYPE			0x01
#define FORMAT_TYPE_FLOAT			0x02
#define FORMAT_TYPE_DECIMAL			0x03
#define FORMAT_TYPE_OCTAL			0x04
#define FORMAT_TYPE_HEX				0x05
#define FORMAT_TYPE_STRING			0x06
#define FORMAT_TYPE_CHAR			0x07
#define FORMAT_TYPE_POINTER			0x08
#define FORMAT_TYPE_PERCENT			0x09


#define		DTYPE_NOTYPE			0x01
#define DTYPE_INTEGER				0x02
#define DTYPE_UNSIGNED				0x03
#define DTYPE_DOUBLE				0x04
#define DTYPE_CHAR				0x05
#define DTYPE_STRING				0x06
#define DTYPE_TL_STRING				0x07
#define DTYPE_POINTER				0x08

// output flags to control pretty printing
#define OFLAG_CLEAR				0x0000
#define OFLAG_PAD_ZERO				0x0001
#define OFLAG_LEFT_JUST				0x0002
#define OFLAG_FORCE_SIGN			0x0004
#define OFLAG_SPACE_SIGN			0x0008
#define OFLAG_UPPERCASE				0x0010
#define OFLAG_EXPONENT				0x0020
#define OFLAG_VAREXP				0x0040
#define OFLAG_HAS_PREC				0x0080
#define OFLAG_USE_ALTERNATE			0x0100
#define OFLAG_WIDTH_LOADED			0x0200
#define OFLAG_PREC_LOADED			0x0400
#define OFLAG_BAD_FORMAT			0x8000

	// types of storage size specification
#define STFLAG_CLEAR				0x01
#define STFLAG_IS_LONG				0x02
#define STFLAG_IS_LONG_LONG			0x03
#define STFLAG_IS_SHORT				0x04


typedef struct SpecifierDescription
{
	int width;
	int precision;
	int frmtType;
	int dataType;
	int storageSpec;
	int outFlags;
} SpecifierDescription;

typedef struct OutputBuffer
{
	char *buffer;
	int length;
	int blocks;
} OutputBuffer;


/**
 * this function gets called right after we saw a '%' sign, so we now
 * have to figure out what the specifier is actually about
 *
 * Here we build up the SpecifierDescription specifier, from which we
 * can simply print out all the output info.
 */
static int
parseFormatSpecifier(
		const char *format,
		SpecifierDescription *specifier
	)
{
	int leaveLoop = 0;
	int sawDigit = 0;
	int lengthRead = 1;
	/** we will always convert at least one character **/


	/**
	 * Set flags for no data
	 */
	specifier->outFlags = OFLAG_CLEAR;
	specifier->width = 0;
	specifier->precision = 0;
	specifier->dataType = DTYPE_NOTYPE;
	specifier->frmtType = FORMAT_TYPE_NOTYPE;
	specifier->storageSpec = STFLAG_CLEAR;

	while (*format != '\0') {
		if (isascii(*format)) {
			/**
			 **	actual format specifiers
			 **/
			switch(*format) {
				case 'd':
				case 'i':
				specifier->frmtType = FORMAT_TYPE_DECIMAL;
				specifier->dataType = DTYPE_INTEGER;
				leaveLoop = 1;
				break;


				case 'o':
				specifier->frmtType = FORMAT_TYPE_OCTAL;
				specifier->dataType = DTYPE_UNSIGNED;
				leaveLoop = 1;
				break;

				case 'u':
				specifier->frmtType = FORMAT_TYPE_DECIMAL;
				specifier->dataType = DTYPE_UNSIGNED;
				leaveLoop = 1;
				break;

				case 'X':
				specifier->outFlags |= OFLAG_UPPERCASE;
				/** fall through **/
				case 'x':
				specifier->frmtType = FORMAT_TYPE_HEX;
				specifier->dataType = DTYPE_UNSIGNED;
				leaveLoop = 1;
				break;

				case 'f':
				specifier->frmtType = FORMAT_TYPE_FLOAT;
				specifier->dataType = DTYPE_DOUBLE;
				leaveLoop = 1;
				break;

				case 'E':
				specifier->outFlags |= OFLAG_UPPERCASE;
				/** fall through **/
				case 'e':
				specifier->frmtType = FORMAT_TYPE_FLOAT;
				specifier->dataType = DTYPE_DOUBLE;
				specifier->outFlags = OFLAG_EXPONENT;
				leaveLoop = 1;
				break;

				case 'G':
				specifier->outFlags |= OFLAG_UPPERCASE;
				/** fall through **/
				case 'g':
				specifier->frmtType = FORMAT_TYPE_FLOAT;
				specifier->dataType = DTYPE_DOUBLE;
				specifier->outFlags |= OFLAG_VAREXP;
				leaveLoop = 1;
				break;

				case 'c':
				specifier->frmtType = FORMAT_TYPE_CHAR;
				specifier->dataType = DTYPE_CHAR;
				leaveLoop = 1;
				break;

				case 's':
				specifier->frmtType = FORMAT_TYPE_STRING;
				specifier->dataType = DTYPE_STRING;
				leaveLoop = 1;
				break;

				case 'S':
				specifier->frmtType = FORMAT_TYPE_STRING;
				specifier->dataType = DTYPE_TL_STRING;
				leaveLoop = 1;
				break;

				case 'p':
				specifier->frmtType = FORMAT_TYPE_POINTER;
				specifier->dataType = DTYPE_POINTER;
				leaveLoop = 1;
				break;

				case '%':
				specifier->frmtType = FORMAT_TYPE_PERCENT;
				leaveLoop = 1;
				break;


				/**
				 **	size modifiers
				 **/
				case 'h':
				if (specifier->storageSpec != STFLAG_CLEAR)
					specifier->outFlags = OFLAG_BAD_FORMAT;
				specifier->storageSpec = STFLAG_IS_SHORT;
				break;

				case 'l':	/** and 'll'	*/
				if (specifier->storageSpec == STFLAG_IS_LONG)
					specifier->storageSpec = STFLAG_IS_LONG_LONG;
				else
					specifier->storageSpec = STFLAG_IS_LONG;
				break;


				/**
				 **	flag modifiers
				 **/
				case '-':
				specifier->outFlags |= OFLAG_LEFT_JUST;
				break;

				case '+':
				specifier->outFlags |= OFLAG_FORCE_SIGN;
				if (specifier->outFlags & OFLAG_SPACE_SIGN)
					specifier->outFlags &= ~(OFLAG_SPACE_SIGN);
				break;

				case ' ':
				if (!(specifier->outFlags & OFLAG_FORCE_SIGN))
					specifier->outFlags |= OFLAG_SPACE_SIGN;
				break;

				case '#':
				specifier->outFlags |= OFLAG_USE_ALTERNATE;
				break;

				case '0':
				if (sawDigit == 0)
					specifier->outFlags |= OFLAG_PAD_ZERO;
				else
					/** load up one of the integer based fields **/
					if (!(specifier->outFlags & OFLAG_HAS_PREC))
						specifier->width = (specifier->width * 10);
					else
						specifier->precision = (specifier->precision * 10);
				sawDigit = 1;
				break;


				case '.':
				specifier->outFlags |= OFLAG_HAS_PREC;
				break;

				case '*':	/** load sizes from stack **/
				if (!(specifier->outFlags & OFLAG_HAS_PREC))
					specifier->outFlags |= OFLAG_WIDTH_LOADED;
				else
					specifier->outFlags |= OFLAG_PREC_LOADED;
				break;


				/**
				 **	flag modifiers
				 **/
				default:
				/** we handle zeros above, but other digits here */
				if (isdigit(*format)) {
					sawDigit = 1;
					/** load up one of the integer based fields **/
					if (!(specifier->outFlags & OFLAG_HAS_PREC))
						specifier->width =
								(specifier->width * 10) + (*format - '0');
					else
						specifier->precision =
								(specifier->precision * 10) + (*format - '0');
				} else
					/** flag bad on through on unknown chars **/
					specifier->outFlags = OFLAG_BAD_FORMAT;

				break;
			}

			if (leaveLoop)
				break;

		} else {
			/**
			 * if we don't understand something,
			 * flag this format as bad
			 */
			specifier->outFlags = OFLAG_BAD_FORMAT;
		}
		format++;
		lengthRead++;
	}

	if (*format == '\0') {
		specifier->outFlags = OFLAG_BAD_FORMAT;
	}

	/** check that default precision is 6 for float **/
	if ((specifier->frmtType == FORMAT_TYPE_FLOAT) &&
				(!(specifier->outFlags & OFLAG_HAS_PREC))) {
		specifier->precision = 6;
	}

	return lengthRead;
}


static void
addCharToOutputBuffer(OutputBuffer *buf, char c)
{
	assert(growListToSize(
			buf->length + 2,
			(void **) &buf->buffer,
			&buf->blocks,
			64,
			sizeof(char)));
	buf->buffer[ buf->length++ ] = c;
	buf->buffer[ buf->length ] = 0;
}

static void
addRepeatCharToOutputBuffer(OutputBuffer *buf, char c, int n)
{
	int i;

	/** grow the buffer enough for the whole string we are adding */
	assert(growListToSize(
			buf->length + n + 1,
			(void **) &buf->buffer,
			&buf->blocks,
			64,
			sizeof(char)));

	/**
	 * add in n chars & update the length
	 */
	for (i = 0; i < n ; i++) {
		buf->buffer[ buf->length++] = c;
	}
	buf->buffer[ buf->length ] = 0;
}

static void
addStringToOutputBuffer(OutputBuffer *buf, const char *dataString)
{
	int length;
	
	length = strlen(dataString);

	/** grow the buffer enough for the whole dataString we are adding */
	assert(growListToSize(
			buf->length + length + 1,
			(void **) &buf->buffer,
			&buf->blocks,
			64,
			sizeof(char)));

	/** copy in the new dataString and update the length */
	memcpy(&buf->buffer[ buf->length ], dataString, length);
	buf->buffer[ buf->length + length ] = 0;
	buf->length += length;
}

// load from a local buffer into the string with padding
static void
loadFromBuffer(
		OutputBuffer *outputBufState,
		const char *localLoad,
		int nPrinted,
		SpecifierDescription *specifierDescription
	)
{
	int		nPad;
	int isPrinted = 0;

	/** add spaces to the buffer **/
	nPad = specifierDescription->width - nPrinted;
	if (nPad > 0) {
		if (specifierDescription->outFlags & OFLAG_LEFT_JUST) {
			addStringToOutputBuffer(outputBufState, localLoad);
			isPrinted = 1;
		}
		if (specifierDescription->outFlags & OFLAG_PAD_ZERO) {
			addRepeatCharToOutputBuffer(outputBufState, '0', nPad);
		} else {
			addRepeatCharToOutputBuffer(outputBufState, ' ', nPad);
		}
	}

	if (!isPrinted)
		addStringToOutputBuffer(outputBufState, localLoad);
}

static void
loadInteger(
		OutputBuffer *outputBufState,
		int iVal,
		SpecifierDescription *specifierDescription
	)
{
	int nPrinted = 0;
	int nPad;
	int isNeg = 0;
	char localBuf[MAX_NUMBER_LENGTH];
	char *localLoad;

	/** set up the temp buffer to load from the right **/
	localLoad = &localBuf[MAX_NUMBER_LENGTH - 1];
	*localLoad-- = 0;

	if (iVal < 0) {
		isNeg = 1;
		iVal *= (-1);
	}

	/** fill up the buffer **/
	while (iVal > 0) {
		*localLoad-- = (char) ('0' + (iVal % 10));
		iVal /= 10;
		nPrinted++;
	}

	/** add in precision **/
	if ((specifierDescription->outFlags & OFLAG_HAS_PREC) != 0) {
		if (nPrinted < specifierDescription->precision) {
			specifierDescription->precision -= nPrinted;
			while (specifierDescription->precision-- > 0) {
			*localLoad-- = '0';
			nPrinted++;
			}
		}
	}

	/** check for 0 value **/
	if (nPrinted == 0) {
		*localLoad-- = '0';
		nPrinted++;
	}

	/**
	 ** we have to determine whether we are padding with zeros
	 ** prior to committing to the sign
	 **/
	if (specifierDescription->outFlags & OFLAG_PAD_ZERO) {
		nPad = (specifierDescription->width - nPrinted) - 1;
		while (nPad-- > 0) {
			*localLoad-- = '0';
			nPrinted++;
		}
	}

	/** handle the sign **/
	if (isNeg) {
		*localLoad-- = '-';
		nPrinted++;

	} else if (specifierDescription->outFlags & OFLAG_FORCE_SIGN) {
		*localLoad-- = '+';
		nPrinted++;

	} else if (specifierDescription->outFlags & OFLAG_SPACE_SIGN) {
		*localLoad-- = ' ';
		nPrinted++;
	}

	/**
	 * we have backed up one too many chars, so pre-increment
	 * localLoad
	 */
	loadFromBuffer(
			outputBufState,
			++localLoad,
			nPrinted,
			specifierDescription
		);
}

static void
loadUnsigned(
		OutputBuffer *outputBufState,
		unsigned long uVal,
		SpecifierDescription *specifierDescription
	)
{
	int nPrinted = 0;
	int outBase;
	int alternateOk = 0;
	char localBuf[MAX_NUMBER_LENGTH];
	char *localLoad;
	const char *outChars;

	/** set up the temp buffer **/
	localLoad = &localBuf[MAX_NUMBER_LENGTH - 1];
	*localLoad-- = 0;

	/** figure out our base **/
	if (specifierDescription->frmtType == FORMAT_TYPE_HEX) {

		outBase = 16;
		alternateOk = 1;
		if (specifierDescription->outFlags & OFLAG_UPPERCASE)
			outChars = "0123456789ABCDEFX";
		else
			outChars = "0123456789abcdefx";

	} else if (specifierDescription->frmtType == FORMAT_TYPE_OCTAL) {
		alternateOk = 1;
		outBase = 8;
		outChars = "01234567";

	} else {
		outBase = 10;
		outChars = "0123456789";
	}
	

	/** fill up the buffer **/
	while (uVal > 0) {
		*localLoad-- = outChars[0 + (uVal % outBase)];
		uVal /= outBase;
		nPrinted++;
	}

	/** add in precision **/
	if (specifierDescription->outFlags & OFLAG_HAS_PREC) {
		if (nPrinted < specifierDescription->precision) {
			specifierDescription->precision -= nPrinted;
			while (specifierDescription->precision-- > 0) {
			*localLoad-- = '0';
			nPrinted++;
			}
		}
	}

	/** check for 0 value **/
	if (nPrinted == 0) {
		alternateOk = 0;
		*localLoad-- = '0';
		nPrinted++;
	}

	/** handle the leader if there **/
	if ((alternateOk) &&
				(specifierDescription->outFlags
			 		& OFLAG_USE_ALTERNATE)) {
		if (specifierDescription->frmtType == FORMAT_TYPE_HEX) {
			*localLoad-- = outChars[outBase];
			nPrinted++;
		}
		*localLoad-- = '0';
		nPrinted++;
	}

	loadFromBuffer(
			outputBufState,
			++localLoad,
			nPrinted,
			specifierDescription
		);
}

static void
loadDouble(
		OutputBuffer *outputBufState,
		double dVal,
		SpecifierDescription *specifierDescription
	)
{
	int nPrinted = 0;
	int nPad;
	int isNeg = 0;
	const char *fmt;
	char localFormatBuf[MAX_NUMBER_LENGTH];
	char localPadBuf[2 * MAX_NUMBER_LENGTH];
	char *localLoad;

	if (dVal < 0.0) {
		isNeg = 1;
		dVal *= (-1);
	}

	/**
	 ** set up the temp buffer -- we may as well not figure out this
	 **		horrible thing all over again!
	 **/
	if (specifierDescription->outFlags & OFLAG_EXPONENT) {
		fmt = "%.*e";

	} else if (specifierDescription->outFlags & OFLAG_VAREXP) {
		fmt = "%.*g";

	} else {
		fmt = "%.*f";
	}

	/** first print it based on type */
	sprintf(localFormatBuf, fmt,
				specifierDescription->precision, dVal);

	/**
	 * now move to the right-most location in the buffer
	 * so that we can pad it
	 */
	nPrinted = strlen(localFormatBuf);
	localLoad = &localPadBuf[(2 * MAX_NUMBER_LENGTH) - (nPrinted + 1)];
	strcpy(localLoad, localFormatBuf);
	localLoad--;


	/**
	 * we have to determine whether we are padding with zeros
	 * prior to committing to the sign
	 */
	if (specifierDescription->outFlags & OFLAG_PAD_ZERO) {
		nPad = (specifierDescription->width - nPrinted) - 1;
		while (nPad-- > 0) {
			*localLoad-- = '0';
			nPrinted++;
		}
	}

	/** handle the sign **/
	if (isNeg) {
		*localLoad-- = '-';
		nPrinted++;

	} else if (specifierDescription->outFlags & OFLAG_FORCE_SIGN) {
		*localLoad-- = '+';
		nPrinted++;

	} else if (specifierDescription->outFlags & OFLAG_SPACE_SIGN) {
		*localLoad-- = ' ';
		nPrinted++;
	}

	loadFromBuffer(
			outputBufState,
			++localLoad,
			nPrinted,
			specifierDescription
		);
}

static void
loadChar(
		OutputBuffer *outputBufState,
		char c,
		SpecifierDescription *specifierDescription
	)
{
	int		isPrinted = 0;

	if (specifierDescription->width != 0) {
		if (specifierDescription->outFlags & OFLAG_LEFT_JUST) {
			addCharToOutputBuffer(outputBufState, c);
			isPrinted = 1;
		}
		/** print out specified width field **/
		addRepeatCharToOutputBuffer(
			outputBufState, ' ',
			specifierDescription->width - 1
			);
	}
	if (!isPrinted)
		addCharToOutputBuffer(outputBufState, c);
}


static void
loadString(
		OutputBuffer *outputBufState,
		const char *strVal,
		SpecifierDescription *specifierDescription
	)
{
	char *dupVal;
	int length;

	length = strlen(strVal);

	/** make sure that we are not padding with zeros **/
	specifierDescription->outFlags &= ~(OFLAG_PAD_ZERO);

	/** see whether we have to limit the precision **/
	if ((specifierDescription->outFlags & OFLAG_HAS_PREC) &&
			(specifierDescription->precision < length)) {
		dupVal = strdup(strVal);

		dupVal[specifierDescription->precision] = '\0';

		loadFromBuffer(outputBufState, dupVal,
				specifierDescription->precision,
				specifierDescription);
		free(dupVal);

	} else {
		loadFromBuffer(
			outputBufState,
			strVal,
			length,
			specifierDescription
			);
	}
}

static void loadBad(OutputBuffer *outputBufState)
{
	addStringToOutputBuffer(outputBufState, "<BAD>");
}

/**
 * print into this string using a "printf()" like format,
 * growing the buffer as we proceed.
 */
int NiceString::vsprintf(const char *formatString, va_list args)
{
	OutputBuffer outputBufState;
	SpecifierDescription specifierDescription;
	int		specifierLength;
	int nConverted = 0;

	/** clear the output buffer */
	memset(&outputBufState, 0, sizeof(OutputBuffer));

	while (*formatString != '\0') {
		if (*formatString != '%') {
			addCharToOutputBuffer(&outputBufState, *formatString++);
		} else {

			/** read the formatString specifier **/
			specifierLength = parseFormatSpecifier(
				++formatString, &specifierDescription
				);

			if (specifierDescription.outFlags == OFLAG_BAD_FORMAT) {
			loadBad(&outputBufState);
			return (nConverted);
			}

			/** handle '*' in width spec **/
			if (specifierDescription.outFlags & OFLAG_WIDTH_LOADED) {
			specifierDescription.width = va_arg(args, int);
			}

			/** handle '*' in precision spec **/
			if (specifierDescription.outFlags & OFLAG_PREC_LOADED) {
			specifierDescription.precision = va_arg(args, int);
			}


			/** determine what data type to load for printing **/
			if (specifierDescription.frmtType == FORMAT_TYPE_PERCENT) {
			loadChar(&outputBufState, '%', &specifierDescription);


			} else switch (specifierDescription.dataType) {
			case	DTYPE_INTEGER:
			{
				int iVal = va_arg(args, int);
				loadInteger(&outputBufState,
						iVal, &specifierDescription);
			}
			break;

			case	DTYPE_UNSIGNED:
			{
				unsigned uVal = va_arg(args, int);
				loadUnsigned(&outputBufState,
						uVal, &specifierDescription);
			}
			break;

			case	DTYPE_DOUBLE:
			{
				double dVal = va_arg(args, double);
				loadDouble(&outputBufState,
						dVal, &specifierDescription);
			}
			break;

			case	DTYPE_CHAR:
			{
				char cVal = (char) va_arg(args, int);
				loadChar(&outputBufState,
						cVal, &specifierDescription);
			}
			break;

			case	DTYPE_STRING:
			{
				/** get a char * off of the stack **/
				const char *strVal;
				strVal = va_arg(args, char *);
				if (strVal == NULL)
				strVal = "(nul)";
				loadString(&outputBufState,
						strVal, &specifierDescription);
			}
			break;

			case	DTYPE_TL_STRING:
			{
				const char *strVal;
				NiceString *tlStrVal;

				/** get a NiceString * off of the stack **/
				tlStrVal = va_arg(args, NiceString *);
				if (tlStrVal == NULL) {
				strVal = "(nul)";
				} else {
				strVal = tlStrVal->getValue();
				if (strVal == NULL) {
					strVal = "(nul-tstring)";
				}
				}
				loadString(&outputBufState,
						strVal, &specifierDescription);
			}
			break;

			case	DTYPE_POINTER:
			{
				void *pVal = va_arg(args, void *);
				specifierDescription.frmtType = FORMAT_TYPE_HEX;
				specifierDescription.outFlags |= OFLAG_PAD_ZERO;
				if (specifierDescription.width < 8) {
					specifierDescription.width = 8;
				}
				loadUnsigned(&outputBufState,
						(unsigned long) pVal,
						&specifierDescription);
			}
			break;
			}

			formatString += specifierLength;
			nConverted++;
		}
	}

	/** save in _our_ internal buffer, free buffer from OutputBuffer **/
	setValue( outputBufState.buffer );
	free(outputBufState.buffer);
	memset(&outputBufState, 0, sizeof(OutputBuffer));

	return (nConverted);
}

int NiceString::sprintf(const char *formatString, ...)
{
	va_list args;
	int retval;

	va_start(args, formatString);
	retval = NiceString::vsprintf(formatString, args);
	va_end(args);

	return(retval);
}

