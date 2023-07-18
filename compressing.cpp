#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <fstream>
#include <stdexcept>
using namespace std;

const int AND8 = 255;
const int INT_SZ_IN_CHAR = 4;
const int SZ_CHAR = 8 * sizeof(char);

class HuffmanTreeNode
{
public:
    bool isEOF, isChar;
    char character;
    int frequency;
    HuffmanTreeNode *leftChild = nullptr;
    HuffmanTreeNode *rightChild = nullptr;

    HuffmanTreeNode(char inputChar, bool isEOF, int frequency)
    {
        this->character = inputChar;
        this->isEOF = isEOF;
        this->isChar = !isEOF;
        this->frequency = frequency;
    }

    HuffmanTreeNode(int frequency)
    {
        this->frequency = frequency;
        this->isChar = false;
        this->isEOF = false;
    }

    void makeLeftChild(HuffmanTreeNode *child)
    {
        this->leftChild = child;
    }

    void makeRightChild(HuffmanTreeNode *child)
    {
        this->rightChild = child;
    }

    void generateCharacterCodes(string &characterCode, map<char, string> &characterCodeMap, string &eofCode)
    {

        if (isChar)
        {
            characterCodeMap[character] = characterCode;
            return;
        }

        if (isEOF)
        {
            eofCode = characterCode;
            return;
        }

        if (this->leftChild != nullptr)
        {
            characterCode.push_back('0');
            (*this->leftChild).generateCharacterCodes(characterCode, characterCodeMap, eofCode);
            characterCode.pop_back();
        }

        if (this->rightChild != nullptr)
        {
            characterCode.push_back('1');
            (*this->rightChild).generateCharacterCodes(characterCode, characterCodeMap, eofCode);
            characterCode.pop_back();
        }
    }
};

class HuffmanNodePointer
{
public:
    HuffmanTreeNode *nodePointer;

    HuffmanNodePointer(HuffmanTreeNode *nodePointer)
    {
        this->nodePointer = nodePointer;
    }
};

bool operator>(const HuffmanNodePointer &node1, const HuffmanNodePointer &node2)
{
    return node1.nodePointer->frequency > node2.nodePointer->frequency;
}

HuffmanTreeNode *buildHuffmanTree(map<char, int> &frequencyMap)
{
    priority_queue<HuffmanNodePointer, vector<HuffmanNodePointer>, greater<HuffmanNodePointer>> pq;

    for (auto it : frequencyMap)
    {
        HuffmanTreeNode *newNode = new HuffmanTreeNode(it.first, false, it.second);
        HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);
        pq.push(newPointer);
    }

    HuffmanTreeNode *newNode = new HuffmanTreeNode('~', true, 1);
    HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);
    pq.push(newPointer);

    while (pq.size() > 1)
    {
        HuffmanNodePointer node1 = pq.top();
        pq.pop();

        HuffmanNodePointer node2 = pq.top();
        pq.pop();

        HuffmanTreeNode *newNode = new HuffmanTreeNode(node1.nodePointer->frequency + node2.nodePointer->frequency);
        
        (*newNode).makeLeftChild(node1.nodePointer);
        (*newNode).makeRightChild(node2.nodePointer);
        
        HuffmanNodePointer newPointer = HuffmanNodePointer(newNode);

        pq.push(newPointer);
    }

    return pq.top().nodePointer;
}

void createFrequencyMap(map<char, int> &frequencyMap, fstream &SourceFile)
{
    char ch;

    while (SourceFile >> noskipws >> ch)
    {
        frequencyMap[ch]++;
    }

    return;
}

void breakInteger(int n, char chArr[])
{
    for (int i = 0; i < INT_SZ_IN_CHAR; i++)
    {
        chArr[i] = ((n >> ((INT_SZ_IN_CHAR - i - 1) * SZ_CHAR)) & AND8);
    }
}

void createHeaderFile(map<char, int> &frequencyMap, fstream &HeaderFile)
{
    char chArr[INT_SZ_IN_CHAR];

    for (auto it : frequencyMap)
    {
        HeaderFile << it.first;
        breakInteger(it.second, chArr);

        for (int i = 0; i < INT_SZ_IN_CHAR; i++)
        {
            HeaderFile << chArr[i];
        }
    }
}

int toInt(char ch)
{
    return (ch == '0') ? 0 : 1;
}

void createBodyFile(map<char, string> &characterCodeMap, string eofCode, fstream &SourceFile, fstream &BodyFile)
{
    char ch;
    queue<char> outputQueue;

    while (SourceFile >> noskipws >> ch)
    {
        for (auto it : characterCodeMap[ch])
        {
            outputQueue.push(it);
        }
    }

    for (auto it : eofCode)
    {
        outputQueue.push(it);
    }

    while (!outputQueue.empty())
    {
        char opchar = 0;
        int val = 0;

        for (int i = 7; i >= 0; i--)
        {
            if (!outputQueue.empty())
            {
                val = toInt(outputQueue.front());
                outputQueue.pop();
            }

            opchar |= val << i;
        }

        BodyFile << opchar;
    }
}

int main()
{
    fstream SourceFile, HeaderFile, BodyFile;

    try
    {
        SourceFile.open("source.txt", ios::in | ios::binary);
        HeaderFile.open("header.huffh", ios::trunc | ios::out | ios::binary);
        BodyFile.open("body.huffb", ios::trunc | ios::out | ios::binary);
    }
    catch (...)
    {
        cerr << "Error opening files" << endl;
        exit(1);
    }

    map<char, int> frequencyMap;
    map<char, string> characterCodeMap;
    string eofCode;
    string characterCode = "";

    createFrequencyMap(frequencyMap, SourceFile);
    SourceFile.clear();
    SourceFile.seekg(0);

    HuffmanTreeNode *root = buildHuffmanTree(frequencyMap);

    root->generateCharacterCodes(characterCode, characterCodeMap, eofCode);

    createHeaderFile(frequencyMap, HeaderFile);
    createBodyFile(characterCodeMap, eofCode, SourceFile, BodyFile);

    SourceFile.close();
    HeaderFile.close();
    BodyFile.close();

    return 0;
}