#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Lexer;

class Token {

 friend class Lexer;
public:
 enum TokenKind : unsigned short {
 eoi, unknown, ident, number, comma, semicolon, plus, 
 minus, star, slash, l_paren, r_paren, KW_type , KW_int , assign
 
 };

 private:
  TokenKind Kind;
  llvm::StringRef Text;

  public:
  TokenKind getKind() const { return Kind; }
  llvm::StringRef getText() const {
    return Text;
  }


class Lexer {
  const char *BufferStart;
  const char *BufferPtr;

public:
  Lexer(const llvm::StringRef &Buffer) {
    BufferStart = Buffer.begin();
    BufferPtr = BufferStart;
  }

   void next(Token &token);

  private:
  void formToken(Token &Result, const char *TokEnd,
                 Token::TokenKind Kind);

};
#endif


  
