#include "inc/magDragNamingCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "llvm/ADT/StringRef.h"

using namespace clang::ast_matchers;

namespace clang {
namespace tidy {
namespace naming {

void MagDragNamingCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations
  Finder->addMatcher(
      varDecl(unless(hasParent(enumDecl()))).bind("var"),
      this);
}

bool MagDragNamingCheck::checkIsContainer(QualType Type) const {
  const std::string TypeName = Type.getAsString();
  return TypeName.find("vector") != std::string::npos ||
         TypeName.find("map") != std::string::npos ||
         TypeName.find("set") != std::string::npos ||
         TypeName.find("list") != std::string::npos;
}

bool MagDragNamingCheck::checkIsTypedef(QualType Type) const {
  if (const TypedefType *TT = Type->getAs<TypedefType>()) {
    return true;
  }
  return false;
}

bool MagDragNamingCheck::checkIsEnum(QualType Type) const {
  return Type->isEnumeralType();
}

bool MagDragNamingCheck::checkIsClass(QualType Type) const {
  return Type->isClassType() || Type->isStructureType();
}

void MagDragNamingCheck::check(const ast_matchers::MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;

  std::string Name = VD->getNameAsString();
  QualType Type = VD->getType();

  // Check if the variable name ends with the correct suffix
  if (checkIsEnum(Type) && !Name.ends_with("__E")) {
    diag(VD->getLocation(), "enum variable should end with '__E'");
  } else if (checkIsContainer(Type) && !Name.ends_with("__C")) {
    diag(VD->getLocation(), "container variable should end with '__C'");
  } else if (checkIsTypedef(Type)) {
    if (checkIsContainer(Type.getDesugaredType(*Result.Context)) && !Name.ends_with("__C")) {
      diag(VD->getLocation(), "typedef of container type should end with '__C'");
    } else if (checkIsClass(Type.getDesugaredType(*Result.Context)) && !Name.ends_with("__O")) {
      diag(VD->getLocation(), "typedef of class type should end with '__O'");
    }
  } else if (checkIsClass(Type) && !Name.ends_with("__O")) {
    diag(VD->getLocation(), "class/struct variable should end with '__O'");
  }
}

} // namespace naming
} // namespace tidy
} // namespace clang

extern "C" ::clang::tidy::ClangTidyModuleRegistry::Add<clang::tidy::naming::MagDragNamingCheck>
X("magdrag-naming-check", "Checks MagDrag C++ naming conventions");

extern "C" const char clang_tidy_plugin_registry_registration[] = "magdrag-naming-check"; 