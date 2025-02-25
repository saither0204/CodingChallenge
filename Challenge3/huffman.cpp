#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <bitset>

using namespace std;

struct HuffmanNode {
    char ch;               // Character
    int freq;              // Frequency
    HuffmanNode *left, *right;

    HuffmanNode(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }
};

// Custom comparator for the priority queue
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        return a->freq > b->freq;
    }
};
unordered_map<char, int> countFrequencies(const string &filename) {
    unordered_map<char, int> freqMap;
    ifstream file(filename);
    char ch;

    if (!file) {
        cerr << "Error: File not found!\n";
        exit(1);
    }

    while (file.get(ch)) {
        freqMap[ch]++;
    }

    file.close();
    return freqMap;
}
HuffmanNode* buildHuffmanTree(unordered_map<char, int> &freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Create a leaf node for each character and push it into the priority queue
    for (auto pair : freqMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode *left = pq.top(); pq.pop();
        HuffmanNode *right = pq.top(); pq.pop();

        // Merge the two smallest nodes
        HuffmanNode *merged = new HuffmanNode('\0', left->freq + right->freq);
        merged->left = left;
        merged->right = right;

        pq.push(merged);
    }

    return pq.top(); // Root node of the Huffman Tree
}
void generateHuffmanCodes(HuffmanNode* root, string code, unordered_map<char, string> &huffmanCodes) {
    if (!root) return;

    // If it's a leaf node, store the code
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = code;
    }

    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}
string encodeText(const string &filename, unordered_map<char, string> &huffmanCodes) {
    ifstream file(filename);
    string encodedText = "";
    char ch;

    while (file.get(ch)) {
        encodedText += huffmanCodes[ch];
    }

    file.close();
    return encodedText;
}
void writeEncodedFile(const string &outputFile, const string &encodedText) {
    ofstream file(outputFile, ios::binary);
    bitset<8> bitChunk;

    for (size_t i = 0; i < encodedText.size(); i += 8) {
        string byteString = encodedText.substr(i, 8);
        while (byteString.length() < 8) byteString += '0'; // Padding

        bitChunk = bitset<8>(byteString);
        file.put(static_cast<unsigned char>(bitChunk.to_ulong()));
    }

    file.close();
}
string decodeText(HuffmanNode* root, const string &encodedText) {
    string decodedText = "";
    HuffmanNode* currentNode = root;

    for (char bit : encodedText) {
        if (bit == '0') currentNode = currentNode->left;
        else currentNode = currentNode->right;

        if (!currentNode->left && !currentNode->right) {
            decodedText += currentNode->ch;
            currentNode = root;
        }
    }

    return decodedText;
}
string readEncodedFile(const string &inputFile) {
    ifstream file(inputFile, ios::binary);
    string encodedText = "";
    unsigned char byte;

    while (file.get(reinterpret_cast<char&>(byte))) {
        bitset<8> bits(byte);
        encodedText += bits.to_string();
    }

    file.close();
    return encodedText;
}
int main() {
    string inputFile = "test.txt";
    string encodedFile = "encoded.bin";
    string outputFile = "decoded.txt";

    // Step 1: Count frequencies
    unordered_map<char, int> freqMap = countFrequencies(inputFile);

    // Step 2: Build Huffman Tree
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Step 3: Generate Huffman Codes
    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);

    // Step 4: Encode text
    string encodedText = encodeText(inputFile, huffmanCodes);

    // Step 5: Write encoded file
    writeEncodedFile(encodedFile, encodedText);

    // Step 6: Read encoded file
    string compressedData = readEncodedFile(encodedFile);

    // Step 7: Decode text
    string decodedText = decodeText(root, compressedData);

    // Step 8: Write decoded text to output file
    ofstream out(outputFile);
    out << decodedText;
    out.close();

    cout << "Compression and Decompression Completed!\n";
    return 0;
}
