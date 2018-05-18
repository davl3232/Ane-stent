#ifndef INFLATING_OBJECT_H
#define INFLATING_OBJECT_H

#include "SceneSoftObject.h"

class InflatingObject : public SceneSoftObject {
public:
  double dPR = 0.0;
  virtual void UpdatePhysics(std::chrono::duration<double> deltaTime) override;

private:
};

#endif