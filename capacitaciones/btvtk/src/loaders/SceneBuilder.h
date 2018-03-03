#ifndef SCENE_BUILDER_H
#define SCENE_BUILDER_H

#include <memory>

#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkSphereSource.h>

#include "../scene/Scene.h"
#include "../scene/SceneRigidObject.h"
#include "ModelLoader.h"

class SceneBuilder {
 public:
  static std::shared_ptr<Scene> Build() {
    std::shared_ptr<Scene> scene(new Scene());
    scene->SetBackgroundColor(0.4, 0.5, 1.0);
    LoadGarden(scene);
    LoadHouse(scene);
    LoadClouds(scene);
    LoadWorldPlane(scene);

    return scene;
  }
  static std::shared_ptr<SceneRigidObject> LoadCube(
      double tx = 0, double ty = 0, double tz = 0, double rx = 0, double ry = 0,
      double rz = 0, double sx = 1, double sy = 1, double sz = 1, double r = 1,
      double g = 1, double b = 1) {
    // La transformación.
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(tx, ty, tz);
    transform->RotateX(rx);
    transform->RotateY(ry);
    transform->RotateZ(rz);
    transform->Scale(sx, sy, sz);
    transform->Update();

    // La fuente.
    vtkSmartPointer<vtkCubeSource> source =
        vtkSmartPointer<vtkCubeSource>::New();
    source->SetCenter(0, 0, 0);
    source->Update();

    // Cargar objeto.
    std::shared_ptr<SceneRigidObject> object(
        ModelLoader::Load(source, transform));

    // Cambiar color.
    object->actor->GetProperty()->SetColor(r, g, b);

    return object;
  }
  static std::shared_ptr<SceneRigidObject> LoadSphere(
      double tx = 0, double ty = 0, double tz = 0, double rx = 0, double ry = 0,
      double rz = 0, double sx = 1, double sy = 1, double sz = 1, double r = 1,
      double g = 1, double b = 1) {
    // La transformación.
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(tx, ty, tz);
    transform->RotateX(rx);
    transform->RotateY(ry);
    transform->RotateZ(rz);
    transform->Scale(sx, sy, sz);
    transform->Update();

    // La fuente.
    vtkSmartPointer<vtkSphereSource> source =
        vtkSmartPointer<vtkSphereSource>::New();
    source->SetRadius(0.5);
    source->SetPhiResolution(20);
    source->SetThetaResolution(40);
    source->SetCenter(0, 0, 0);
    source->Update();

    // Cargar objeto.
    std::shared_ptr<SceneRigidObject> object(
        ModelLoader::Load(source, transform));

    // Cambiar color.
    object->actor->GetProperty()->SetColor(r, g, b);

    return object;
  }
  static std::shared_ptr<SceneRigidObject> LoadCylinder(
      double tx = 0, double ty = 0, double tz = 0, double rx = 0, double ry = 0,
      double rz = 0, double sx = 1, double sy = 1, double sz = 1, double r = 1,
      double g = 1, double b = 1) {
    // La transformación.
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(tx, ty, tz);
    transform->RotateX(rx);
    transform->RotateY(ry);
    transform->RotateZ(rz);
    transform->Scale(sx, sy, sz);
    transform->Update();

    // La fuente.
    vtkSmartPointer<vtkCylinderSource> source =
        vtkSmartPointer<vtkCylinderSource>::New();
    source->SetRadius(0.5);
    source->SetHeight(1);
    source->SetResolution(40);
    source->SetCenter(0, 0, 0);
    source->Update();

    // Cargar objeto.
    std::shared_ptr<SceneRigidObject> object(
        ModelLoader::Load(source, transform));

    // Cambiar color.
    object->actor->GetProperty()->SetColor(r, g, b);

    return object;
  }
  static std::shared_ptr<SceneRigidObject> LoadRoof() {
    // La transformación.
    vtkSmartPointer<vtkTransform> R = vtkSmartPointer<vtkTransform>::New();
    vtkSmartPointer<vtkTransform> S = vtkSmartPointer<vtkTransform>::New();
    vtkSmartPointer<vtkTransform> T = vtkSmartPointer<vtkTransform>::New();
    R->RotateZ(45);
    S->Scale(sqrt(2.0) / 2.0, 0.5, 0.99);
    T->Translate(0, 0.5, 0);
    R->Update();
    S->Update();
    T->Update();
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PreMultiply();
    transform->Concatenate(R);
    transform->PostMultiply();
    transform->Concatenate(S);
    transform->PostMultiply();
    transform->Concatenate(T);
    transform->Update();

    // La fuente.
    vtkSmartPointer<vtkCubeSource> source =
        vtkSmartPointer<vtkCubeSource>::New();
    source->SetCenter(0, 0, 0);
    source->Update();
    std::shared_ptr<SceneRigidObject> object(
        ModelLoader::Load(source, transform));
    object->actor->GetProperty()->SetColor(1, 0.4, 0);
    return object;
  }
  static void LoadGarden(std::shared_ptr<Scene> scene) {
    // Pasto
    scene->AddObject(LoadCube(0, -0.6, 0, 0, 0, 0, 4, 0.1, 4, 0, 1, 0.2));
    // Arbusto
    scene->AddObject(
        LoadCube(-0.8, -0.3, 0.7, 0, 0, 0, 1, 0.4, 0.4, 0, 1, 0.2));
    // Árbol
    scene->AddObject(LoadSphere(1, 0.3, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0.2));
    scene->AddObject(LoadCylinder(1, 0, 1, 0, 0, 0, 0.2, 1, 0.2, 0.3, 0.3, 0));
  }
  static void LoadHouse(std::shared_ptr<Scene> scene) {
    // Techo
    scene->AddObject(LoadRoof());
    // Paredes
    scene->AddObject(LoadCube());
    // Ventanas
    scene->AddObject(
        LoadCube(0.25, 0.2, 0.525, 0, 0, 0, 0.25, 0.25, 0.05, 0.8, 0.8, 1));
    scene->AddObject(
        LoadCube(-0.25, 0.2, 0.525, 0, 0, 0, 0.25, 0.25, 0.05, 0.8, 0.8, 1));

    // Puerta
    scene->AddObject(
        LoadCube(0, -0.3, 0.525, 0, 0, 0, 0.4, 0.5, 0.05, 0.4, 0.4, 0));
  }
  static void LoadClouds(std::shared_ptr<Scene> scene) {
    scene->AddObject(
        LoadSphere(-0.4, 2, 0.7, 0, 0, 0, 1, 0.4, 0.4, 0.9, 0.9, 0.9));
    scene->AddObject(
        LoadSphere(-0.8, 2, 0.5, 0, 0, 0, 1, 0.3, 0.4, 0.9, 0.9, 0.9));

    scene->AddObject(
        LoadSphere(1.4, 1.4, -0.3, 0, 0, 0, 1, 0.4, 0.4, 0.9, 0.9, 0.9));
    scene->AddObject(
        LoadSphere(1.7, 1.5, -0.5, 0, 0, 0, 1, 0.3, 0.4, 0.9, 0.9, 0.9));
  }
  static void LoadWorldPlane(std::shared_ptr<Scene> scene) {
    // Tierra
    scene->AddObject(
        LoadCube(0, -0.6, 0, 0, 0, 0, 1000, 0.09, 1000, 0.7, 0.7, 0.7));
  }
};
#endif