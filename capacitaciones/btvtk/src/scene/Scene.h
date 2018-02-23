#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include <vtkRenderWindowInteractor.h>

#include "SceneObject.h"

class Scene {
 private:
  std::vector<std::shared_ptr<SceneObject>> objects;

 public:
  std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

  Scene();
  ~Scene();
  void Init();
  void InitPhysics();
  void InitGraphics();
  void Loop();
  void Update(std::chrono::duration<double> deltaTime);
  void UpdatePhysics(std::chrono::duration<double> deltaTime);
  void AddObject(std::shared_ptr<SceneObject> object);
  std::vector<std::shared_ptr<SceneObject>> GetObjects();
  void SetBackgroundColor(double r, double g, double b);
};
#endif
