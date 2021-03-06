#ifndef BABYLON_CULLING_OCTREES_IOCTREE_CONTAINER_H
#define BABYLON_CULLING_OCTREES_IOCTREE_CONTAINER_H

#include <babylon/babylon_global.h>

namespace BABYLON {

template <class T>
struct BABYLON_SHARED_EXPORT IOctreeContainer {
  static std::vector<OctreeBlock<T>> blocks;
}; // end of struct IOctreeContainer

template <class T>
std::vector<OctreeBlock<T>> IOctreeContainer<T>::blocks;

} // end of namespace BABYLON

#endif // end of BABYLON_CULLING_OCTREES_IOCTREE_CONTAINER_H
