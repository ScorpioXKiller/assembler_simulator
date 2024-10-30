#ifndef __UTILS__H__
#define __UTILS__H__

/**
 * @file utils.c
 * @brief This file contains utility functions for string manipulation and
 * validation.
 */

#include <stdbool.h>

/**
 * @brief Checks if a digit is present in a string.
 *
 * @param str The string to check.
 * @param digit The digit to look for.
 * @return true if the digit is found in the string, false otherwise.
 */
bool isDigitInString(const char *str, int digit);

/**
 * @brief Removes a substring from a string.
 *
 * @param str The string to remove the substring from.
 * @param sub The substring to remove.
 */
void removeSubstring(char *str, const char *sub);

/**
 * @brief Checks if a string represents an integer.
 *
 * @param str The string to check.
 * @return true if the string represents an integer, false otherwise.
 */
bool is_integer(const char *str);

/**
 * @brief Creates a duplicate of a string.
 *
 * @param str The string to duplicate.
 * @return A pointer to the duplicated string.
 */
char *strdup(const char *str);

#endif