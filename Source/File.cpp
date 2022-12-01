#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

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

    printf(m_Data);

    m_pFileEnd = m_Data + m_Size;
    m_pFilePos = m_Data;
}

File::~File()
{
    delete[] m_Data;
}

int File::ReadLineAsNumber()
{
    int num = atoi(m_pFilePos);
    return num;
}

bool File::IsLineEmpty() const
{
    bool bEmpty = *m_pFilePos == '\n';
    return bEmpty;
}

void File::NextLine()
{
    while (*m_pFilePos != '\n')
    {
        if (m_pFilePos == m_pFileEnd)
            return;

        m_pFilePos++;
    }

    m_pFilePos++;   //Move to the next line.
}

bool File::IsAtEnd() const
{
    return m_pFilePos == m_pFileEnd;
}
