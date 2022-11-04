#ifndef _GETDATAFROMSTR_H_
#define _GETDATAFROMSTR_H_

/**
 * @brief Get the String between two strings
 * 
 * @param srcString Source string.
 * @param strStart String start.
 * @param strEnd String end.
 * @return char* Return pointer address to string.
 */
char* getStrBetweenTwoStr(const char *srcString, const char *strStart, const char *strEnd);

/**
 * @brief Get the integer between two strings
 * 
 * @param srcString Source string.
 * @param strStart String start.
 * @param strEnd String end.
 * @return int Return the integer between two strings.
 */
int getIntBetweenTwoStr(const char *srcString, const char *strStart, const char *strEnd);

#endif // !_GETDATAFROMSTR_H_
