#include <fstream>
#include <iostream>
#include <memory>

#include <vtkCubeSource.h>
#include <vtkSphereSource.h>

#include <vtkActor.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include "loaders/ModelLoader.h"
#include "scene/Scene.h"
#include "scene/SceneRigidObject.h"
#include "scene/SceneSoftObject.h"

std::shared_ptr<SceneSoftObject> loadSoftSphere(std::shared_ptr<Scene> scene) {
  // Crear source de cubo.
  vtkSmartPointer<vtkSphereSource> source =
      vtkSmartPointer<vtkSphereSource>::New();

  source->Update();

  // Crear mapper.
  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(source->GetOutputPort());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(1, 0.5, 0.5);

  // Crear SceneRigidObject
  std::shared_ptr<SceneSoftObject> object(new SceneSoftObject(actor));
  object->UpdateSoftBody(scene->softBodyWorldInfo, 10);

  object->name = "Soft Sphere";
   std::cout<<"llegue aca mother8"<<std::endl;
  return object;
}

std::shared_ptr<SceneRigidObject> loadPlane() {
  // Crear source de cubo.
  vtkSmartPointer<vtkCubeSource> source = vtkSmartPointer<vtkCubeSource>::New();
  source->SetXLength(200);
  source->SetYLength(1);
  source->SetZLength(200);
  source->Update();

  // Crear mapper.
  vtkSmartPointer<vtkPolyDataMapper> mapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(source->GetOutputPort());

  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(0, 1, 0);

  // Crear collider de cubo.
  std::shared_ptr<btCollisionShape> collider(
      new btBoxShape(btVector3(100, 0.5, 100)));

  // Crear SceneRigidObject
  std::shared_ptr<SceneRigidObject> object(
      new SceneRigidObject(actor, collider));
  object->UpdateRigidBody(0);
  object->name = "Plane";
  return object;
}

int main(int argc, char **argv) {
  // Añadir plano
  std::shared_ptr<Scene> scene(new Scene());
  scene->AddRigidObject(loadPlane());
  scene->AddSoftObject(loadSoftSphere(scene));
  // Añadir modelos desde archivos en consola
  //   for (size_t i = 1; i < argc; i++) {
  //     std::shared_ptr<SceneRigidObject> object =
  //         ModelLoader::Load(std::string(argv[i]));
  //     btTransform trans(btQuaternion::getIdentity(), btVector3(0, 10, 0));
  //     object->rigidBody->setWorldTransform(trans);
  //     scene->AddRigidObject(object);
  //   }
  std::cout<<"llegue aca mother9"<<std::endl;
  scene->Loop();
  return EXIT_SUCCESS;
}