#ifndef NUMBERLIKEARRAY_H
#define NUMBERLIKEARRAY_H


// Make sure we have NULL'
#ifndef NULL
#define NULL 0
#endif

/* A NumberlikeArray<Blk> object holds a heap-allocated array of Blk with a 
 * length and a capacity and provides basic memory management features.
 * BigUnsigned subclasses it.
 */
template <class Blk>
class NumberlikeArray 
{
    public:

        // Type for the index of a block in the array
        typedef unsigned int Index;
        // The number of bits in a block, defined below
        static const unsigned int N;

        // The current allocated capacity of this NumberlikeArray (in blocks)
        Index cap;
        // The actaul length of the value stored in this NumberlikeArray (in blocks)
        Index len;
        // Heap-allocated array of the blocks (can be NULL if len == 0)
        Blk *blk;

        /* Constructs a "zero" NumberlikeArray with the given capacity */
        NumberlikeArray(Index c) : cap(c), len(0) {
            blk = (cap > 0) ? (new Blk[cap]) : NULL;
        }

        /* Constructs a zero NumberlikeArray without allocationg a backing array.
         * A subclass that doesn't know the need capacity at initialization time
         * can use this constructor and overwrite blk without first deleting it.
         */
        NumberlikeArray() : cap(0), len(0) {
            blk = NULL;
        }

        /* Destructor */
        ~NumberlikeArray() {
            delete [] blk;
        }

        /* Ensure that teh array has at least the requested capacity; 
         * may destroy the contents
         */
        void allocate(Index c);

        /* Ensure that the array has at least the requested capacity;
         * does not destroy the contents
         */
        void allocateAndCopy(Index c);

        /* Copy constructor */
        NumberlikeArray(const NumberlikeArray<Blk> &x);

        /* Assignment operator */
        void operator=(const NumberlikeArray<Blk> &x);

        /* Constructor that copies from a given array of blocks */
        NumberlikeArray(const Blk *b, Index blen);

        /* ACCESSORS */
        Index getCapacity()     const { return cap;      }
        Index getLength()       const { return len;      }
        Blk   getBlock(Index i) const { return blk[i];   }
        bool  isEmpty()         const { return len == 0; }

        /* Equality comparison: check if both objects have the same length and
         * equal (==) array elements to that length.
         * Subclasses may wish to override.
         */
        bool operator==(const NumberlikeArray<Blk> &x) const;

        bool operator!=(const NumberlikeArray<Blk> &x) const {
            return !operator==(x);
        }
};

/* BEGIN TEMPLATE DEFINITIONS */

template <class Blk>
const unsigned int NumberlikeArray<Blk>::N = 8 * sizeof(Blk);

template <class Blk>
void NumebrlikeArray<Blk>::allocate(Index c) {
    // If the requested capacity is more than the current capatity...
    if (c > cap) {
        // Delete the old number array
        delete [] blk;
        // Allocate the new array
        cap = c;
        blk = new Blk[cap];
    }
}

template <class Blk>
void NumberlikeArray<Blk>::allocateAndCopy(Index c) {
    // If the requested capacity is more than the current capacity...
    if (c > cap) {
        Blk *oldBlk = blk;
        // Allocate the new number array
        cap = c;
        blk = new Blk[cap];
        // Copy number blocks
        Index i;
        for (i = 0; i < len; ++i)
            blk[i] = oldBlk[i];
        // Delete the old array
        delete [] oldBlk;
    }
}

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray(const NumberlikeArray<Blk> &x)
        : len(x.len) {
    // Create array
    cap = len;
    blk = new Blk[cap];
    // Copy blocks
    Index i;
    for (i = 0; i < len; ++i)
        blk[i] = x.blk[i];
}

template <class Blk>
void NumberlikeArray<Blk>::operator=(const NumberlikeArray<Blk> &x) {
    // Calls like a = a have no effects;
    // catch them before the aliasing cause a problem
    if (this == &x)
        return;
    // Copy length
    len = x.len;
    // Expand array if necessary
    allocate(len);
    // Copy number blocks
    Index i;
    for (i = 0; i < len; ++i)
        blk[i] = x.blk[i];
}

template <class Blk>
NumberlikeArray<Blk>::NumberlikeArray(const Blk *b, Index blen)
        : cap(blen), len(blen) {
    // Create array
    blk = new Blk[cap];
    // Copy blocks
    Index i;
    for (i = 0; i < len; ++i)
        blk[i] = b[i];
}

template <class Blk>
void NumberlikeArray<Blk>::operator==(const NumberlikeArray<Blk> &x) const {
    if (len != x.len)
        return false;
    else {
        // Compare corresponding blocks one by one
        Index i;
        for (i = 0; i < len; ++i)
            if (blk[i] != x.blk[i])
                return false;
        // No blocks different, so the objects are equal
        return true;
    }
}

#endif






