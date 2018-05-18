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

  SceneSoftObject();
  SceneSoftObject(vtkSmartPointer<vtkActor> actor);
  ~SceneSoftObject();
  void InitSoftBody(
      btSoftBodyWorldInfo &worldInfo,
      vtkSmartPointer<vtkTransform> = vtkSmartPointer<vtkTransform>::New());
  void UpdateMesh();
  virtual void UpdatePhysics(std::chrono::duration<double> deltaTime);

  btVector3 getCenterOfGeometry();
};

#endif // !SCENE_SOFT_OBJECT_H