#include "inc/magDragNamingCheck.h"
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/AST/Type.h>

using namespace clang::ast_matchers;

namespace clang::tidy::naming {

void MagDragNamingCheck::registerMatchers(MatchFinder *Finder) {
  // Only match variable declarations for our custom suffix rules
  Finder->addMatcher(
      varDecl(unless(hasParent(enumDecl()))).bind("var"),
      this);
}

bool MagDragNamingCheck::isContainer(QualType Type) const {
  const std::string TypeName = Type.getAsString();
  return TypeName.find("vector") != std::string::npos ||
         TypeName.find("map") != std::string::npos ||
         TypeName.find("set") != std::string::npos ||
         TypeName.find("list") != std::string::npos;
}

bool MagDragNamingCheck::isTypedef(QualType Type) const {
  if (const TypedefType *TT = Type->getAs<TypedefType>()) {
    return true;
  }
  return false;
}

bool MagDragNamingCheck::isEnum(QualType Type) const {
  return Type->isEnumeralType();
}

bool MagDragNamingCheck::isClass(QualType Type) const {
  return Type->isClassType() || Type->isStructureType();
}

void MagDragNamingCheck::check(const MatchFinder::MatchResult &Result) {
  const auto *VD = Result.Nodes.getNodeAs<VarDecl>("var");
  if (!VD)
    return;

  std::string Name = VD->getNameAsString();
  QualType Type = VD->getType();

  // We don't need case conversion since built-in checks handle that
  // We only need to check for our special suffixes

  // Check class/struct member variables
  #if LLVM_VERSION_MAJOR >= 10
    if (VD->isCXXClassMember() && !Name.ends_with("__O")) {
  #else
    if (VD->isMemberVar() && !Name.ends_with("__O")) {
  #endif
    std::string suggestedName = Name + "__O";
    diag(VD->getLocation(), "class/struct member variable must end with '__O'")
        << FixItHint::CreateReplacement(VD->getLocation(), suggestedName);
  }
  // Check container variables
  else if (isContainer(Type) && !Name.ends_with("__C")) {
    std::string suggestedName = Name + "__C";
    diag(VD->getLocation(), "container variable must end with '__C'")
        << FixItHint::CreateReplacement(VD->getLocation(), suggestedName);
  }
  // Check enum variables
  else if (isEnum(Type) && !Name.ends_with("__E")) {
    std::string suggestedName = Name + "__E";
    diag(VD->getLocation(), "enum variable must end with '__E'")
        << FixItHint::CreateReplacement(VD->getLocation(), suggestedName);
  }
  // Check typedef variables
  else if (isTypedef(Type)) {
    const clang::Type* UnderlyingType = Type.getDesugaredType(*Result.Context).getTypePtr();
    if (isContainer(QualType(UnderlyingType, 0)) && !Name.ends_with("__C")) {
      std::string suggestedName = Name + "__C";
      diag(VD->getLocation(), "typedef of container type must end with '__C'")
          << FixItHint::CreateReplacement(VD->getLocation(), suggestedName);
    }
    else if (UnderlyingType->isClassType() && !Name.ends_with("__O")) {
      std::string suggestedName = Name + "__O";
      diag(VD->getLocation(), "typedef of class type must end with '__O'")
          << FixItHint::CreateReplacement(VD->getLocation(), suggestedName);
    }
  }
}

// Create a module class
class MagDragModule : public ClangTidyModule {
public:
    void addCheckFactories(ClangTidyCheckFactories &Factories) override {
        Factories.registerCheck<MagDragNamingCheck>(
            "magdrag-naming-check");
    }
};

} // namespace clang::tidy::naming

// Register the module
static clang::tidy::ClangTidyModuleRegistry::Add<clang::tidy::naming::MagDragModule>
    X("magdrag", "Adds MagDrag naming convention checks."); 