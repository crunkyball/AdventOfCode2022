#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <utility>

#include "File.h"

File::File(const char* fileName)
{
    FILE* pFile;
    fopen_s(&pFile, fileName, "r");
    assert(pFile != nullptr);

    fseek(pFile, 0, SEEK_END);
    m_Size = ftell(pFile);
    rewind(pFile);

    int dataSize = m_Size + 1;  //+1 for an NTC, just in-case.
    m_Data = new char[dataSize];
    memset(m_Data, 0, dataSize);
    fread(m_Data, 1, m_Size, pFile);

    fclose(pFile);

    //Recalculate the size as the string length as we're dealing with text 
    //and any CR LF two chars will get converted to a single /n.
    int strLen = static_cast<int>(strlen(m_Data));
    assert(strLen <= m_Size);
    m_Size = strLen;

    m_pFileEnd = m_Data + m_Size;
    m_pFilePos = m_Data;
}

File::~File()
{
    delete[] m_Data;
}

int File::ReadLineAsNumber() const
{
    int num = atoi(m_pFilePos);
    return num;
}

char File::ReadChar() const
{
    return *m_pFilePos;
}

void File::ReadLine(char buffer[], int bufferSize) const
{
    int bufPos = 0;
    for (const char* pChar = m_pFilePos; pChar != m_pFileEnd && *pChar != '\n'; ++pChar)
    {
        if (bufPos < (bufferSize - 1))
        {
            buffer[bufPos++] = *pChar;
        }
        else
        {
            assert(0);  //Buffer size too small!
        }
    }
}

bool File::IsLineEmpty() const
{
    bool bEmpty = *m_pFilePos == '\n';
    return bEmpty;
}

void File::NextLine(int stepSize)
{
    while (stepSize-- > 0)
    {
        while (*m_pFilePos != '\n')
        {
            if (m_pFilePos == m_pFileEnd)
                return;

            m_pFilePos++;
        }

        m_pFilePos++;   //Move to the next line.
    }
}

void File::NextChar(int stepSize)
{
    m_pFilePos += stepSize;
    m_pFilePos = std::min(m_pFilePos, m_pFileEnd);
}

bool File::IsAtEnd() const
{
    return m_pFilePos == m_pFileEnd;
}
