#ifndef SCENE_MOTION_STATE_H
#define SCENE_MOTION_STATE_H

#include <memory>

#include "SceneRigidObject.h"

class SceneMotionState : public btMotionState {
 protected:
  std::weak_ptr<SceneRigidObject> rigidObject;
  btTransform initialTransform;

 public:
  SceneMotionState(std::shared_ptr<SceneRigidObject> rigidObject,
                   btTransform initialTransform);
  virtual ~SceneMotionState();
  virtual void getWorldTransform(btTransform &worldTrans) const;
  virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif