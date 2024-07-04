#include <iostream> 
#include <cctype> 
#include <vector> 
#include <string> 
using namespace std;


vector<vector<char>> generateKeyTable(const string& key) {
    vector<vector<char>> table(6, vector<char>(6, ' '));
    string uniqueKey = "";

    
    for (char ch : key) {
        if (isalnum(ch) && uniqueKey.find(toupper(ch)) == string::npos) {
            uniqueKey += toupper(ch);
        }
    }

    
    int i = 0, j = 0;
    for (char ch : uniqueKey) {
        table[i][j++] = ch;
        if (j == 6) {
            j = 0;
            i++;
        }
    }
 
    string remainingChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (char ch : remainingChars) {
        if (uniqueKey.find(ch) == string::npos) {
            table[i][j++] = ch;
            if (j == 6) {
                j = 0;
                i++;
            }
        }
    }

    return table;
}

void findPositions(const vector<vector<char>>& table, char letter, int& row, int& col) {
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            if (table[i][j] == letter) {
                row = i;
                col = j;
                return;
            }
        }
    }
}

string encryptPair(const vector<vector<char>>& table, char first, char second) {
    int row1, col1, row2, col2;
    findPositions(table, toupper(first), row1, col1);
    findPositions(table, toupper(second), row2, col2);

    if (row1 == row2) {
        return string(1, table[row1][(col1 + 1) % 6]) + table[row2][(col2 + 1) % 6];
    }
    else if (col1 == col2) {
        return string(1, table[(row1 + 1) % 6][col1]) + table[(row2 + 1) % 6][col2];
    }
    else {
        return string(1, table[row1][col2]) + table[row2][col1];
    }
}

string preprocessText(const string& plaintext) {
    string cleanedText;

    for (char ch : plaintext) {
        if (isalnum(ch)) {  
            cleanedText += toupper(ch);
        }
    }

    for (size_t i = 1; i < cleanedText.length(); i += 2) {
        if (cleanedText[i] == cleanedText[i - 1]) {
            cleanedText.insert(i, 1, 'X');
        }
    }

    if (cleanedText.length() % 2 != 0) {
        cleanedText += 'X';
    }

    return cleanedText;
}

string playfairEncrypt(const string& plaintext, const string& key) {
    vector<vector<char>> table = generateKeyTable(key);

    string cleanedPlaintext = preprocessText(plaintext);

    cout << "Parted Plaintext: ";
    for (size_t i = 0; i < cleanedPlaintext.length(); i += 2, cout << " ")
        cout << cleanedPlaintext[i] << (i + 1 < cleanedPlaintext.length() ? cleanedPlaintext[i + 1] : ' ');

    cout << "\nCiphertext: ";

    string ciphertext;
    for (size_t i = 0; i < cleanedPlaintext.length(); i += 2) {
        ciphertext += encryptPair(table, cleanedPlaintext[i], cleanedPlaintext[i + 1]);
    }

    return ciphertext;
}
void displayKeyTable(const vector<vector<char>>& table) {
    for (const auto& row : table) {
        for (char ch : row) {
            cout << ch << " ";
        }
        cout << endl;
    }
}

int main() {
    string key, plaintext;

    cout << "Enter the key: ";
    cin >> key;

    cout << "Enter the plaintext: ";
    cin.ignore(); 
    getline(cin, plaintext);

    vector<vector<char>> table = generateKeyTable(key);

    cout << "The Playfair cipher key table: " << endl;
    displayKeyTable(table);

    string ciphertext = playfairEncrypt(plaintext, key);

    cout << ciphertext << endl;

    return 0;
}