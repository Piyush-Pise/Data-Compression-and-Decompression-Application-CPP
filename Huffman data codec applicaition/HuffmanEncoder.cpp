#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

unordered_map<char, int> CharacterAndFrequencyMap;
unordered_map<char, string> characterAndCodeMap;

struct Node
{
    char character;
    int frequency;
    Node* left;
    Node* right;
    bool IsALeafNode;
    Node(char C, int Frequency)
        : character(C), frequency(Frequency), left(NULL), right(NULL), IsALeafNode(true)
    {
    }
    Node(int Frequency)
        : frequency(Frequency), left(NULL), right(NULL), IsALeafNode(false)
    {
    }
};

void HuffamanCompressData(string , string);
string ReadFileDataToString(string);
void CalculateFrequency(string);
void DisplayFrequency();
void CalculateCodes();
vector<Node*> MergeSort(vector<Node*>);
void HuffmanTreeToCodes(Node*, string);
void DisplayCode();
string EncodedString(string);
bool WriteEncodedDataToFile(string, string);

int main()
{   
    cout << "Welcome to the Huffman Compression Application!"<< endl;
    cout << "Please provide the file name (with extension) to be compressed(ex. filename.txt):" << endl;
    string InputFilename;
    cin >> InputFilename;
    string OutputFilename = "EncodedOutput.bin";
    
    HuffamanCompressData(InputFilename, OutputFilename);
    
    cout << endl << "Press enter to exit...";
    cin.ignore();
    cin.get();
    
    return 0;
}

void HuffamanCompressData(string InputFilename, string OutputFilename)
{
    string text;
    string result;

    text = ReadFileDataToString(InputFilename);
    if(text == "NULL")
    {
        return;
    }
    int textSize = text.size();
    CalculateFrequency(text);
    //DisplayFrequency();  // Uncomment if you want to display the frequency of each character.
    CalculateCodes();
    //DisplayCode(); // Uncomment if you want to display the character codes.
    result = EncodedString(text);
    int resultSize = result.size();
    bool status = WriteEncodedDataToFile(OutputFilename, result);
    if(!status)
    {
        return;
    }
    cout << "Compression ratio : " << (1.0-(resultSize*1.0)/(textSize*1.0))*100.0 << "%"<< endl;
    cout << "************************************************************";
}

string ReadFileDataToString(string filename)
{
    ifstream inputFile(filename);
    
    if (!inputFile) {
        cout << "Error opening file." << std::endl;
        return "NULL";
    }
    
    // string fileContent((istreambuf_iterator<char>(inputFile)), 
    //                             (istreambuf_iterator<char>()));
    string fileContent;
    char byte;
    while (inputFile.get(byte)) {
        fileContent.push_back(byte);
    }

    inputFile.close();
    //cout << "File content has been read:\n" << fileContent << endl << endl;
    cout << "************************************************************" << endl;
    cout << "File content has been read successfully!\n";
    return fileContent;
}

string EncodedString(string text)
{
    string temp;
    string result;
    for(int i = 0; i < text.size(); i++)
    {
        temp += characterAndCodeMap[text[i]];
    }
    // cout << endl << "Encoded String : " << temp << " lenght : " << temp.size() << endl;
    
    for(int i = 0; i < temp.size()/8; i++)
    {
        string tempString;
        for(int j = 0; j < 8; j++)
        {
            tempString += temp[i*8+j];
        }
        
        unsigned char byte = static_cast<char>(bitset<8>(tempString).to_ulong());
        result.push_back(byte);
    }

    if(temp.size() % 8 != 0)
    {
        char byte = 0;
        int offset = (temp.size()/8) * 8;
        for(int i = 0 ; i < temp.size() % 8; i++)
        {
            if(temp[offset + i] == '0')
            {
                byte &= ~(1 << (7-i));
            }
            else
            {
                byte |= (1 << (7-i));
            }
        }
        result.push_back(byte);
    }
    // cout << endl << "String stored in .bin File : " << result << " no. of characters = " << result.size() << endl;
    cout << "Encoding successfully!" << endl;
    return result;
}

bool WriteEncodedDataToFile(string filename, string data)
{
    ofstream outfile(filename, ios::binary);

    if (!outfile) {
        cout << "Error Opening The Output File." << endl;
        return false;
    }
    // Storing characters and thier corresponding codes
    for (const auto& pair : characterAndCodeMap) {
        outfile << "|" << pair.first << pair.second;
    }
    outfile << ".";

    int str_len = data.length();
    //outfile.write((char*)&str_len, sizeof(int));
    outfile.write(data.c_str(), str_len);
    outfile.close();
    cout << "Encoded data stored in "<< filename << " file." << endl;
    return true;
}

void CalculateFrequency(string text)
{
    for (int i = 0; i < text.size(); i++)
    {
        if (CharacterAndFrequencyMap.count(text[i]) == 0)
        {
            CharacterAndFrequencyMap[text[i]] = 1;
        }
        else
        {
            CharacterAndFrequencyMap[text[i]]++;
        }
    }
}

void CalculateCodes()
{
    vector<Node*> arr;

    for (const auto& pair : CharacterAndFrequencyMap) {
        arr.push_back(new Node(pair.first, pair.second));
    }

    while (arr.size() > 1) 
    {
        arr = MergeSort(arr);
        Node* nodeA = arr[0];
        Node* nodeB = arr[1];
        arr.erase(arr.begin());
        arr.erase(arr.begin());
        Node* newNode = new Node(nodeA->frequency + nodeB->frequency);
        newNode->left = nodeA;
        newNode->right = nodeB;
        arr.push_back(newNode);
    }

    // Huffman tree has been calculated //
    Node* root = arr[0];
    
    HuffmanTreeToCodes(root,"");
}

void HuffmanTreeToCodes(Node* root, string code)
{
    if (root->IsALeafNode)
    {
        char c = root->character;
        characterAndCodeMap[c] = code;
        return;
    }

    if (root->left != NULL)
    {
        string leftCode = code;
        leftCode.push_back('0');
        HuffmanTreeToCodes(root->left, leftCode);
    }
    if (root->right != NULL)
    {
        string rightCode = code;
        rightCode.push_back('1');
        HuffmanTreeToCodes(root->right, rightCode);
    }
    return;
}

void DisplayFrequency()
{
    cout << "Character Frequencies" << endl;
    for (const auto& pair : CharacterAndFrequencyMap) {
        cout << pair.first << " : " << pair.second << endl;
    }
}

void DisplayCode()
{
    cout << "Huffman Codes" << endl;
    for (const auto& pair : characterAndCodeMap) {
        cout << pair.first << " : " << pair.second << endl;
    }
}

vector<Node*> MergeSort(vector<Node*> arr)
{
    if (arr.size() == 1)
    {
        return arr;
    }
    int mid = arr.size() / 2;
    vector<Node*> left;
    vector<Node*> right;

    for (int i = 0; i < arr.size(); i++)
    {
        if (i < mid)
        {
            left.push_back(arr[i]);
        }
        else
        {
            right.push_back(arr[i]);
        }
    }

    left = MergeSort(left);
    right = MergeSort(right);

    int l = 0;
    int r = 0;

    vector<Node*> result;

    while (l + r < arr.size())
    {
        if (l >= left.size())
        {
            result.push_back(right[r++]); 
        }
        else if (r >= right.size())
        {
            result.push_back(left[l++]);
        }
        else
        {
            int l1 = left[l]->frequency;
            int r1 = right[r]->frequency;
            if (l1 < r1)
            {
                result.push_back(left[l++]);
            }
            else
            {
                result.push_back(right[r++]);
            }
        }
    }
    return result;
}
