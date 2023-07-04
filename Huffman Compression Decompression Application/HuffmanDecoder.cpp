#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

unordered_map<char, string> characterAndCodeMap;

struct Node
{
    char character;
    Node* left;
    Node* right;
    bool IsALeafNode;
    Node(char C)
        : character(C), left(NULL), right(NULL), IsALeafNode(true)
    {
    }
    Node()
        : left(NULL), right(NULL), IsALeafNode(false)
    {
    }
};

void HuffamanDecompressData(string, string);
string ReadFileDataToString(string);
Node* CalculateHuffmanTree(string&);
void Helper(Node*, char, string, int);
void DisplayCode();
string DecodeData(Node* ,string);
void WriteDecodedDataToFile(string, string);

int main()
{
    cout << "Welcome to the Huffman decompression Application!" << endl;
    cout << "Please provide the file name (with extension) to be decompressed(ex. filename.bin):" << endl;

    string InputFilename;
    cin >> InputFilename;
    
    string OutputFilename = "Output.txt";

    HuffamanDecompressData(InputFilename, OutputFilename);

    cout << endl << "Press enter to exit...";
    cin.ignore();
    cin.get();

    return 0;
}

void HuffamanDecompressData(string InputFilename, string OutputFilename)
{
    string text;
    string result;

    text = ReadFileDataToString(InputFilename);
    if(text == "NULL")
    {
        return;
    }
    Node* root = CalculateHuffmanTree(text);
    // DisplayCode(); // Uncomment if you want to display the character codes.
    result = DecodeData(root,text);
    WriteDecodedDataToFile(OutputFilename, result);
}

string ReadFileDataToString(string filename)
{
    ifstream inputFile(filename, ios::binary);
    
    if (!inputFile) {
        cout << "Error opening file." << endl;
        return "NULL";
    }
    string fileContent;
    char byte;
    while (inputFile.get(byte)) {
        fileContent.push_back(byte);
    }

    inputFile.close();
    //cout << "Content of "<< filename << " has been read successfully!\n" << fileContent << endl << endl;
    cout << "************************************************************" << endl;
    cout << "Content of "<< filename << " has been read successfully!" << endl;
    return fileContent;
}

Node* CalculateHuffmanTree(string& text)
{
    int i = 0;
    while (text[i] != '.')
    {
        if(text[i] == '|')
        {
            i++;
            char c = text[i++];
            string code;
            while(text[i] != '|' && text[i] != '.')
            {
                code += text[i++];
            }
            characterAndCodeMap[c] = code;
        }
    }
    //cout << i << endl;
    text = text.substr(i+1,text.size()-i-1);
    //cout << text <<" length : "<< text.size() << endl;
    cout << "Successfully extracted characters and their codes."<< endl;
    Node* root = new Node();
    for (const auto& pair : characterAndCodeMap) {
        Helper(root, pair.first, pair.second, 0);
    }
    cout << "Successfully built the Huffman Tree." << endl;
    return root;
}

void Helper(Node* root, char c, string code, int i)
{
    if(i >= code.size()-1)
    {
        if(code[i] == '0')
        {
            if(root->left == NULL)
            {
                root->left = new Node(c);
            }
        }
        else if(code[i] == '1')
        {
            if(root->right == NULL)
            {
                root->right = new Node(c);
            }
        }
        return;
    }

    if(code[i] == '0')
    {
        if(root->left == NULL)
        {
            root->left = new Node();
        }
        Helper(root->left, c, code, i+1);
    }
    else if(code[i] == '1')
    {
        if(root->right == NULL)
        {
            root->right = new Node();
        }
        Helper(root->right, c, code, i+1);
    }
}

string DecodeData(Node* root, string text)
{
    string temp;
    string result;
    for(int i = 0; i < text.size(); i++)
    {
        string tempString;
        for(int j = 7 ; j >= 0; j--)
        {
            if(text[i] & (1 << j))
            {
                tempString.push_back('1');
            }
            else
            {
                tempString.push_back('0');
            }
        }
        temp += tempString;
    }
    
    // character codes to characters
    Node* itr = root;
    for(int i = 0; i < temp.size(); i++)
    {
        if(itr->IsALeafNode)
        {
            result.push_back(itr->character);
            itr = root;
            i--;         
        }
        else if(temp[i] == '0')
        {
            itr = itr->left;
        }
        else
        {
            itr = itr->right;
        }
    }

    // if(temp.size() % 8 != 0)
    // {
    //     unsigned char byte = 0;
    //     int offset = (temp.size()/8) * 8;
    //     for(int i = 0 ; i < temp.size() % 8; i++)
    //     {
    //         if(temp[offset + i] == '0')
    //         {
    //             byte &= ~(1 << (8-i));
    //         }
    //         else
    //         {
    //             byte |= (1 << (8-i));
    //         }
    //     }
    //     result.push_back(byte);
    // }
    // cout << endl << "String stored in .bin File : " << endl << result << " no. of characters = " << result.size() << endl;
    cout << "Decoding successfully!" << endl;
    return result;
}

void WriteDecodedDataToFile(string filename, string data)
{
    ofstream file(filename);
    
    if (!file) {
        cout << "Error opening the file." << endl;
        return;
    }

    const char* bytes = data.c_str();
    file.write(bytes, data.size());
    file.close();
    cout << "Decoded data stored in " << filename << " file." << endl;
    cout << "************************************************************";
}

void DisplayCode()
{
    cout << "Huffman Codes" << endl;
    for (const auto& pair : characterAndCodeMap) {
        cout << pair.first << " : " << pair.second << endl;
    }
}
