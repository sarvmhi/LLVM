#include "Parser.h"

AST *Parser::parse() {
  AST *Res = parseGoal();
  expect(Token::eoi);
  return Res;
}

AST *Parser::parseGoal() {
  AST *Res = parseProgram();
  expect(Token::semicolon);
  while (Tok.is(Token::semicolon)) {
    advance();
    if (Tok.is(Token::eoi))
      break;
    Res = parseProgram();
  }
  return Res;
}

AST *Parser::parseProgram() {
  if (Tok.is(Token::kw_type)) {
    return parseDefine();
  } else {
    return parseEquality();
  }
}

AST *Parser::parseDefine() {
  consume(Token::kw_type);
  if (expect(Token::ident))
    return nullptr;

  llvm::SmallVector<llvm::StringRef, 8> Idents;
  Idents.push_back(Tok.getText());
  advance();

  while (Tok.is(Token::comma)) {
    advance();
    if (expect(Token::ident))
      return nullptr;
    Idents.push_back(Tok.getText());
    advance();
  }

  expect(Token::semicolon);

  return new DefineDecl(Idents);
}

AST *Parser::parseEquality() {
  if (expect(Token::ident))
    return nullptr;

  llvm::StringRef Ident = Tok.getText();
  advance();

  expect(Token::equal);

  Expr *E = parseExpr();

  expect(Token::semicolon);

  return new EqualityDecl(Ident, E);
}

Expr *Parser::parseExpr() {
  Expr *Left = parseTerm();
  while (Tok.isOneOf(Token::plus, Token::minus)) {
    BinaryOp::Operator Op = Tok.is(Token::plus) ? BinaryOp::Plus : BinaryOp::Minus;
    advance();
    Expr *Right = parseTerm();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseTerm() {
  Expr *Left = parseFactor();
  while (Tok.isOneOf(Token::star, Token::slash)) {
    BinaryOp::Operator Op = Tok.is(Token::star) ? BinaryOp::Mul : BinaryOp::Div;
    advance();
    Expr *Right = parseFactor();
    Left = new BinaryOp(Op, Left, Right);
  }
  return Left;
}

Expr *Parser::parseFactor() {
  Expr *Res = nullptr;
  switch (Tok.getKind()) {
    case Token::ident:
      Res = new Factor(Factor::Ident, Tok.getText());
      advance();
      break;
    case Token::number:
      Res = new Factor(Factor::Number, Tok.getText());
      advance();
      break;
    case Token::l_paren:
      advance();
      Res = parseExpr();
      expect(Token::r_paren);
      break;
    default:
      error();
      break;
  }
  return Res;
}
