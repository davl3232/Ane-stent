#include "SceneMotionState.h"

#include <vtkActor.h>
#include <vtkTransform.h>

#include "SceneObject.h"

SceneMotionState::SceneMotionState(std::shared_ptr<SceneObject> sceneObject,
                                   btTransform initialTransform) {
  this->sceneObject = sceneObject;
  this->initialTransform = initialTransform;
}
SceneMotionState::~SceneMotionState() {}

void SceneMotionState::getWorldTransform(btTransform &worldTrans) const {
  worldTrans = initialTransform;
}

void SceneMotionState::setWorldTransform(const btTransform &worldTrans) {
  vtkSmartPointer<vtkTransform> vtkTrans = vtkSmartPointer<vtkTransform>::New();
  // btScalar btTransMat[16] = {};
  // double vtkTransMat[16] = {};
  // worldTrans.getOpenGLMatrix(btTransMat);
  // std::cout << "Mat: {";
  // for (size_t i = 0; i < 16; i++) {
  //   if (i % 4 == 0) {
  //     std::cout << std::endl;
  //   } else {
  //     std::cout << ',';
  //   }
  //   vtkTransMat[i] = btTransMat[i];
  //   std::cout << btTransMat[i];
  // }
  // std::cout << std::endl;
  // std::cout << "}" << std::endl;
  // vtkTrans->SetMatrix(vtkTransMat);
  btVector3 btOrigin = worldTrans.getOrigin();
  // vtkTrans->Translate(btOrigin.x(), btOrigin.y(), btOrigin.z());
  // vtkTrans->Update();
  // btQuaternion btRotation = worldTrans.getRotation();
  // btQuaternion btRotation = worldTrans.getRotation();
  if (auto object = this->sceneObject.lock()) {
    object->actor->SetOrigin(btOrigin.x(), btOrigin.y(), btOrigin.z());
    // object->actor->SetPosition(btOrigin.x(), btOrigin.y(), btOrigin.z());
    // object->actor->SetOrientation(btRotation.);
    object->actor->SetUserTransform(vtkTrans);
  }
}