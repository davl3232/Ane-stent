#ifndef SCENE_SOFT_OBJECT_H
#define SCENE_SOFT_OBJECT_H

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
  std::shared_ptr<btSoftBody> softBody;

  SceneSoftObject(vtkSmartPointer<vtkActor> actor);
  ~SceneSoftObject();
  void UpdateSoftBody(btSoftBodyWorldInfo &worldInfo, btScalar mass);
  void UpdateVertices(btAlignedObjectArray<btScalar> vertices,
                      btAlignedObjectArray<int> indices)
};

#endif  // !SCENE_SOFT_OBJECT_H