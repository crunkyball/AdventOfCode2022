#include <vector>
#include <memory>
#include <cassert>

#include "File.h"
#include "Utils.h"
#include "Day7.h"

namespace
{
    const char kCommandChar = '$';
    
    static const int kFileNameSize = 32;

    using DirectorySharedPtr = std::shared_ptr<class Directory>;
    using DirectoryPtr = std::weak_ptr<class Directory>;
    using FilePair = std::pair<char[kFileNameSize], int>;

    class Directory : public std::enable_shared_from_this<Directory>
    {
    public:
        Directory(const char* pName, DirectoryPtr parentDir = DirectoryPtr())
        {
            assert(strlen(pName) < kFileNameSize);
            strcpy_s(m_DirName, kFileNameSize, pName);
            m_ParentDir = parentDir;
        }

        bool IsRoot() const { return m_ParentDir.expired(); }
        DirectoryPtr GetParentDir() { return m_ParentDir; }

        void AddSubDir(const char* pName)
        {
            DirectorySharedPtr subDir = std::make_shared<Directory>(pName, shared_from_this());
            m_SubDirs.push_back(subDir);
        }

        void AddFile(const char* pName, int size)
        {
            m_Files.emplace_back();

            FilePair& filePair = m_Files[m_Files.size() - 1];
            strcpy_s(filePair.first, kFileNameSize, pName);
            filePair.second = size;
        }

        DirectoryPtr FindSubDir(const char* pDirName)
        {
            for (DirectorySharedPtr dir : m_SubDirs)
            {
                if (strcmp(dir->m_DirName, pDirName) == 0)
                {
                    return dir;
                }
            }

            return DirectoryPtr();
        }

        int CalculateDirSize()
        {
            assert(m_DirSize == 0);

            for (DirectoryPtr subDir : m_SubDirs)
            {
                DirectorySharedPtr pSubDir = subDir.lock();

                if (pSubDir != nullptr)
                {
                    m_DirSize += pSubDir->CalculateDirSize();
                }
            }

            for (auto filePair : m_Files)
            {
                m_DirSize += filePair.second;
            }

            return m_DirSize;
        }

        int GetDirSize() const { return m_DirSize; }
        const std::vector<DirectorySharedPtr>& GetSubDirs() const { return m_SubDirs; }

    private:
        char m_DirName[kFileNameSize];
        DirectoryPtr m_ParentDir;
        std::vector<DirectorySharedPtr> m_SubDirs;
        std::vector<FilePair> m_Files;

        int m_DirSize = 0;
    };

    void Command_NavigateUp(DirectoryPtr& outDir, bool toRoot)
    {
        DirectorySharedPtr pDir = outDir.lock();

        while (pDir != nullptr && !pDir->IsRoot())
        {
            outDir = pDir->GetParentDir();

            if (!toRoot)
            {
                break;
            }
        }
    }

    void Command_ChangeDir(DirectoryPtr& outDir, const char* pDirName)
    {
        DirectoryPtr subDir = outDir.lock()->FindSubDir(pDirName);
        assert(!subDir.expired());
        outDir = subDir;
    }

    void Command_ListDir(File& file, DirectoryPtr& outDir)
    {
        DirectorySharedPtr pDir = outDir.lock();

        if (pDir != nullptr)
        {
            while (!file.IsAtEnd() && file.ReadChar() != kCommandChar)
            {
                const int kLineBufSize = 32;
                char lineBuf[kLineBufSize];
                file.ReadLine(lineBuf, kLineBufSize);

                const int kSplitBufSize = 16;
                char firstBuf[kSplitBufSize];
                char secondBuf[kSplitBufSize];

                Utils::SplitString(lineBuf, " ", firstBuf, kSplitBufSize, secondBuf, kSplitBufSize);

                if (strcmp(firstBuf, "dir") == 0)
                {
                    pDir->AddSubDir(secondBuf);
                }
                else
                {
                    int fileSize = atoi(firstBuf);
                    pDir->AddFile(secondBuf, fileSize);
                }

                file.NextLine();
            }
        }
    }

