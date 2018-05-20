#include "SceneRigidObject.h"

#include <memory>

#include <vtkMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "RigidMotionState.h"

void SceneRigidObject::InitRigidBody(btScalar mass,
                                     vtkSmartPointer<vtkTransform> transform) {
  btVector3 inertia(0, 0, 0);
  this->collider->calculateLocalInertia(mass, inertia);
  this->motionState =
      std::shared_ptr<btMotionState>(new RigidMotionState(shared_from_this()));
  // this->motionState = std::shared_ptr<btMotionState>(
  //     new btDefaultMotionState(btTransform::getIdentity()));
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

void SceneRigidObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {}

btVector3 SceneRigidObject::GetCenterOfGeometry() {
  btVector3 centerOfGeometry(0, 0, 0);
  vtkSmartPointer<vtkPolyData> polyData =
      vtkPolyData::SafeDownCast(this->actor->GetMapper()->GetInputAsDataSet());
  vtkIdType numVerts = polyData->GetNumberOfPoints();
  vtkSmartPointer<vtkPoints> oldpts = polyData->GetPoints();
  for (vtkIdType i = 0; i < numVerts; i++) {
    double pk[3];
    oldpts->GetPoint(i, pk);
    btVector3 p(pk[0], pk[1], pk[2]);
    centerOfGeometry += p;
  }
  return centerOfGeometry / double(numVerts);
}