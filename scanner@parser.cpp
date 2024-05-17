#include <iostream> 
#include <string> 
#include <unordered_map> 
#include <vector> 
 
using namespace std; 
 
// Maximum number of tokens 
const int MAX_TOKENS = 100; 
 
// Token types enumeration 
enum class TokenType { 
    RESERVED_WORD, 
    IDENTIFIER, 
    NUMBER, 
    OPERATOR, 
    DELIMITER, 
    DATA_TYPE, 
    LOGICAL_OPERATOR, 
    SEMICOLON, 
    INCREMENTAL, 
    DECREMENTAL, 
    ASSIGNMENT_OP, 
    COMPARISON_OP, 
    UNKNOWN 
}; 
 
// Token structure to hold token information 
struct Token { 
    string lexeme; 
    TokenType type; 
}; 
 
// Function to check if a string is a reserved word 
bool isReservedWord(const string& word) { 
    // List of reserved words 
    unordered_map<string, bool> reservedWords = { 
        {"if", true}, 
        {"else", true}, 
        {"then", true}, 
        {"for", true}, 
        {"do", true}, 
        {"while", true}, 
        {"read", true},  // Added "read" as a reserved word 
        {"print", true}, // Added "print" as a reserved word 
        // Add more reserved words here 
    }; 
    return reservedWords.count(word) > 0; 
} 
 
// Function to check if a string is a data type 
bool isDataType(const string& word) { 
    unordered_map<string, bool> dataTypes = { 
        {"int", true}, 
        {"char", true}, 
        {"string", true}, 
        {"float", true}, 
        // Add more data types here 
    }; 
    return dataTypes.count(word) > 0; 
} 
 
// Function to check if a string is a logical operator 
bool isLogicalOperator(const string& op) { 
    return (op == "&&" || op == "||" || op == "!"); 
} 
 
// Function to determine the token type 
TokenType getTokenType(const string& lexeme) { 
    if (isReservedWord(lexeme)) { 
        return TokenType::RESERVED_WORD; 
    } 
    else if (isDataType(lexeme)) { 
        return TokenType::DATA_TYPE; 
    } 
    else if (isLogicalOperator(lexeme)) { 
        return TokenType::LOGICAL_OPERATOR; 
    } 
    else if (isdigit(lexeme[0])) { 
        return TokenType::NUMBER; 
    } 
    else if (isalpha(lexeme[0])) { 
        return TokenType::IDENTIFIER; 
    } 
    else { 
        // Assuming operators and delimiters are single characters 
        switch (lexeme[0]) { 
        case '>': 
        case '<': 
        case '!': 
            if (lexeme.size() == 1) 
                return TokenType::OPERATOR; 
            else if (lexeme == "==") 
                return TokenType::COMPARISON_OP; 
            else if (lexeme == ">=" || lexeme == "<=") 
                return TokenType::COMPARISON_OP; 
            else 
                return TokenType::UNKNOWN; 
        case '=': 
            if (lexeme.size() == 1) 
                return TokenType::ASSIGNMENT_OP; 
            else 
                return TokenType::UNKNOWN; 
        case '(': 
        case ')': 
        case '{': 
        case '}': 
            return TokenType::DELIMITER; 
        case ';': 
            return TokenType::SEMICOLON; 
        case '+': 
            return TokenType::INCREMENTAL; 
        case '-': 
            return TokenType::DECREMENTAL; 
        default: 
            return TokenType::UNKNOWN; 
        } 
    } 
} 
 
// Function to perform lexical analysis 
int scanner(const string& input, Token tokens[]) { 
    int tokenCount = 0; 
    string currentToken; 
 
    for (char c : input) { 
        // Check if character is a delimiter 
        if (isspace(c) || c == '(' || c == ')' || c == '{' || c == '}' || c == ';') { 
            // If current token is not empty, process it 
            if (!currentToken.empty()) { 
                tokens[tokenCount++] = { currentToken, getTokenType(currentToken) }; 
                // Reset current token 
                currentToken.clear(); 
            } 
            // Add delimiter token 
            if (c != ' ') 
                tokens[tokenCount++] = { string(1, c), getTokenType(string(1, c)) }; 
        } 
        else { 
            // Append non-delimiter characters to current token 
            currentToken += c; 
        } 
    } 
 
    // Process the last token 
    if (!currentToken.empty()) { 
        tokens[tokenCount++] = { currentToken, getTokenType(currentToken) }; 
    } 
 
    return tokenCount; 
} 
 