    void ParseCommand(File& file, DirectoryPtr& outCurrentDir)
    {
        assert(file.ReadChar() == kCommandChar);
        file.NextChar(2);

        const int kLineBufSize = 32;
        char lineBuf[kLineBufSize];
        file.ReadLine(lineBuf, kLineBufSize);

        const int kCommandBufSize = 8;
        char commandBuf[kCommandBufSize];

        const int kParamBufSize = 16;
        char paramBuf[kParamBufSize];

        Utils::SplitString(lineBuf, " ", commandBuf, kCommandBufSize, paramBuf, kParamBufSize);

        if (strcmp(commandBuf, "cd") == 0)
        {
            file.NextLine();
            if (strcmp(paramBuf, "/") == 0)
            {
                Command_NavigateUp(outCurrentDir, true);
            }
            else if (strcmp(paramBuf, "..") == 0)
            {
                Command_NavigateUp(outCurrentDir, false);
            }
            else
            {
                Command_ChangeDir(outCurrentDir, paramBuf);
            }
        }
        else if (strcmp(commandBuf, "ls") == 0)
        {
            file.NextLine();
            Command_ListDir(file, outCurrentDir);
        }
        else
        {
            assert(0);  //Unhandled!
        }
    }

    void ReadDataAndParse(const char* pFileName, DirectoryPtr rootDir)
    {
        File file(pFileName);

        while (!file.IsAtEnd())
        {
            ParseCommand(file, rootDir);
        }
    }

    int SumDirsUnderSize(const DirectorySharedPtr& dir, int size)
    {
        int sum = 0;

        for (auto subDir : dir->GetSubDirs())
        {
            sum += SumDirsUnderSize(subDir, size);
        }

        if (dir->GetDirSize() <= size)
        {
            sum += dir->GetDirSize();
        }

        return sum;
    }

    DirectorySharedPtr FindSpaceToFree(const DirectorySharedPtr& dir, int spaceToFree)
    {
        DirectorySharedPtr pSmallestDirWithSpace = nullptr;

        for (auto subDir : dir->GetSubDirs())
        {
            DirectorySharedPtr pSubDirSmallest = FindSpaceToFree(subDir, spaceToFree);

            if ( pSmallestDirWithSpace == nullptr || (pSubDirSmallest != nullptr && pSubDirSmallest->GetDirSize() < pSmallestDirWithSpace->GetDirSize()) )
            {
                pSmallestDirWithSpace = pSubDirSmallest;
            }
        }

        if (dir->GetDirSize() >= spaceToFree && (pSmallestDirWithSpace == nullptr || dir->GetDirSize() < pSmallestDirWithSpace->GetDirSize()))
        {
            pSmallestDirWithSpace = dir;
        }
        
        return pSmallestDirWithSpace;
    }

    DirectorySharedPtr BuildFilesystem(const char* pDataFileName)
    {
        DirectorySharedPtr rootDir = std::make_shared<Directory>("/");
        ReadDataAndParse(pDataFileName, rootDir);

        rootDir->CalculateDirSize();

        return rootDir;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        DirectorySharedPtr rootDir = BuildFilesystem(pDataFileName);

        int sum = SumDirsUnderSize(rootDir, 100000);

        printf("Sum: %d\n", sum);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        DirectorySharedPtr rootDir = BuildFilesystem(pDataFileName);

        const int filesystemSize = 70000000;
        const int spaceNeeded = 30000000;

        int freeSpace = filesystemSize - rootDir->GetDirSize();
        int spaceToFree = spaceNeeded - freeSpace;

        DirectorySharedPtr smallestDirWithSpace = FindSpaceToFree(rootDir, spaceToFree);

        printf("Dir Size: %d\n", smallestDirWithSpace->GetDirSize());
    }
}

void Day7::Run()
{
    //RunPuzzle1("Day7_Sample.txt");
    //RunPuzzle1("Day7_Puzzle.txt");

    //RunPuzzle2("Day7_Sample.txt");
    RunPuzzle2("Day7_Puzzle.txt");
}
