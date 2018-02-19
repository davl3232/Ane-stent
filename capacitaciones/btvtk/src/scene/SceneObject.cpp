#include "SceneObject.h"

// std::shared_ptr<btRigidBody> SceneObject::UpdateRigidBody(
//     btScalar mass = 1, btVector3 inertia(0, 0, 0)) {}
std::shared_ptr<btRigidBody> SceneObject::UpdateRigidBody(btScalar mass = 1) {
  btVector3 inertia(0, 0, 0);
  this->collider->calculateLocalInertia(mass, inertia);
  std::shared_ptr<btMotionState> motionState(
      new btDefaultMotionState(btTransform::getIdentity()));
  btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
      mass, motionState.get(), this->collider.get(), inertia);
  std::shared_ptr<btRigidBody> rigidBody(new btRigidBody(rigidBodyCI));
  this->rigidBody = rigidBody;
}
SceneObject::SceneObject(vtkSmartPointer<vtkActor> actor,
                         std::shared_ptr<btCollisionShape> collider) {
  this->actor = actor;
  this->collider = collider;
  this->UpdateRigidBody();
}
SceneObject::SceneObject(vtkSmartPointer<vtkActor> actor,
                         std::shared_ptr<btCollisionShape> collider,
                         std::shared_ptr<btRigidBody> rigidBody) {
  this->actor = actor;
  this->collider = collider;
  this->rigidBody = rigidBody;
}
SceneObject::~SceneObject() {}

void SceneObject::PhysicsUpdate(std::chrono::duration<double> deltaTime) {
  btTransform trans;
  this->rigidBody->getMotionState()->getWorldTransform(trans);

  // aca se pone.
  // coneSource->SetCenter( 0,  trans.getOrigin().getY(), 0);
  // coneSource->Update();
  // mapper->SetInputConnection(coneSource->GetOutputPort());
  // actor->SetMapper(mapper);
  // renderer->AddActor(actor);
  // actor->SetPosition(0.0,trans.getOrigin().getY(),0.0);

  vtkSmartPointer<vtkTransform> transform1a =
      vtkSmartPointer<vtkTransform>::New();
  transform1a->PostMultiply();

  transform1a->Translate(trans.getOrigin().getX(), trans.getOrigin().getY(),
                         trans.getOrigin().getZ());
  // std::cout << "rotation: " << rotado.getAngle() << " " <<
  // rotado.getAxis().getX() << " " << rotado.getAxis().getY() << " " <<
  // rotado.getAxis().getZ() << std::endl;
  // transform1a->RotateWXYZ(std::abs(rotado.getAngle()),std::abs(rotado.getAxis().getX()),std::abs(rotado.getAxis().getY()),std::abs(rotado.getAxis().getZ()));
  transform1a->Update();
  actor->SetUserTransform(transform1a);

  btQuaternion rotado = trans.getRotation();

  double w = rotado.getW();
  double x = rotado.getX();
  double y = rotado.getY();
  double z = rotado.getZ();
  double x1, x2, x3;
  double sqw = w * w;
  double sqx = x * x;
  double sqy = y * y;
  double sqz = z * z;
  x3 = ((atan2(2.0 * (x * y + z * w), (sqx - sqy - sqz + sqw))));
  x1 = ((atan2(2.0 * (y * z + x * w), (-sqx - sqy + sqz + sqw))));
  x2 = ((asin(-2.0 * (x * z - y * w))));

  actor->RotateX(x1);
  actor->RotateY(x2);
  actor->RotateZ(x3);

  renderWindow->Render();
}