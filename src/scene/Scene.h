#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include <vtkAssembly.h>
#include <vtkRenderWindowInteractor.h>

#include "SceneRigidObject.h"
#include "SceneSoftObject.h"
class Scene {
private:
  std::vector<std::shared_ptr<SceneRigidObject>> rigidObjects;
  std::vector<std::shared_ptr<SceneSoftObject>> softObjects;

public:
  // int frameNum = 0;
  btSoftBodyWorldInfo softBodyWorldInfo;
  std::shared_ptr<btSoftRigidDynamicsWorld> dynamicsWorld;
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

  Scene();
  ~Scene();
  void Init();
  void InitPhysics();
  void InitGraphics();
  void Loop();
  void Update(std::chrono::duration<double> deltaTime);
  void UpdatePhysics(std::chrono::duration<double> deltaTime);
  void DebugNormals();
  void AddRigidObject(std::shared_ptr<SceneRigidObject> object);
  void AddSoftObject(std::shared_ptr<SceneSoftObject> object);
  void SetBackgroundColor(double r, double g, double b);
};
#endif
