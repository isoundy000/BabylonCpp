#ifndef BABYLON_TOOLS_OPTIMIZATION_POST_PROCESS_OPTIMIZATION_H
#define BABYLON_TOOLS_OPTIMIZATION_POST_PROCESS_OPTIMIZATION_H

#include <babylon/babylon_global.h>
#include <babylon/tools/optimization/scene_optimization.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT PostProcessesOptimization
  : public SceneOptimization {

public:
  PostProcessesOptimization(int priority = 0);
  ~PostProcessesOptimization();

  bool apply(Scene* scene) override;

}; // end of class PostProcessesOptimization

} // end of namespace BABYLON

#endif // end of BABYLON_TOOLS_OPTIMIZATION_POST_PROCESS_OPTIMIZATION_H