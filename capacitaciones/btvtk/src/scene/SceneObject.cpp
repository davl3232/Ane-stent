#include "SceneObject.h"

#include <memory>

#include <vtkProperty.h>
#include <vtkTransform.h>

#include "SceneMotionState.h"

void SceneObject::UpdateRigidBody(btScalar mass) {
  btVector3 inertia(0, 0, 0);
  this->collider->calculateLocalInertia(mass, inertia);
  this->motionState = std::shared_ptr<btMotionState>(
      new SceneMotionState(shared_from_this(), btTransform::getIdentity()));
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
      mass, this->motionState.get(), this->collider.get(), inertia);
  std::shared_ptr<btRigidBody> rigidBody(new btRigidBody(rigidBodyCI));
  this->rigidBody = rigidBody;
}
SceneObject::SceneObject(vtkSmartPointer<vtkActor> actor,
                         std::shared_ptr<btCollisionShape> collider) {
  this->actor = actor;
  this->collider = collider;
}
SceneObject::SceneObject(vtkSmartPointer<vtkActor> actor,
                         std::shared_ptr<btCollisionShape> collider,
                         std::shared_ptr<btRigidBody> rigidBody) {
  this->actor = actor;
  this->collider = collider;
  this->rigidBody = rigidBody;
}
SceneObject::~SceneObject() {}

void SceneObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  btTransform btTrans;
  this->rigidBody->getMotionState()->getWorldTransform(btTrans);
}