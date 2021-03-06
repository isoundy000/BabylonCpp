#ifndef BABYLON_MATERIALS_LIBRARY_GRID_MATERIAL_DEFINES_H
#define BABYLON_MATERIALS_LIBRARY_GRID_MATERIAL_DEFINES_H

#include <babylon/babylon_global.h>
#include <babylon/materials/material_defines.h>

namespace BABYLON {
namespace MaterialsLibrary {

struct BABYLON_SHARED_EXPORT GridMaterialDefines : public MaterialDefines {

  static constexpr unsigned int TRANSPARENT      = 0;
  static constexpr unsigned int FOG              = 1;
  static constexpr unsigned int LOGARITHMICDEPTH = 2;
  static constexpr unsigned int POINTSIZE        = 3;

  GridMaterialDefines();
  ~GridMaterialDefines();

}; // end of struct GridMaterialDefines

} // end of namespace MaterialsLibrary
} // end of namespace BABYLON

#endif // end of BABYLON_MATERIALS_LIBRARY_GRID_MATERIAL_DEFINES_H
