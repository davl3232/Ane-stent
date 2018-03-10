#ifndef SCENE_MOTION_STATE_H
#define SCENE_MOTION_STATE_H

#include "SceneRigidObject.h"

class RigidMotionState : public btMotionState {
 protected:
  std::weak_ptr<SceneRigidObject> rigidObject;
  btTransform initialTransform;

 public:
  RigidMotionState(std::shared_ptr<SceneRigidObject> rigidObject,
                   btTransform initialTransform);
  virtual ~RigidMotionState();
  virtual void getWorldTransform(btTransform &worldTrans) const;
  virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif
