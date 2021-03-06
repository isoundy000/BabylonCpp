#ifndef BABYLON_ENGINE_POINTER_INFO_PRE_H
#define BABYLON_ENGINE_POINTER_INFO_PRE_H

#include <babylon/babylon_global.h>
#include <babylon/engine/pointer_info_base.h>

namespace BABYLON {

/**
 * @brief This class is used to store pointer related info for the
 * onPrePointerObservable event.
 * Set the skipOnPointerObservable property to true if you want the engine
 * to stop any process after this event is triggered, even not calling
 * onPointerObservable
 */
class BABYLON_SHARED_EXPORT PointerInfoPre : public PointerInfoBase {

public:
  PointerInfoPre(unsigned int type, const PointerEvent& event, float localX,
                 float localY);
  PointerInfoPre(unsigned int type, const MouseWheelEvent& event, float localX,
                 float localY);
  ~PointerInfoPre();

public:
  Vector2 localPosition;
  bool skipOnPointerObservable;

}; // end of struct PointerInfoPre

} // end of namespace BABYLON

#endif // end of BABYLON_ENGINE_POINTER_INFO_PRE_H
