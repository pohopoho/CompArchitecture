#ifndef SUMINDICESOF_H
#define SUMINDICESOF_H

/***  DO NOT MODIFY THIS FILE IN ANY WAY!!  ***/

/** Computes sum of locations of given character in a given C string.
 * 
 *  Pre:  Str is a valid C string
 *        Target is set to the desired character
 * 
 *  Returns:  sum of indices at which Target occurs in Str;
 *            -1 if Target does not occur in Str
 * 
 *  Restrictions:
 *    You may not use any Standard Library string functions; if you
 *    do not include string.h, you will satisfy this requirement.
 *    You may not write output within your submitted solution; but,
 *    you may write output during your testing.  However, if so, you
 *    must delete any such code, and any include for stdio.h, before
 *    submitting your solution.
 * 
 *    Violations of these restrictions will result in a score of 0.
 */
int sumIndicesOf(const char Str[], const char Target);

#endif
