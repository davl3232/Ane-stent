#include "RigidMotionState.h"

#include <vtkActor.h>
#include <vtkTransform.h>

#include "../util/ToString.h"
#include "SceneRigidObject.h"

RigidMotionState::RigidMotionState(
    std::shared_ptr<SceneRigidObject> rigidObject,
    btTransform initialTransform) {
  this->rigidObject = rigidObject;
  this->initialTransform = initialTransform;
}
RigidMotionState::~RigidMotionState() {}

void RigidMotionState::getWorldTransform(btTransform &worldTrans) const {
  worldTrans = initialTransform;
}

void RigidMotionState::setWorldTransform(const btTransform &worldTrans) {
  vtkSmartPointer<vtkTransform> vtkTrans = vtkSmartPointer<vtkTransform>::New();
  btScalar btTransMat[16] = {};
  double vtkTransMat[16] = {};
  worldTrans.getOpenGLMatrix(btTransMat);
  std::cout << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << "Bullet:" << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << ToString::MatArr(btTransMat) << std::endl;
  for (size_t i = 0; i < 16; i++) {
    vtkTransMat[i / 4 + 4 * (i % 4)] = btTransMat[i];
  }

  std::cout << "===================================" << std::endl;
  std::cout << "VTK:" << std::endl;
  std::cout << "===================================" << std::endl;
  std::cout << ToString::MatArr(vtkTransMat) << std::endl;
  vtkTrans->SetMatrix(vtkTransMat);
  if (auto object = this->rigidObject.lock()) {
    object->actor->SetUserTransform(vtkTrans);
  }
}