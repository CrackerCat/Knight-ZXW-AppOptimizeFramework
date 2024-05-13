#include "class_linker.h"
#include "art_xdl.h"
#include "xdl.h"
#include "art_runtime.h"
#include "offsets.h"
namespace art {

void VisitClasses(void *class_linker, void *visitor) {
  void *classLinker =
      ArtRuntime::Get()->GetClassLinker();
  auto visitClasses = (void (*)(void *,void *)) xdl_dsym(open_art_handle(), "_ZN3art11ClassLinker12VisitClassesEPNS_12ClassVisitorE", nullptr);
  visitClasses(class_linker,visitor);
}
}