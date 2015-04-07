#ifndef __CASE_MAP_TABLE_H__
#define __CASE_MAP_TABLE_H__

#include <stdint.h>

uint32_t toUpperCase(uint32_t);
uint32_t toLowerCase(uint32_t);

#define GROUP_SIZE  3
#define UNICODE_CASE_MAP_TABLE_SIZE  2188 * GROUP_SIZE

#endif
