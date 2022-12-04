#include <cassert>
#include <cstring>

#include "Utils.h"

void Utils::SplitString(const char* pString, const char* splitString, char* outLeft, const int leftSize, char* outRight, const int rightSize)
{
    memset(outLeft, 0, leftSize);
    memset(outRight, 0, rightSize);

    int stringLen = static_cast<int>(strlen(pString));
    int breakPoint = 0;

    int splitCharsLen = static_cast<int>(strlen(splitString));
    int splitCharsIdx = 0;

    for (const char* pChar = pString; *pChar != 0; pChar++)
    {
        if (*pChar == splitString[splitCharsIdx])
        {
            if (++splitCharsIdx == splitCharsLen)
            {
                breakPoint = (pChar - pString) - (splitCharsLen - 1);
                break;
            }
        }
        else
        {
            splitCharsIdx = 0;
        }
    }

    assert(breakPoint < leftSize);
    memcpy(outLeft, pString, breakPoint);

    breakPoint += splitCharsLen;	//Don't copy split char.

    assert(stringLen - breakPoint < rightSize);
    memcpy(outRight, pString + breakPoint, stringLen - breakPoint);
}
