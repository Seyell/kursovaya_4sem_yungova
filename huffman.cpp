#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>

using namespace std;




class HuffmanEncoder {
    private:

        struct Node {
            char ch;
            int freq;
            shared_ptr<Node> left, right;

            Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
            Node(shared_ptr<Node> l, shared_ptr<Node> r) : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
        };


        struct Compare {
            bool operator()(shared_ptr<Node> a, shared_ptr<Node> b) {
                return a->freq > b->freq;
            }
        };


        shared_ptr<Node> root;
        unordered_map<char, string> huffmanCode;


        void buildCodeTable(shared_ptr<Node> node, const string& str) {
            if (!node) return;
            if (!node->left && !node->right) huffmanCode[node->ch] = str;
            buildCodeTable(node->left, str + "0");
            buildCodeTable(node->right, str + "1");
        }

    public:

        void build(const string& text) {
            unordered_map<char, int> freq;
            for (char ch : text) freq[ch]++;

            priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> pq;
            for (auto pair : freq) {
                pq.push(make_shared<Node>(pair.first, pair.second));
            }

            while (pq.size() > 1) {
                auto left = pq.top(); pq.pop();
                auto right = pq.top(); pq.pop();
                pq.push(make_shared<Node>(left, right));
            }

            root = pq.top();
            buildCodeTable(root, "");
        }


        string encode(const string& text) {
            string encoded = "";
            for (char ch : text) {
                encoded += huffmanCode[ch];
            }
            return encoded;
        }


        string decode(const string& encodedText) {
            string decoded = "";
            shared_ptr<Node> current = root;
            for (char bit : encodedText) {
                if (bit == '0') current = current->left;
                else current = current->right;

                if (!current->left && !current->right) {
                    decoded += current->ch;
                    current = root;
                }
            }
            return decoded;
        }


        void printCodeTable() {
            cout << "\nHuffman Code Table:\n";
            cout << "Char | Code\n";
            cout << "-------------\n";
            for (const auto& pair : huffmanCode) {
                if (pair.first == ' ') cout << "' '  | " << pair.second << endl;
                else cout << " " << pair.first << "   | " << pair.second << endl;
            }
        }


        void evaluateCompression(const string& originalText, const string& encodedText) {
            int originalBits = originalText.length() * 8;
            int compressedBits = encodedText.length();
            double ratio = (double)compressedBits / originalBits * 100;

            cout << fixed << setprecision(2);
            cout << "\nCompression Analysis:\n";
            cout << "Original size:   " << originalBits << " bits\n";
            cout << "Compressed size: " << compressedBits << " bits\n";
            cout << "Compression rate: " << ratio << " %\n";
        }
};




int main() {
    while (true) {
    cout << "Huffman Compression Menu\n";
    cout << "  1. Encode text\n";
    cout << "  2. Exit\n";
    cout << "\nChoose an option: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore();
        cout << "Invalid input. Please enter a number.\n";    
        continue;                                        
    }

    cin.ignore();


    switch (choice) {
        case 1: {
            string text;
            cout << "\nEnter text to encode: ";
            getline(cin, text);

            HuffmanEncoder encoder;
            encoder.build(text);
            encoder.printCodeTable();

            string encoded = encoder.encode(text);
            cout << "\nEncoded text: " << encoded << endl;

            string decoded = encoder.decode(encoded);
            cout << "Decoded text: " << decoded << endl;

            encoder.evaluateCompression(text, encoded);

            cout << "\n---------------------------------------------\n";    
            break;
        }

        case 2:
            cout << "Exiting...\n";
            return 0;

        default:
            cout << "Invalid choice. Please try again.\n";    
            break;
    }
}

    return 0;
}
