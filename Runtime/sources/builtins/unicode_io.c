#include <stdio.h>

#include <builtins/builtins.h>

#define UNI_MAX_LEGAL_UTF32 (uint32_t)0x0010FFFF
#define UNI_SUR_HIGH_START  (uint32_t)0xD800
#define UNI_SUR_HIGH_END    (uint32_t)0xDBFF
#define UNI_SUR_LOW_START   (uint32_t)0xDC00
#define UNI_SUR_LOW_END     (uint32_t)0xDFFF
#define UNI_REPLACEMENT_CHAR (uint32_t)0x0000FFFD

static const char trailingBytesForUTF8[256] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

static const uint8_t firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };
static const uint32_t offsetsFromUTF8[6] = { 0x00000000UL, 0x00003080UL, 0x000E2080UL,
                     0x03C82080UL, 0xFA082080UL, 0x82082080UL };

/// Read UTF-8 char as UTF32 char
uint32_t readUTF8Char()
{
    uint32_t res = 0;
    uint8_t ch;

    scanf("%c", &ch);

    int extraBytesToRead = trailingBytesForUTF8[ch];
    int i = 0;

    for (i = 0; i < extraBytesToRead; ++i)
    {
        res += ch;
        res <<= 6;
        scanf("%c", &ch);
    }
    res += ch;
    res -= offsetsFromUTF8[extraBytesToRead];

    if (res <= UNI_MAX_LEGAL_UTF32)
    {
        if (res >= UNI_SUR_HIGH_START && res <= UNI_SUR_LOW_END)
        {
            res = UNI_REPLACEMENT_CHAR;
        }
    }
    else
        res = UNI_REPLACEMENT_CHAR;

    return res;
}

/// Print UTF32 char as UTF8 char
void printUTF32 (uint32_t ch)
{
    const uint32_t byteMask = 0xBF;
    const uint32_t byteMark = 0x80;

    uint8_t out[4];
    int bytesToWrite = 0;

    if (ch < (uint32_t)0x80)
        bytesToWrite = 1;
    else if (ch < (uint32_t)0x800)
        bytesToWrite = 2;
    else if (ch < (uint32_t)0x10000)
        bytesToWrite = 3;
    else if (ch <= UNI_MAX_LEGAL_UTF32)
        bytesToWrite = 4;
    else
    {
        bytesToWrite = 3;
        ch = UNI_REPLACEMENT_CHAR;
    }

    uint8_t* p = out + bytesToWrite - 1;
    while (p > out)
    {
        *p-- = (uint8_t)((ch | byteMark) & byteMask);
        ch >>= 6;
    }

    *p = (uint8_t)(ch | firstByteMark[bytesToWrite]);

    int i;
    for (i = 0; i < bytesToWrite; ++i)
        printf("%c", out[i]);
}
