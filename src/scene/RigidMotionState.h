#ifndef SCENE_MOTION_STATE_H
#define SCENE_MOTION_STATE_H

#include <memory>

#include "SceneRigidObject.h"

class RigidMotionState : public btMotionState {
protected:
  std::weak_ptr<SceneRigidObject> rigidObject;

public:
  RigidMotionState(std::shared_ptr<SceneRigidObject> rigidObject);
  virtual ~RigidMotionState();
  virtual void getWorldTransform(btTransform &worldTrans) const;
  virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif