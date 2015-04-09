#ifndef __CASE_MAP_TABLE_H__
#define __CASE_MAP_TABLE_H__

#include <stdint.h>

/// Переводит символ Юникода в верхний регистр.
/// Символы представлены в формате UTF-32.
uint32_t toUpperCase(uint32_t);

/// Переводит символ Юникода в нижний регистр.
/// Символы представлены в формате UTF-32.
uint32_t toLowerCase(uint32_t);

#define GROUP_SIZE  3
#define UNICODE_CASE_MAP_TABLE_SIZE  2188 * GROUP_SIZE

#endif
