


#include <iostream>  
#include <iostream>       
#include <queue>            // для очереди с приоритетом (priority_queue)
#include <unordered_map>    // для хранения частот и кодов символов (hash-таблица)
#include <vector>           // для использования в очереди
#include <string>           // для работы со строками
#include <memory>           // для shared_ptr (умные указатели)
#include <iomanip>          // для форматированного вывода


using namespace std;




class HuffmanEncoder {
    private:

        struct Node {                        // структура узла собственно
            char ch;                         // символ (только для листьев)
            int freq;                        // частота символа или сумма частот поддеревьев
            shared_ptr<Node> left, right;    // указатели на левое и правое поддерево

            Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}                                    // создает листовой узел с символом 'c' и частотой 'f'
            Node(shared_ptr<Node> l, shared_ptr<Node> r) : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}    // создает внутренний узел, соединяя дочерние узлы 'l' и 'r'
        };


        struct Compare {                                                  // нужно для priority_queue, чтобы отсортировать узлы по возрастанию частоты
            bool operator()(shared_ptr<Node> a, shared_ptr<Node> b) {     // компаратор от двух переменных
                return a->freq > b->freq;                                 // если чистота символа в переменной 'a' больше чем в 'b', то TRUE, иначе FALSE
            }
        };


        shared_ptr<Node> root;                      // корень древа
        unordered_map<char, string> huffmanCode;    // хэш-табличка (или по простому словарь) с символами и соотв кодами


        void buildCodeTable(shared_ptr<Node> node, const string& str) {      // рекурсия для построения таблички кодов
            if (!node) return;                                               // если нет потомков, то оставляем как есть
            if (!node->left && !node->right) huffmanCode[node->ch] = str;    // если есть дочерние узлы, то:
            buildCodeTable(node->left, str + "0");                           // слева добавляем 0
            buildCodeTable(node->right, str + "1");                          // справа добавляем 1
        }

    public:

        void build(const string& text) {                                               // построение дерева 
            unordered_map<char, int> freq;                                             // делаем хэш-таблицу для хранения частоты вхождения символов
            for (char ch : text) freq[ch]++;                                           // проходимся по введенному тексту и увеличиваем частоту вхождения символа при встрече с ним

            priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, Compare> pq;    // делаем приоритетную очередь используя компаратор с 34 строки
            for (auto pair : freq) {
                pq.push(make_shared<Node>(pair.first, pair.second));                   // для каждого уникального символа создаем новый узел древа; first – символ, second – частота
            }

            while (pq.size() > 1) {                         // пока не останется один узел, который и будет корнем древа
                auto left = pq.top(); pq.pop();             // извлекаем первый узел с наименьшей частотой
                auto right = pq.top(); pq.pop();            // извлекаем второй узел с наименьшей частотой
                pq.push(make_shared<Node>(left, right));    // объединяем в один новый узел и возвращаем обратно в очередь
            }

            root = pq.top();                                // даем понять, что тот самый один узел и явялется корнем
            buildCodeTable(root, "");                       // с помощью рекурсии из 45 строки строим таблицу кодов
        }


        string encode(const string& text) {    // кодер строки
            string encoded = "";               // пустая строчка для закодированной строки
            for (char ch : text) {             // для каждого символа ->
                encoded += huffmanCode[ch];    // -> вставляем соотв код из словаря (строка 42)
            }
            return encoded;                    // ну и эту строку возвращаем
        }


        string decode(const string& encodedText) {          // декодер строки
            string decoded = "";                            // пустая строчка для декодированной строки
            shared_ptr<Node> current = root;                // вставим указатель на корень древа
            for (char bit : encodedText) {                  // проходочка по каждоу символу закодированной строки (там только 0 и 1)
                if (bit == '0') current = current->left;    // если символ – 0, то идем налево по древу 
                else current = current->right;              // иначе топаем направо 

                if (!current->left && !current->right) {    // если мы наконец уперлись в листовой узел, у которого нет дочерних, то символ, хранящийся в нем ->
                    decoded += current->ch;                 // -> записываем в декодированную строку
                    current = root;                         // и по новой всаем на коревой узел
                }
            }
            return decoded;                                 // и эту строку возвращаем 
        }


        void printCodeTable() {                                                      // метод для красивого отображения таблицы кодов в консоли
            cout << "\nHuffman Code Table:\n";                                       // заголовок таблицы 
            cout << "Char | Code\n";                                                 // заголовки столбцов
            cout << "-------------\n";                                               // для визуального комфорта отделяем
            for (const auto& pair : huffmanCode) {                                   // проходим по всем символам из словаря (строка 42)
                if (pair.first == ' ') cout << "' '  | " << pair.second << endl;     // если символ – пробел, то добавляем кавычки, чтобы его было видно
                else cout << " " << pair.first << "   | " << pair.second << endl;    // инче же просто выводим символ и через разделитель '|' его код
            }
        }


        void evaluateCompression(const string& originalText, const string& encodedText) {    // анализируем эффективность сжатия в % соотношении
            int originalBits = originalText.length() * 8;                                    // исходный размер текста в битах (1 символ = 8 бит)
            int compressedBits = encodedText.length();                                       // размер закодированного текста в битах (1 символ = 1 бит, т.к. только единички и нули)
            double ratio = (double)compressedBits / originalBits * 100;                      // вычисляем процентное соотношение

            cout << fixed << setprecision(2);                                                // устанавливаем фикс на двух знаках после запятой (формат вывода)
            cout << "\nCompression Analysis:\n";                                             // заголовочная строка
            cout << "Original size:   " << originalBits << " bits\n";                        // выводим оригинальный размер
            cout << "Compressed size: " << compressedBits << " bits\n";                      // выводим размер закодированной строки
            cout << "Compression rate: " << ratio << " %\n";                                 // выводим проценты
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
