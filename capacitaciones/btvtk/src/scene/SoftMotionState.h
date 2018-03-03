#ifndef SOFT_MOTION_STATE_H
#define SOFT_MOTION_STATE_H

#include <memory>

#include "SceneSoftObject.h"

class SoftMotionState : public btMotionState {
 protected:
  std::weak_ptr<SceneSoftObject> softObject;
  btTransform initialTransform;

 public:
  SoftMotionState(std::shared_ptr<SceneSoftObject> softObject,
                  btTransform initialTransform);
  virtual ~SoftMotionState();
  virtual void getWorldTransform(btTransform &worldTrans) const;
  virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif