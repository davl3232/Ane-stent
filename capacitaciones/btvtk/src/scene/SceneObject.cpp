#include "SceneObject.h"

SceneObject::SceneObject() {}
SceneObject::SceneObject(vtkSmartPointer<vtkActor> actor,
                         std::shared_ptr<btCollisionShape> collider) {
  this->actor = actor;
  this->collider = collider;
}
SceneObject::~SceneObject() {}