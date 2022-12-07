#include <cassert>
#include <cstring>

#include "Utils.h"

void Utils::SplitString(const char* pString, const char* splitString, char* outLeft, const int leftSize, char* outRight, const int rightSize)
{
    memset(outLeft, 0, leftSize);
    memset(outRight, 0, rightSize);

    int stringLen = static_cast<int>(strlen(pString));
    int breakPoint = stringLen; //If splitString isn't found then put the whole string on the left side.

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

    int rightNumChars = stringLen - breakPoint;

    if (rightNumChars > 0)
    {
        assert(stringLen - breakPoint < rightSize);
        memcpy(outRight, pString + breakPoint, rightNumChars);
    }
}
