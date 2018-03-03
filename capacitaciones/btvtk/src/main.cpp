#include <fstream>
#include <iostream>
#include <memory>

#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include "loaders/ModelLoader.h"
#include "scene/Scene.h"
#include "scene/SceneObject.h"

std::shared_ptr<SceneObject> loadPlane() {
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

  // Crear SceneObject
  std::shared_ptr<SceneObject> object(new SceneObject(actor, collider));
  object->UpdateRigidBody(0);
  object->name = "Plane";
  return object;
}

int main(int argc, char **argv) {
  // Añadir plano
  std::shared_ptr<Scene> scene(new Scene());
  scene->AddObject(loadPlane());
  // Añadir modelos desde archivos en consola
  for (size_t i = 1; i < argc; i++) {
    std::shared_ptr<SceneObject> object =
        ModelLoader::Load(std::string(argv[i]));
    btTransform trans(btQuaternion::getIdentity(), btVector3(0, 10, 0));
    object->rigidBody->setWorldTransform(trans);
    scene->AddObject(object);
  }
  scene->Loop();
  return EXIT_SUCCESS;
}