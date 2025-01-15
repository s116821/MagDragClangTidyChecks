#include "inc/magDragNamingCheck.h"
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>

using namespace clang::ast_matchers;

namespace clang::tidy::naming {

void MagDragNamingCheck::registerMatchers(MatchFinder *Finder) {
  // Match variable declarations
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

  // Check if the variable name ends with the correct suffix
  if (isEnum(Type) && !Name.ends_with("__E")) {
    diag(VD->getLocation(), "enum variable should end with '__E'");
  } else if (isContainer(Type) && !Name.ends_with("__C")) {
    diag(VD->getLocation(), "container variable should end with '__C'");
  } else if (isTypedef(Type)) {
    if (isContainer(Type.getDesugaredType(*Result.Context)) && !Name.ends_with("__C")) {
      diag(VD->getLocation(), "typedef of container type should end with '__C'");
    } else if (isClass(Type.getDesugaredType(*Result.Context)) && !Name.ends_with("__O")) {
      diag(VD->getLocation(), "typedef of class type should end with '__O'");
    }
  } else if (isClass(Type) && !Name.ends_with("__O")) {
    diag(VD->getLocation(), "class/struct variable should end with '__O'");
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