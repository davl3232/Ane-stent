#ifndef SCENE_SOFT_OBJECT_H
#define SCENE_SOFT_OBJECT_H

#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include <BulletSoftBody/btSoftBody.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <LinearMath/btVector3.h>
#include <btBulletDynamicsCommon.h>

#include <vtkActor.h>
#include <vtkSmartPointer.h>

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