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
  std::cout << btGetVersion() << std::endl;
  // Añadir plano
  std::shared_ptr<Scene> scene(new Scene());
  scene->AddRigidObject(loadPlane());
  // Añadir modelos desde archivos en consola
  for (size_t i = 1; i < argc; i++) {
    std::shared_ptr<SceneSoftObject> object =
        ModelLoader::LoadSoft(std::string(argv[i]), scene->softBodyWorldInfo);

    std::cout << object->softBody->getTotalMass() << std::endl;
    scene->AddSoftObject(object);
  }

  //   Añadir modelos desde archivos en consola
  for (size_t i = 1; i < argc; i++) {
    std::shared_ptr<SceneRigidObject> object =
        ModelLoader::Load(std::string(argv[i]));
    object->UpdateRigidBody(1);
    object->rigidBody->setWorldTransform(
        btTransform(btQuaternion::getIdentity(), btVector3(0, 10, 0)));
    scene->AddRigidObject(object);
  }

  scene->Loop();
  return EXIT_SUCCESS;
}