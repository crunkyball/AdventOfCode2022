#include <vector>
#include <memory>
#include <algorithm>
#include <cassert>

#include "File.h"
#include "Day13.h"

namespace
{
    using NodeListSharedPtr = std::shared_ptr<struct NodeList>;
    using NodeListWeakPtr = std::weak_ptr<struct NodeList>;

    struct NodeList
    {
        std::vector<struct Node> List;
        NodeListWeakPtr Parent;
    };

    struct Node
    {
        Node(int val) : IntegerValue(val) {}
        Node(NodeListSharedPtr ptr) : NodeListPtr(ptr) {}

        NodeListSharedPtr NodeListPtr = nullptr;
        int IntegerValue = 0;
    };

    enum class CompareResult
    {
        UNKNOWN,
        RIGHT_ORDER,
        WRONG_ORDER
    };

    using NodeListPair = std::pair<NodeListSharedPtr, NodeListSharedPtr>;

    void ParseNodeList(File& file, NodeListSharedPtr pNodeList)
    {
        file.NextChar();

        NodeListSharedPtr pCurrentList = pNodeList;

        while (!file.IsLineEmpty())
        {
            if (file.ReadChar() == '[')
            {
                NodeListSharedPtr pSubList = std::make_shared<NodeList>();
                pSubList->Parent = pCurrentList;
                pCurrentList->List.push_back({ pSubList });
                pCurrentList = pSubList;

                file.NextChar();
            }
            else if (file.ReadChar() == ']')
            {
                pCurrentList = pCurrentList->Parent.lock();

                file.NextChar();
            }
            else if (file.ReadChar() == ',')
            {
                file.NextChar();
            }
            else
            {
                char charBuf[8];
                memset(charBuf, 0, sizeof(charBuf));
                for (int c = 0; file.ReadChar() >= '0' && file.ReadChar() <= '9'; ++c)
                {
                    charBuf[c] = file.ReadChar();
                    file.NextChar();
                }

                int num = atoi(charBuf);
                pCurrentList->List.push_back({ num });
            }
        }
    }

