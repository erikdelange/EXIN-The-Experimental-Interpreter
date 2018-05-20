/* error.h
 *
 * Error message codes
 *
 * 1995	K.W.E. de Lange
 */
#ifndef _ERROR_
#define _ERROR_

#define	NameError 0
#define	TypeError 1
#define SyntaxError 2
#define ValueError 3
#define SystemError 4
#define IndexError 5
#define OutOfMemoryError 6
#define ModNotAllowedError 7
#define DivisionByZeroError 8

extern void error(int number, ...);

#endif
