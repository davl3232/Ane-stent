#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <chrono>
#include <memory>
#include <string>

#include <btBulletDynamicsCommon.h>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

/**
 * @brief Contiene la información necesaria para simular la física de un objeto
 * Bullet y renderizarlo con VTK.
 */
class SceneObject : public std::enable_shared_from_this<SceneObject> {
 public:
  std::string name;
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btCollisionShape> collider;
  std::shared_ptr<btMotionState> motionState;
  std::shared_ptr<btRigidBody> rigidBody;

  SceneObject(vtkSmartPointer<vtkActor> actor,
              std::shared_ptr<btCollisionShape> collider);
  SceneObject(vtkSmartPointer<vtkActor> actor,
              std::shared_ptr<btCollisionShape> collider,
              std::shared_ptr<btRigidBody> rigidBody);
  ~SceneObject();
  void UpdateRigidBody(btScalar mass = 0);
  void UpdatePhysics(std::chrono::duration<double> deltaTime);
};
#endif