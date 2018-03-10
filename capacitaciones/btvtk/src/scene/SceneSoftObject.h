#ifndef SCENE_SOFT_OBJECT_H
#define SCENE_SOFT_OBJECT_H

#include <string>
#include <memory>

#include "BulletSoftBody/btSoftBody.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>

class SceneSoftObject {
 public:
  std::string name;
  vtkSmartPointer<vtkActor> actor;
  std::shared_ptr<btSoftBody> softBody;

  SceneSoftObject(vtkSmartPointer<vtkActor> actor);
  ~SceneSoftObject();
  void UpdateSoftBody(btSoftBodyWorldInfo &worldInfo);
  void UpdateMesh();
};

#endif  // !SCENE_SOFT_OBJECT_H
