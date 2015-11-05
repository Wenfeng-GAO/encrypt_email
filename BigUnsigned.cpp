#include "BigUnsigned.h"

// Memory management definitions are at the bottom of NumberlikeArray.hh

// The templates used by these constructors and converters are at the bottom 
// of BigUnsigned.hh

BigUnsigned::BigUnsigned(unsigned long  x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(unsigned int   x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(unsigned short x) { initFromPrimitive      (x); }
BigUnsigned::BigUnsigned(long           x) { initFromSignedPrimitive(x); }
BigUnsigned::BigUnsigned(int            x) { initFromSignedPrimitive(x); }
BigUnsigned::BigUnsigned(short          x) { initFromSignedPrimitive(x); }

unsigned long  BigUnsigned::toUnsignedLong () const { return convertToPrimitive      <unsigned long >(); }
unsigned int   BigUnsigned::toUnsignedInt  () const { return convertToPrimitive      <unsigned int  >(); }
unsigned short BigUnsigned::toUnsignedShort() const { return convertToPrimitive      <unsigned short>(); }
long           BigUnsigned::toLong         () const { return convertToSignedPrimitive<long          >(); }
int            BigUnsigned::toInt          () const { return convertToSignedPrimitive<int           >(); }
short          BigUnsigned::toShort        () const { return convertToSignedPrimitive<short         >(); }

// COMPARISON
BigUnsigned::CmpRes BigUnsigned::compareTo(const BigUnsigned &x) const {
    // A bigger length implies a bigger number
    if (len < x.len)
        return less;
    else if (len > x.len)
        return greater;
    else {
        // Compare blocks one by one
        Index i = len;
        while (i > 0) {
            --i;
            if (blk[i] == x.blk[i])
                continue;
            else if (blk[i] > x.blk[i])
                return greater;
            else
                return less;
        }
        // If no blocks differed, the numbers are equal
        return equal;
    }
}

// COPY-LESS OPERATIONS

/* On most calls to copy-less operations, it's safe to read the inputs little ny little and write the
 * outputs little by little. However, if one of the inputs is coming from the same variable into which
 * the output is to be stored (an "aliased" call), we risk overwriting the input before we read it.
 * In this case, we first comput the ressult into a temporary BigUnsigned variable and then copy it
 * into the requested output variable *this. Each put-here operation ueses the DTRT_ALIASED macro (Do
 * The Right Thing on aliased calls) to generate code for this check.
 */
#define DTRT_ALIASED(cond, op) \
    if (cond) { \
        BigUnsigned tmpThis; \
        tmpThis.op; \
        *this = tmpThis; \
        return; \
    }

void BigUnsigned::add(const BigUnsigned &a, const BigUnsigned &b) {
    DTRT_ALIASED(this == &a || this == &b, add(a, b));
    // if one argument is zero, copy the other
    if (a.len == 0) {
        operator=(b);
        return;
    } else if (b.len == 0) {
        operator=(a);
        return;
    }
    // Some variables...
    // Carries in and out of an addition operation
    bool carryIn, carryOut;
    Blk temp;
    Index i;
    // a2 points to the longer input, b2 points to the shorter
    const BigUnsigned *a2, *b2;
    if (a.len >= b.len) {
        a2 = &a;
        b2 = &b;
    } else {
        a2 = &b;
        b2 = &a;
    }
    // Set preliminary length and room in this BigUnsigned
    len = a2->len + 1;
    allocate(len);
    // For each block index that is present in both inputs...
    for (i = 0, carryIn = false; i < b2->len; ++i) {
        // Add input blocks
        temp = a2->blk[i] + b2->blk[i];
        // If a rollover occurred, the result is less than either input
        carryOut = (temp < a2->blk[i]);
        // If a carry was input, handle it
        if (carryIn) {
            ++temp;
            carryOut |= (temp == 0);
        }
        blk[i] = temp;      // Save the addition result
        carryIn = carryOut; // Pass the carry
    }
    // If there is a carry left over, increase blocks until one does not
    // roll over
    for (; i < a2->len && carryIn; ++i) {
        temp = a2->blk[i] + 1;
        carryIn = (temp == 0);
        blk[i] = temp;
    }
    // Set the extra block if there's still a carry, decrease length otherwise
    if (carryIn)
        blk[i] = 1;
    else
        --len;
    // If the carry was resolved but the larger number still has blocks,
    // copy them over.
    for (; i < a2->len; ++i)
        blk[i] = a2->blk[i];
}

void BigUnsigned::subtract(const BigUnsigned &a, const BigUnsigned &b) {
    DTRT_ALIASED(this == &a || this == &b, subtract(a, b));
    if (b.len == 0) {
        // If b is zero, copy a
        operator=(a);
        return;
    } else if (a.len < b.len)
        // If a is shorter than b, the result is negative
        throw "BigUnsigned::subtract: "
            "Negative result in unsigned calculation";
    // Some variables...
    bool borrowIn, borrowOut;
    Blk temp;
    Index i;
    // Set preliminary length and make room
    len = a.len;
    allocate(len);
    // For each block index that is present in both inputs...
    for (i = 0, borrowIn = false; i < b.len; ++i) {
        temp = a.blk[i] - b.blk[i];
        // If a reverse rollover occurred, the 
        // result is greater than the block from a
        borrowOut = (temp > a.blk[i]);
        // Handle an incoming borrow
        if (borrowIn) {
            borrowOut |= (temp == 0);
            temp --;
        }
        blk[i] = temp; // Save the subtraction result
        borrowIn = borrowOut; // Pass the borrow along
    }
    // If there is a borrow left over, decrease blocks until one does 
    // not reverse rollover
    for (; i < a.len && borrowIn; ++i) {
        borrowIn = (a.blk[i] == 0);
        blk[i] = a.blk[i] - 1;
    }
    // If there's still a borrow, the result is negative.
    // Throw an exception, but zero out this object so as to leave it
    // in a predictable state.
    if (borrowIn) {
        len = 0;
        throw "BigUnsigned::subtract: Negative result in unsigned calculation";
    } else
        // Copy over the rest of the blocks
        for (; i < a.len; ++i)
            blk[i] = a.blk[i];
    // Zap leading zeros
    zapLeadingZeros();
}
