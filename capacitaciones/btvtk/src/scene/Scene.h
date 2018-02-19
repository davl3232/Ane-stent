#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <vector>

#include "SceneObject.h"

class Scene {
 public:
  double backgroundColor[3];
  std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
  std::vector<std::shared_ptr<SceneObject>> objects;

  Scene();
  ~Scene();
  void Init();
  void InitPhysics();
  void InitGraphics();
  void Loop();
  void Update(std::chrono::duration<double> deltaTime);
  void PhysicsUpdate(std::chrono::duration<double> deltaTime);
  void SetBackgroundColor(double r, double g, double b);
}

#endif  // !SCENE_H
