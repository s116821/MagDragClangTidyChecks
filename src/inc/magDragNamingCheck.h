#ifndef MAGDRAG_NAMING_CHECK_H
#define MAGDRAG_NAMING_CHECK_H

#include <clang-tidy/ClangTidyCheck.h>
#include <clang/AST/Type.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

namespace clang::tidy::naming {

class MagDragNamingCheck : public ClangTidyCheck {
public:
  MagDragNamingCheck(StringRef Name, ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}

  void registerMatchers(ast_matchers::MatchFinder *Finder) override;
  void check(const ast_matchers::MatchFinder::MatchResult &Result) override;

private:
  bool isContainer(QualType Type) const;
  bool isTypedef(QualType Type) const;
  bool isEnum(QualType Type) const;
  bool isClass(QualType Type) const;
};

} // namespace clang::tidy::naming

#endif // MAGDRAG_NAMING_CHECK_H 