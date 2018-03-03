#include "SceneRigidObject.h"

#include <memory>

#include <vtkProperty.h>
#include <vtkTransform.h>

#include "RigidMotionState.h"

void SceneRigidObject::UpdateRigidBody(btScalar mass) {
  btVector3 inertia(0, 0, 0);
  this->collider->calculateLocalInertia(mass, inertia);
  this->motionState = std::shared_ptr<btMotionState>(
      new RigidMotionState(shared_from_this(), btTransform::getIdentity()));
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
      mass, this->motionState.get(), this->collider.get(), inertia);
  std::shared_ptr<btRigidBody> rigidBody(new btRigidBody(rigidBodyCI));
  this->rigidBody = rigidBody;
}

SceneRigidObject::SceneRigidObject(vtkSmartPointer<vtkActor> actor,
                                   std::shared_ptr<btCollisionShape> collider) {
  this->actor = actor;
  this->collider = collider;
}
SceneRigidObject::SceneRigidObject(vtkSmartPointer<vtkActor> actor,
                                   std::shared_ptr<btCollisionShape> collider,
                                   std::shared_ptr<btRigidBody> rigidBody) {
  this->actor = actor;
  this->collider = collider;
  this->rigidBody = rigidBody;
}

SceneRigidObject::~SceneRigidObject() {}

void SceneRigidObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  btTransform btTrans;
  this->rigidBody->getMotionState()->getWorldTransform(btTrans);
}