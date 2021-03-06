#include <babylon/postprocess/anaglyph_post_process.h>

#include <babylon/cameras/camera.h>
#include <babylon/materials/effect.h>

namespace BABYLON {

AnaglyphPostProcess::AnaglyphPostProcess(const std::string& iName, float ratio,
                                         std::vector<Camera*> rigCameras,
                                         unsigned int samplingMode,
                                         Engine* engine, bool reusable)
    : PostProcess{iName,         "anaglyph",   {},     {"leftSampler"}, ratio,
                  rigCameras[1], samplingMode, engine, reusable}
{
  _passedProcess = rigCameras[0]->_rigPostProcess;

  onApplyObservable.add([&](Effect* effect) {
    effect->setTextureFromPostProcess("leftSampler", _passedProcess);
  });
}

AnaglyphPostProcess::~AnaglyphPostProcess()
{
}

} // end of namespace BABYLON
