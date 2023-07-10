#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// Huffman tree node
struct Node {
    char data;
    int frequency;
    Node* left;
    Node* right;

    Node(char data, int frequency, Node* left = nullptr, Node* right = nullptr) {
        this->data = data;
        this->frequency = frequency;
        this->left = left;
        this->right = right;
    }

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
};

// Comparison function for priority queue
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

// Function to build Huffman tree and return the root
Node* buildHuffmanTree(const string& text) {
    // Calculate character frequencies
    unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }

    // Create a priority queue to store nodes
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Create a leaf node for each character and add it to the priority queue
    for (auto& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    // Build the Huffman tree
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        int sumFrequency = left->frequency + right->frequency;
        Node* parent = new Node('\0', sumFrequency, left, right);
        pq.push(parent);
    }

    // Return the root of the Huffman tree
    return pq.top();
}

// Function to encode the text using Huffman coding
void encode(Node* root, string code, unordered_map<char, string>& encoding) {
    if (root == nullptr) {
        return;
    }

    if (root->isLeaf()) {
        encoding[root->data] = code;
    }

    encode(root->left, code + "0", encoding);
    encode(root->right, code + "1", encoding);
}

// Function to compress the text using Huffman coding
string compress(const string& text) {
    // Build the Huffman tree
    Node* root = buildHuffmanTree(text);

    // Generate Huffman codes for each character
    unordered_map<char, string> encoding;
    encode(root, "", encoding);

    // Compress the text using the Huffman codes
    string compressedText;
    for (char c : text) {
        compressedText += encoding[c];
    }

    return compressedText;
}

// Function to decode the compressed text using Huffman coding
string decompress(Node* root, const string& compressedText) {
    string decompressedText;
    Node* current = root;

    for (char bit : compressedText) {
        if (bit == '0') {
            current = current->left;
        } else if (bit == '1') {
            current = current->right;
        }

        if (current->isLeaf()) {
            decompressedText += current->data;
            current = root;
        }
    }

    return decompressedText;
}

int main() {
    string text;
    cout << "Enter the text to compress: ";
    getline(cin, text);

    // Compress the text
    string compressedText = compress(text);
    cout << "Compressed text: " << compressedText << endl;

    // Decompress the text
    Node* root = buildHuffmanTree(text);
    string decompressedText = decompress(root, compressedText);
    cout << "Decompressed text: " << decompressedText << endl;

    return 0;
}
