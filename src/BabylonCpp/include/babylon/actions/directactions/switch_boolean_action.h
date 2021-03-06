#ifndef BABYLON_ACTIONS_DIRECTACTIONS_SWITCH_BOOLEAN_ACTION_H
#define BABYLON_ACTIONS_DIRECTACTIONS_SWITCH_BOOLEAN_ACTION_H

#include <babylon/actions/action.h>
#include <babylon/babylon_global.h>

namespace BABYLON {

class BABYLON_SHARED_EXPORT SwitchBooleanAction : public Action {

public:
  SwitchBooleanAction(unsigned int triggerOptions, IAnimatable* target,
                      const std::string& propertyPath,
                      Condition* condition = nullptr);
  ~SwitchBooleanAction();

  void _prepare() override;
  void execute(const ActionEvent& evt) override;
  Json::object serialize(Json::object& parent) const override;

private:
  IAnimatable* _target;
  IAnimatable* _effectiveTarget;
  std::string _property;
  std::string _propertyPath;

}; // end of class SwitchBooleanAction

} // end of namespace BABYLON

#endif // end of BABYLON_ACTIONS_DIRECTACTIONS_SWITCH_BOOLEAN_ACTION_H