    void ReadDataAndParsePairs(const char* pDataFileName, std::vector<NodeListPair>& outNodeListPairs)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {     
            NodeListSharedPtr pLeft = std::make_shared<NodeList>();
            NodeListSharedPtr pRight = std::make_shared<NodeList>();

            ParseNodeList(file, pLeft);
            file.NextLine();
            ParseNodeList(file, pRight);
            file.NextLine(2);

            outNodeListPairs.push_back({ pLeft, pRight });
        }
    }

    void ReadDataAndParse(const char* pDataFileName, std::vector<NodeListSharedPtr>& outNodeLists)
    {
        File file(pDataFileName);

        while (!file.IsAtEnd())
        {
            if (!file.IsLineEmpty())
            {
                NodeListSharedPtr pNodeList = std::make_shared<NodeList>();
                ParseNodeList(file, pNodeList);
                outNodeLists.push_back(pNodeList);
            }
            
            file.NextLine();
        }
    }

    CompareResult CompareLists(NodeListSharedPtr pLeft, NodeListSharedPtr pRight)
    {
        for (int i = 0; ; ++i)
        {
            if (i < pLeft->List.size() && i < pRight->List.size())
            {
                Node& leftNode = pLeft->List[i];
                Node& rightNode = pRight->List[i];

                //Both integers.
                if (leftNode.NodeListPtr == nullptr && rightNode.NodeListPtr == nullptr)
                {
                    if (leftNode.IntegerValue < rightNode.IntegerValue)
                    {
                        return CompareResult::RIGHT_ORDER;
                    }
                    else if (leftNode.IntegerValue > rightNode.IntegerValue)
                    {
                        return CompareResult::WRONG_ORDER;
                    }
                }
                else if (leftNode.NodeListPtr != nullptr && rightNode.NodeListPtr != nullptr)
                {
                    CompareResult result = CompareLists(leftNode.NodeListPtr, rightNode.NodeListPtr);
                    
                    if (result != CompareResult::UNKNOWN)
                    {
                        return result;
                    }
                }
                else
                {
                    NodeListSharedPtr pLeftNode = leftNode.NodeListPtr;
                    NodeListSharedPtr pRightNode = rightNode.NodeListPtr;

                    if (pLeftNode == nullptr)
                    {
                        pLeftNode = std::make_shared<NodeList>();
                        pLeftNode->List.push_back( {leftNode.IntegerValue} );
                    }
                    else if (pRightNode == nullptr)
                    {
                        pRightNode = std::make_shared<NodeList>();
                        pRightNode->List.push_back({ rightNode.IntegerValue });
                    }
                    
                    CompareResult result = CompareLists(pLeftNode, pRightNode);

                    if (result != CompareResult::UNKNOWN)
                    {
                        return result;
                    }
                }
            }
            //Can we do these before the above for efficiency?
            else if (pLeft->List.size() < pRight->List.size())
            {
                //Left ran out first.
                return CompareResult::RIGHT_ORDER;
            }
            else if (pLeft->List.size() > pRight->List.size())
            {
                //Right ran out first.
                return CompareResult::WRONG_ORDER;
            }
            else
            {
                break;
            }
        }

        return CompareResult::UNKNOWN;
    }

    CompareResult TestPair(NodeListPair& pair)
    {
        NodeListSharedPtr pLeft = pair.first;
        NodeListSharedPtr pRight = pair.second;
        return CompareLists(pLeft, pRight);
    }

    bool SortNodeLists(NodeListSharedPtr pLeft, NodeListSharedPtr pRight)
    {
        return CompareLists(pLeft, pRight) == CompareResult::RIGHT_ORDER;
    }

    void RunPuzzle1(const char* pDataFileName)
    {
        std::vector<NodeListPair> nodeListPairs;

        ReadDataAndParsePairs(pDataFileName, nodeListPairs);

        int sumCorrectPairs = 0;
        
        for (int i = 0; i < nodeListPairs.size(); ++i)
        {
            CompareResult result = TestPair(nodeListPairs[i]);

            if (result == CompareResult::RIGHT_ORDER)
            {
                sumCorrectPairs += (i + 1);
            }
        }

        printf("Sum Correct Pairs: %d\n", sumCorrectPairs);
    }

    void RunPuzzle2(const char* pDataFileName)
    {
        std::vector<NodeListSharedPtr> nodeLists;

        ReadDataAndParse(pDataFileName, nodeLists);

        //Add the two divider packets.
        NodeListSharedPtr pDivider1List = std::make_shared<NodeList>();
        NodeListSharedPtr pDivider1EntryList = std::make_shared<NodeList>();
        pDivider1EntryList->List.push_back({ 2 });
        pDivider1List->List.push_back(pDivider1EntryList);

        NodeListSharedPtr pDivider2List = std::make_shared<NodeList>();
        NodeListSharedPtr pDivider2EntryList = std::make_shared<NodeList>();
        pDivider2EntryList->List.push_back({ 6 });
        pDivider2List->List.push_back(pDivider2EntryList);

        nodeLists.push_back(pDivider1List);
        nodeLists.push_back(pDivider2List);

        std::sort(nodeLists.begin(), nodeLists.end(), SortNodeLists);

        int divider1Pos = 0;
        int divider2Pos = 0;

        for (int i = 0; i < nodeLists.size(); ++i)
        {
            if (nodeLists[i] == pDivider1List)
            {
                divider1Pos = i + 1;
            }
            else if (nodeLists[i] == pDivider2List)
            {
                divider2Pos = i + 1;
            }
        }

        int signal = divider1Pos * divider2Pos;
        printf("Signal: %d\n", signal);
    }
}

void Day13::Run()
{
    //RunPuzzle1("Day13_Sample.txt");
    //RunPuzzle1("Day13_Puzzle.txt");

    //RunPuzzle2("Day13_Sample.txt");
    RunPuzzle2("Day13_Puzzle.txt");
}
