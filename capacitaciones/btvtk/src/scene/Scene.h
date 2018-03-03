#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <btBulletDynamicsCommon.h>

#include <vtkRenderWindowInteractor.h>
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include "SceneObject.h"
#include "SceneSoftObject.h"
class Scene {
 private:
  std::vector<std::shared_ptr<SceneObject>> objects;
   std::vector<std::shared_ptr<SceneSoftObject>> softObjects;
 public:
  std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
  std::shared_ptr<btSoftRigidDynamicsWorld> dynamicsWorldAux;
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

  Scene();
  ~Scene();
  void Init();
  void InitPhysics();
  void InitGraphics();
  void Loop();
  void Update(std::chrono::duration<double> deltaTime);
  void UpdatePhysics(std::chrono::duration<double> deltaTime);
  void AddRigidObject(std::shared_ptr<SceneObject> object);
  void AddSoftObject(std::shared_ptr<SceneSoftObject> object);
  std::vector<std::shared_ptr<SceneObject>> GetObjects();
  //void AddObject(std::shared_ptr<SceneObject> object);
  std::vector<std::shared_ptr<SceneObject>> GetRigidObjects();
  void SetBackgroundColor(double r, double g, double b);
};
#endif
