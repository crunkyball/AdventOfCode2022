#pragma once

class File
{
public:
    File(const char* fileName);
    virtual ~File();

    int ReadLineAsNumber() const;
    char ReadChar() const;
    void ReadLine(char buffer[], int bufferSize) const;

    void NextLine(int stepSize = 1);
    void NextChar(int stepSize = 1);

    bool IsLineEmpty() const;
    bool IsAtEnd() const;

private:
    char* m_Data = nullptr;
    int m_Size = 0;

    const char* m_pFileEnd;
    const char* m_pFilePos;
};
