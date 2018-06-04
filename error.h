/* error.h
 *
 * Error message codes
 *
 * 1995	K.W.E. de Lange
 */
#ifndef _ERROR_
#define _ERROR_

/* Error number is used as program return code, so numbering starts at 1
 * because return code 0 indicates no error */
#define	NameError 1
#define	TypeError 2
#define SyntaxError 3
#define ValueError 4
#define SystemError 5
#define IndexError 6
#define OutOfMemoryError 7
#define ModNotAllowedError 8
#define DivisionByZeroError 9

extern void error(int number, ...);

#endif
