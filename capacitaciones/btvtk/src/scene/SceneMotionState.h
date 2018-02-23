#ifndef SCENE_MOTION_STATE_H
#define SCENE_MOTION_STATE_H

#include <memory>

#include "SceneObject.h"

class SceneMotionState : public btMotionState {
 protected:
  std::weak_ptr<SceneObject> sceneObject;
  btTransform initialTransform;

 public:
  SceneMotionState(std::shared_ptr<SceneObject> sceneObject,
                   btTransform initialTransform);
  virtual ~SceneMotionState();
  virtual void getWorldTransform(btTransform &worldTrans) const;
  virtual void setWorldTransform(const btTransform &worldTrans);
};
#endif