// Function to perform syntax analysis 
string syntaxAnalyzer(const vector<Token>& tokens) { 
    // Flags to track grammar 
    bool ifFound = false; 
    bool conditionFound = false; 
    bool thenFound = false; 
    bool statementFound = false; 
 
    for (size_t i = 0; i < tokens.size(); ++i) { 
        const Token& token = tokens[i]; 
 
        if (token.type == TokenType::RESERVED_WORD) { 
            if (token.lexeme == "if") { 
                ifFound = true; 
            } 
            else if (ifFound && token.lexeme == "then") { 
                if (!conditionFound) 
                    return "Error: Condition missing after 'if'"; 
                thenFound = true; 
                // Check if a statement follows 'then' 
                if (i == tokens.size() - 1 || tokens[i + 1].type != TokenType::RESERVED_WORD) 
                    return "Error: Statement missing after 'then'"; 
            } 
        } 
        else if (ifFound && !conditionFound && token.type == TokenType::DELIMITER) { 
            if (token.lexeme == "(") 
                conditionFound = true; 
        } 
        else if (thenFound && token.type != TokenType::RESERVED_WORD) { 
            // Check if a statement follows 'then' 
            statementFound = true; 
        } 
    } 
 
    if (ifFound && !conditionFound) 
        return "Error: Condition missing after 'if'"; 
    if (!thenFound && conditionFound) 
        return "Error: 'then' statement missing after condition"; 
    if (!statementFound && thenFound) 
        return "Error: Statement missing after 'then'"; 
 
    return "Syntax analysis passed"; 
} 
 
int main() { 
    cout << "Enter an equation: "; 
    string input; 
    getline(cin, input); 
 
    // Array to store tokens 
    Token tokens[MAX_TOKENS]; 
 
    // Perform lexical analysis 
    int tokenCount = scanner(input, tokens); 
 
    // Vector to store tokens for syntax analysis 
    vector<Token> tokensVector(tokens, tokens + tokenCount); 
 
    // Perform syntax analysis 
    string syntaxResult = syntaxAnalyzer(tokensVector); 
    if (syntaxResult != "Syntax analysis passed") { 
        cout << syntaxResult << endl; 
    } 
    else { 
        // Output token information 
        cout << "Tokenized equation:\n"; 
        for (int i = 0; i < tokenCount; ++i) { 
            cout << "Lexeme: " << tokens[i].lexeme << ", Type: "; 
            switch (tokens[i].type) { 
            case TokenType::RESERVED_WORD: 
                cout << "Reserved Word"; 
                break; 
            case TokenType::IDENTIFIER: 
                cout << "Identifier"; 
                break; 
            case TokenType::NUMBER: 
                cout << "Number"; 
                break; 
            case TokenType::OPERATOR: 
                cout << "Operator"; 
                break; 
            case TokenType::DELIMITER: 
                cout << "Delimiter"; 
                break; 
            case TokenType::DATA_TYPE: 
                cout << "Data Type"; 
                break; 
            case TokenType::LOGICAL_OPERATOR: 
                cout << "Logical Operator"; 
                break; 
            case TokenType::SEMICOLON: 
                cout << "Semicolon"; 
                break; 
            case TokenType::INCREMENTAL: 
                cout << "Incremental"; 
                break; 
            case TokenType::DECREMENTAL: 
                cout << "Decremental"; 
                break; 
            case TokenType::ASSIGNMENT_OP: 
                cout << "Assignment Operator"; 
                break; 
            case TokenType::COMPARISON_OP: 
                cout << "Comparison Operator"; 
                break; 
            case TokenType::UNKNOWN: 
                cout << "Unknown"; 
                break; 
            } 
            cout << endl; 
        } 
    } 
 
    return 0; 
} 
 
 
 
 
 