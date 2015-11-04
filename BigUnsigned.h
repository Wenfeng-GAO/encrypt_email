#ifndef BIGUNSIGNED_H
#define BIGUNSIGNED_H

#include "NumberlikeArray.h"

/* A BigUnsigned object represents a nonnegative integer of size limited only by
 * available memory. BigUnsigned support most mathematical operators and can be
 * converted to and from most primitive integer types.
 *
 * The number is stored as a NumberlikeArray of unsigned longs as if it were
 * written in base 256^sizeof(unsigned long).
 */
class BigUnsigned : protected NumberlikeArray<unsigned long> 
{
    public:
        // Enumeration for the result of a comparison
        enum CmpRes { less = -1, equal = 0, greater = 1 };

        // BigUnsigneds are built with a Blk type of unsigned long
        typedef unsigned long Blk;

        typedef NumberlikeArray<Blk>::Index Index;
        using NumberlikeArray<Blk>::N;

    protected:
        /* Create a BigUnsigned with a capacity; for internal use */
        BigUnsigned(int, Index c) : NumberlikeArray<Blk>(0, c) {}

        /* Decreases len to eliminate any leading zero blocks */
        void zapLeadingZeros() {
            while (len > 0 && blk[len-1] == 0)
                --len;
        }

    public:
        /* Constructs zero */
        BigUnsigned() : NumberlikeArray<Blk>() {}

        /* Copy constructor */
        BigUnsigned(const BigUnsigned &x) : NumberlikeArray<Blk>(x) {}

        /* Assignment operator */
        void operator=(const BigUnsigned &x) {
            NumberlikeArray<Blk>::operator=(x);
        }

        /* Constructor that copies from a given array of blocks */
        BigUnsigned(const Blk *b, Index blen) : NumberlikeArray<Blk>(b, blen) {
            // Eliminate any leading zeros we may have been passed
            zapLeadingZeros();
        }

        /* The number is zero if and only if the length is zero */
        bool isZero() const { return NumberlikeArray<Blk>::isEmpty(); }

        /* Destructor. NumberlikeArray does the delete for us */
        ~BigUnsigned() {}

        /* Constructors from primitive integer types */
        BigUnsigned(unsigned long  x);
        BigUnsigned(         long  x);
        BigUnsigned(unsigned int   x);
        BigUnsigned(         int   x);
        BigUnsigned(unsigned short x);
        BigUnsigned(         short x);

    protected:
        /* Helpers */
        template <class X> void initFromPrimitive      (X x);
        template <class X> void initFromSignedPrimitive(X x);

    public:
        /* Converters to primitive integer types */
        unsigned long  toUnsignedLong () const;
        long           toLong         () const;
        unsigned int   toUnsignedInt  () const;
        int            toInt          () const;
        unsigned short toUnsignedShort() const;
        short          toShort        () const;

    protected:
        /* Helpers */
        template <class X> X convertToSignedPrimitive() const;
        template <class X> X convertToPrimitive      () const;

        // COMPARISONS

        /* Compare this to x like Java's */
        CmpRes compareTo(const BigUnsigned &x) const;

        /* Oridinary comparison operators */
        bool operator==(const BigUnsigned &x) const {
            return NumberlikeArray<Blk>::operator==(x);
        }
        bool operator!=(const BigUnsigned &x) const {
            return NumberlikeArray<Blk>::operator!=(x);
        }
        bool operator <(const BigUnsigned &x) const { return compareTo(x) == less   ; }
        bool operator<=(const BigUnsigned &x) const { return compareTo(x) != greater; }
        bool operator>=(const BigUnsigned &x) const { return compareTo(x) != less   ; }
        bool operator >(const BigUnsigned &x) const { return compareTo(x) == greater; }

        // OPERATORS

        // COPY-LESS OPERATIONS

        // These 8: Arguments are read-only operands, result is saved in *this
        void add     (const BigUnsigned &a, const BigUnsigned &b);
        void subtract(const BigUnsigned &a, const BigUnsigned &b);
        void multiply(const BigUnsigned &a, const BigUnsigned &b);

        /* "divide" and "modulo" are no longer offered. Use "divideWithRemainder"
         * instead;
         * "a.dividewithRemainder(b, q)' is like "q = a / b, a %= b;
         * "a.divideWithRemainder(b, a)" throws an exception: it doesn't make
         * sense to write quotient and remainder into the same variable. 
         */
        void divideWithRemainder(const BigUnsigned &b, BigUnsigned &q);

