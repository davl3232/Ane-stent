#include "RigidMotionState.h"

#include <vtkActor.h>
#include <vtkTransform.h>

#include "../util/ToString.h"
#include "SceneRigidObject.h"

RigidMotionState::RigidMotionState(
    std::shared_ptr<SceneRigidObject> rigidObject) {
  this->rigidObject = rigidObject;
}

RigidMotionState::~RigidMotionState() {}

void RigidMotionState::getWorldTransform(btTransform &worldTrans) const {
  btTransform transform(btQuaternion::getIdentity());
  worldTrans = transform;
  // if (auto object = this->rigidObject.lock()) {
  //   const btVector3 origin = object->GetCenterOfGeometry();
  //   std::cout << ToString::btVector3(origin) << std::endl;
  //   btScalar btTransMat[16] = {};
  //   transform.getOpenGLMatrix(btTransMat);
  //   std::cout << ToString::MatArr(btTransMat) << std::endl;
  // }
}

void RigidMotionState::setWorldTransform(const btTransform &worldTrans) {
  vtkSmartPointer<vtkTransform> vtkTrans = vtkSmartPointer<vtkTransform>::New();
  btScalar btTransMat[16] = {};
  double vtkTransMat[16] = {};
  worldTrans.getOpenGLMatrix(btTransMat);
  // std::cout << std::endl;
  // std::cout << "===================================" << std::endl;
  // std::cout << "Bullet:" << std::endl;
  // std::cout << "===================================" << std::endl;
  // std::cout << ToString::MatArr(btTransMat) << std::endl;
  for (size_t i = 0; i < 16; i++) {
    if ((i == 0 || i == 5 || i == 10 || i == 15) &&
        (btTransMat[i] < 0.9 || btTransMat[i] > 1.1)) {
      std::cout << "!!!!!" << std::endl;
    }
    vtkTransMat[i / 4 + 4 * (i % 4)] = btTransMat[i];
  }

  // std::cout << "===================================" << std::endl;
  // std::cout << "VTK:" << std::endl;
  // std::cout << "===================================" << std::endl;
  // std::cout << ToString::MatArr(vtkTransMat) << std::endl;
  vtkTrans->SetMatrix(vtkTransMat);
  if (auto object = this->rigidObject.lock()) {
    object->actor->SetUserTransform(vtkTrans);
  }
}