#ifndef INFLATING_CYLINDER_H
#define INFLATING_CYLINDER_H

#include "SceneSoftObject.h"

class InflatingCylinder : SceneSoftObject {
 public:
  virtual void UpdatePhysics(std::chrono::duration<double> deltaTime) override;

 private:
};

#endif  // !INFLATING_CYLINDER_H