        // OVERLOAD RETURN-BY-VALUE OPERATORS
        BigUnsigned operator+(const BigUnsigned &x) const;
        BigUnsigned operator-(const BigUnsigned &x) const;
        BigUnsigned operator*(const BigUnsigned &x) const;
        BigUnsigned operator/(const BigUnsigned &x) const;
        BigUnsigned operator%(const BigUnsigned &x) const;

        // OVERLOAD ASSIGNMENT OPERATORS
        void operator+=(const BigUnsigned &x);
        void operator-=(const BigUnsigned &x);
        void operator*=(const BigUnsigned &x);
        void operator/=(const BigUnsigned &x);
        void operator%=(const BigUnsigned &x);

        // INCREMENT / DECREMENT OPERATORS
        void operator++(   );
        void operator++(int);
        void operator--(   );
        void operator--(int);
};

/* Implementing the return-by-value and assignment operators in terms of the
 * copy-less operations.
 */

inline BigUnsigned BigUnsigned::operator+(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.add(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator-(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.subtract(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator*(const BigUnsigned &x) const {
    BigUnsigned ans;
    ans.multiply(*this, x);
    return ans;
}
inline BigUnsigned BigUnsigned::operator/(const BigUnsigned &x) const {
    if (x.isZero()) throw "BigUnsigned::operator/: division by zero";
    BigUnsigned q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return q;
}
inline BigUnsigned BigUnsigned::operator%(const BigUnsigned &x) const {
    if (x.isZero()) throw "BigUnsigned::operator%: division by zero";
    BigUnsigned q, r;
    r = *this;
    r.divideWithRemainder(x, q);
    return r;
}

inline void BigUnsigned::operator+=(const BigUnsigned &x) {
    add(*this, x);
}
inline void BigUnsigned::operator-=(const BigUnsigned &x) {
    subtract(*this, x);
}
inline void BigUnsigned::operator*=(const BigUnsigned &x) {
    multiply(*this, x);
}
inline void BigUnsigned::operator/=(const BigUnsigned &x) {
    if (x.isZero()) throw "BigUnsigned::operator/: dividion by zero";
    BigUnsigned q;
    divideWithRemainder(x, q);
    // *this contains the remainder, but we overwrite it with the quotient
    *this = q;
}
inline void BigUnsigned::operator%=(const BigUnsigned &x) {
    if (x.isZero()) throw "BigUnsigned::operator%: dividion by zero";
    BigUnsigned q;
    // Mods *this by x, don't care about quotient left in q
    divideWithRemainder(x, q);
}

/* Templates for conversions fo BigUnsigned to and from primitive integers */

// CONSTRUCTION FROM PRIMITIVE INTEGRS

/* Initialize this BigUnsigned from the given primitive integer. The same pattern 
 * works for all primitive integer types, so put it into a template to reduce code
 * duplication. Type X could be signed, but x is known to be nonnegative.
 */
template <class X>
void BigUnsigned::initFromPrimitive(X x) {
    if (x == 0)
        ; // NumberlikeArray already initialized us to zero
    else {
        // Create a single block. blk is NULL, so no need to delete it
        cap = 1;
        blk = new Blk[1];
        len = 1;
        blk[0] = Blk(x);
    }
}

/* Ditto, but first check that x is nonnegative */
template <class X>
void BigUnsigned::initFromSignedPrimitive(X x) {
    if (x < 0)
        throw "BigUnsigned constructor: "
            "Cannot construct a BigUnsigned from a negative number";
    else
        initFromPrimitive(x);
}

// CONVERSION TO PRIMITIVE INTEGERS

template <class X>
X BigUnsigned::convertToPrimitive() const {
    if (len == 0)
        // The number is zero; return zero
        return 0;
    else if (len == 1) {
        // The single block might fit in an X. Try the conversion
        X x = X(blk[0]);
        // Make sure the result accurately represents the block
        if (Blk(x) == blk[0])
            // Successful conversion
            return x;
        // Otherwise throws
    }
    throw "BigUnsigned::to<Primitive>: "
        "Value is too big to fit in the requested type";
}

/* Wrap the above in an x >= 0 test to make sure we got a nonnegative result */
template <class X>
X BigUnsigned::convertToSignedPrimitive() const {
    X x = convertToPrimitive<X>();
    if (x >= 0)
        return x;
    else
        throw "BigUnsigned::to<Primitive>: "
            "Value is too big to fit in the requested type";
}

#endif
