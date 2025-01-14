#ifndef MAGDRAG_CLANG_TIDY_NAMING_CHECK_H
#define MAGDRAG_CLANG_TIDY_NAMING_CHECK_H

#include "clang-tidy/ClangTidy.h"

namespace clang {
namespace tidy {
namespace naming {

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

} // namespace naming
} // namespace tidy
} // namespace clang

#endif 