#include "InflatingObject.h"

void InflatingObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  this->softBody->m_cfg.maxvolume = 1000000.0;
  this->softBody->m_cfg.kPR += this->dPR * deltaTime.count();
  std::cout << this->softBody->m_cfg.kPR << std::endl;
}