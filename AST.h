#ifndef AST_H
#define AST_H

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST;
class Expr;
class Factor;
class BinaryOp;
class DefineDecl;
class EqualityDecl;

class ASTVisitor {
public:
  virtual void visit(AST &) {}
  virtual void visit(Expr &) {}
  virtual void visit(Factor &) = 0;
  virtual void visit(BinaryOp &) = 0;
  virtual void visit(DefineDecl &) = 0;
  virtual void visit(EqualityDecl &) = 0;
};

class AST {
public:
  virtual ~AST() {}
  virtual void accept(ASTVisitor &V) = 0;
};

class Expr : public AST {
public:
  Expr() {}
};

class Factor : public Expr {
public:
  enum ValueKind { Ident, Number };

private:
  ValueKind Kind;
  llvm::StringRef Val;

public:
  Factor(ValueKind Kind, llvm::StringRef Val)
      : Kind(Kind), Val(Val) {}
  ValueKind getKind() { return Kind; }
  llvm::StringRef getVal() { return Val; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class BinaryOp : public Expr {
public:
  enum Operator { Plus, Minus, Mul, Div };

private:
  Expr *Left;
  Expr *Right;
  Operator Op;

public:
  BinaryOp(Operator Op, Expr *L, Expr *R)
      : Op(Op), Left(L), Right(R) {}
  Expr *getLeft() { return Left; }
  Expr *getRight() { return Right; }
  Operator getOperator() { return Op; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class DefineDecl : public AST {
  using IdentVector = llvm::SmallVector<llvm::StringRef, 8>;
  IdentVector Idents;

public:
  DefineDecl(llvm::SmallVector<llvm::StringRef, 8> Idents)
      : Idents(Idents) {}
  IdentVector::const_iterator begin() { return Idents.begin(); }
  IdentVector::const_iterator end() { return Idents.end(); }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

class EqualityDecl : public AST {
  llvm::StringRef Ident;
  Expr *E;

public:
  EqualityDecl(llvm::StringRef Ident, Expr *E)
      : Ident(Ident), E(E) {}
  llvm::StringRef getIdentifier() { return Ident; }
  Expr *getExpression() { return E; }
  virtual void accept(ASTVisitor &V) override {
    V.visit(*this);
  }
};

#endif
