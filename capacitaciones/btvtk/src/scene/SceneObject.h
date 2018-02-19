#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <chrono>
#include <memory>
#include <string>

#include <btBulletDynamicsCommon.h>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

class SceneObject {
 public:
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btCollisionShape> collider;
  std::shared_ptr<btRigidBody> rigidBody;

  SceneObject(vtkSmartPointer<vtkActor> actor,
              std::shared_ptr<btCollisionShape> collider);
  SceneObject(vtkSmartPointer<vtkActor> actor,
              std::shared_ptr<btCollisionShape> collider,
              std::shared_ptr<btRigidBody> rigidBody);
  ~SceneObject();
  std::shared_ptr<btRigidBody> SceneObject::UpdateRigidBody(btScalar mass = 1);
  void PhysicsUpdate(std::chrono::duration<double> deltaTime);
};
#endif