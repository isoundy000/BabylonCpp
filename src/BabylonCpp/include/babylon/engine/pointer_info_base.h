#ifndef BABYLON_ENGINE_POINTER_INFO_BASE_H
#define BABYLON_ENGINE_POINTER_INFO_BASE_H

#include <babylon/babylon_global.h>
#include <babylon/core/structs.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT PointerInfoBase {

public:
  PointerInfoBase(unsigned int type, const PointerEvent& event);
  PointerInfoBase(unsigned int type, const MouseWheelEvent& event);
  ~PointerInfoBase();

public:
  unsigned int type;
  PointerEvent pointerEvent;
  MouseWheelEvent mouseWheelEvent;

}; // end of struct PointerInfoBase

} // end of namespace BABYLON

#endif // end of BABYLON_ENGINE_POINTER_INFO_BASE_H
