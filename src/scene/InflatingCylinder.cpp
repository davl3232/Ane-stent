#include "InflatingCylinder.h"

void InflatingCylinder::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  double dPR = 100.0;
  this->softBody->m_cfg.kPR += dPR;
  std::cout << this->softBody->m_cfg.kPR << std::endl;
}