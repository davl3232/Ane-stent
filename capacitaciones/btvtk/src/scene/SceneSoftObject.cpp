#include "SceneSoftObject.h"

#include <memory>

#include <vtkProperty.h>
#include <vtkTransform.h>

#include "SceneMotionState.h"

SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor,
                                 std::shared_ptr<btCollisionShape> collider,
                                 std::shared_ptr<btSoftBody> softBody) {
  this->actor = actor;
  this->collider = collider;
  this->softBody = softBody;
}