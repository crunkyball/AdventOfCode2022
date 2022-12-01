#pragma once

class File
{
public:
    File(const char* fileName);
    virtual ~File();

    int ReadLineAsNumber();
    bool IsLineEmpty() const;
    void NextLine();

    bool IsAtEnd() const;

private:
    char* m_Data = nullptr;
    int m_Size = 0;

    const char* m_pFileEnd;
    const char* m_pFilePos;
};
