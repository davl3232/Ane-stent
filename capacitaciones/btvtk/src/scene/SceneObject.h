#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <memory>
#include <string>

#include <btBulletDynamicsCommon.h>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

class SceneObject {
public:
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btCollisionShape> collider;
  SceneObject();
  SceneObject(vtkSmartPointer<vtkActor> actor,
              std::shared_ptr<btCollisionShape> collider);
  ~SceneObject();
};
#endif