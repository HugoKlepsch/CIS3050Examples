/**
 * A reference-counting class
 *
 * $Id: CountedMemoryReference.h 492 2012-04-02 14:49:41Z andrew $
 */


#ifndef		__MEMORY_REFERENCE_COUNTING_HEADER__
#define		__MEMORY_REFERENCE_COUNTING_HEADER__

#include <assert.h>

/**
CLASS
		CountedMemoryReference

	Base ref-counting class for memory management.
	All child classes should have protected
	desctructors, which will only be called
	from unref here.

	If there are no circular dependencies based
	on references, then this will manage memory
	for us, based on the life of the object.
*/
class CountedMemoryReference
{
protected:
	int referenceCount;
	int wasTouched;

public:
	////////////////////////////////////////
	// Constructor
	CountedMemoryReference();

protected:
	////////////////////////////////////////
	// Protected destructor, called only
	// from unref()
	virtual ~CountedMemoryReference();

	////////////////////////////////////////
	// Return the typename for
	// reference debugging
	virtual char *clsId() const;

public:
	////////////////////////////////////////
	// Called in order to release a reference to the
	// object is made
	void ref();

	////////////////////////////////////////
	// Called every time a reference to an
	// object is released.
	void unref();
};

inline void
CountedMemoryReference::ref()
{
	referenceCount++;
	if ( ! wasTouched )
		wasTouched = 1;
}

inline void
CountedMemoryReference::unref()
{
	/* unref occurring on object which was never reffed */
	assert(wasTouched == 1);

	if (--referenceCount == 0) {
		delete this;
	}
}


#endif /* __MEMORY_REFERENCE_COUNTING_HEADER__ */

