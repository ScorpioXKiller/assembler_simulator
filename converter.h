#ifndef __CONVERTER__H__
#define __CONVERTER__H__

/**
 * @file converter.h
 * @brief This file contains functions for converting integers to binary strings
 * and shifting, adding, and encrypting binary strings.
 */

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Converts an integer to a 14-bit binary string.
 *
 * This function takes an integer and converts it to a 14-bit binary string.
 *
 * @param num The integer to convert.
 * @return A pointer to the 14-bit binary string.
 */
char *to_binary14(int num);

/**
 * @brief Shifts a 14-bit binary string to the left by a specified offset.
 *
 * This function takes a 14-bit binary string and an offset, and returns a
 * new string where each bit is shifted to the left by the offset.
 *
 * @param binary_14 The 14-bit binary string to shift.
 * @param offset The number of positions to shift the string to the left.
 * @return A pointer to the shifted binary string.
 */
char *shift_left(const char *binary14, int offset);

/**
 * @brief Adds two 14-bit binary numbers.
 *
 * This function takes two 14-bit binary strings, converts them to integers,
 * adds them together, and then converts the sum back to a 14-bit binary string.
 *
 * @param binary14_1 The first 14-bit binary string.
 * @param binary14_2 The second 14-bit binary string.
 * @return A pointer to the 14-bit binary string representing the sum.
 */
char *add_binary_numbers(const char *binary14_1, const char *binary14_2);

/**
 * @brief Converts a 14-bit binary string to an encrypted base-4 string.
 *
 * This function takes a 14-bit binary string and converts it to an encrypted
 * base-4 string. Each pair of binary digits is converted to a base-4 digit,
 * which is then encrypted to a character according to the following mapping:
 * 0 -> '*', 1 -> '#', 2 -> '%', 3 -> '!'.
 * The resulting string is allocated on
 * the heap and should be freed when no longer needed. If memory allocation
 * fails, the function prints an error message and exits the program.
 *
 * @param binary14 The 14-bit binary string to convert.
 * @return A pointer to the encrypted base-4 string.
 */
char *to_base4_encrypted(char *binary14);

#endif