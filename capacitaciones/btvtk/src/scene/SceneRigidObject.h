#ifndef SCENE_RIGID_OBJECT_H
#define SCENE_RIGID_OBJECT_H

#include <memory>
#include <chrono>

#include <btBulletDynamicsCommon.h>

#include <vtkSmartPointer.h>
#include <vtkActor.h>

/**
 * @brief Contiene la información necesaria para simular la física de un objeto
 * Bullet y renderizarlo con VTK.
 */
class SceneRigidObject : public std::enable_shared_from_this<SceneRigidObject> {
 public:
  std::string name;
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btCollisionShape> collider;
  std::shared_ptr<btMotionState> motionState;
  std::shared_ptr<btRigidBody> rigidBody;

  SceneRigidObject(vtkSmartPointer<vtkActor> actor,
                   std::shared_ptr<btCollisionShape> collider);
  SceneRigidObject(vtkSmartPointer<vtkActor> actor,
                   std::shared_ptr<btCollisionShape> collider,
                   std::shared_ptr<btRigidBody> rigidBody);

  ~SceneRigidObject();
  void UpdateRigidBody(btScalar mass = 0);
  void UpdatePhysics(std::chrono::duration<double> deltaTime);
};
#endif
