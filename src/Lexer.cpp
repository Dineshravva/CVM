#include "Lexer.h"
#include <cctype>
#include <unordered_map>

Lexer::Lexer(const std::string& source) : source(source), start(0), current(0), line(1) {}

bool Lexer::isAtEnd() const { return current >= source.length(); }

char Lexer::advance() { return source[current++]; }

char Lexer::peek() const { return isAtEnd() ? '\0' : source[current]; }

char Lexer::peekNext() const {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

void Lexer::skipWhitespace() {
    while (true) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return;
        }
    }
}

Token Lexer::makeToken(TokenType type) {
    return Token{type, source.substr(start, current - start), line};
}

Token Lexer::errorToken(const std::string& message) {
    return Token{TokenType::ERROR_TOKEN, message, line};
}

Token Lexer::identifierOrKeyword() {
    while (std::isalnum(peek()) || peek() == '_') {
        advance();
    }
    std::string text = source.substr(start, current - start);
    
    TokenType type = TokenType::IDENTIFIER;
    if (text == "let") type = TokenType::LET;
    else if (text == "if") type = TokenType::IF;
    else if (text == "else") type = TokenType::ELSE;
    else if (text == "while") type = TokenType::WHILE;
    else if (text == "print") type = TokenType::PRINT;
    else if (text == "input") type = TokenType::INPUT;
    else if (text == "true") type = TokenType::BOOL_LITERAL;
    else if (text == "false") type = TokenType::BOOL_LITERAL;
    
    return Token{type, text, line};
}

Token Lexer::number() {
    while (std::isdigit(peek())) {
        advance();
    }
    return makeToken(TokenType::INT_LITERAL);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    while (!isAtEnd()) {
        skipWhitespace();
        if (isAtEnd()) break;
        start = current;
        char c = advance();
        
        if (std::isalpha(c) || c == '_') {
            tokens.push_back(identifierOrKeyword());
        } else if (std::isdigit(c)) {
            tokens.push_back(number());
        } else {
            switch (c) {
                case '+': tokens.push_back(makeToken(TokenType::PLUS)); break;
                case '-': tokens.push_back(makeToken(TokenType::MINUS)); break;
                case '*': tokens.push_back(makeToken(TokenType::STAR)); break;
                case '/': tokens.push_back(makeToken(TokenType::SLASH)); break;
                case '(': tokens.push_back(makeToken(TokenType::LPAREN)); break;
                case ')': tokens.push_back(makeToken(TokenType::RPAREN)); break;
                case '{': tokens.push_back(makeToken(TokenType::LBRACE)); break;
                case '}': tokens.push_back(makeToken(TokenType::RBRACE)); break;
                case ';': tokens.push_back(makeToken(TokenType::SEMICOLON)); break;
                case '=':
                    tokens.push_back(match('=') ? makeToken(TokenType::EQ_EQ) : makeToken(TokenType::EQUAL));
                    break;
                case '<': tokens.push_back(makeToken(TokenType::LESS)); break;
                default: tokens.push_back(errorToken("Unexpected character: " + std::string(1, c))); break;
            }
        }
    }
    tokens.push_back(Token{TokenType::END_OF_FILE, "", line});
    return tokens;
}
