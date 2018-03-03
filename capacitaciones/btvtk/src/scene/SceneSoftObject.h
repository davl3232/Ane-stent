#ifndef SCENE_SOFT_OBJECT_H
#define SCENE_SOFT_OBJECT_H

#include "SceneRigidObject.h"

#include <chrono>
#include <memory>
#include <string>

#include <BulletSoftBody/btSoftBody.h>
#include <btBulletDynamicsCommon.h>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

class SceneSoftObject {
 public:
  std::string name;
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btCollisionShape> collider;
  std::shared_ptr<btMotionState> motionState;
  std::shared_ptr<btSoftBody> softBody;

  SceneSoftObject(vtkSmartPointer<vtkActor> actor,
                  std::shared_ptr<btCollisionShape> collider,
                  std::shared_ptr<btSoftBody> softBody);
  ~SceneSoftObject();
};

#endif  // !SCENE_SOFT_OBJECT_H