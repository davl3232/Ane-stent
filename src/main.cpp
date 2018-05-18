#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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
#include "scene/InflatingObject.h"
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
  std::shared_ptr<SceneRigidObject> softObject(
      new SceneRigidObject(actor, collider));
  softObject->UpdateRigidBody(0);
  softObject->name = "Plane";
  return softObject;
}

int main(int argc, char **argv) {
  std::cout << btGetVersion() << std::endl;
  // Añadir plano
  std::shared_ptr<Scene> scene(new Scene());
  scene->AddRigidObject(loadPlane());

  std::string line = "";
  while (std::getline(std::cin, line)) {
    std::stringstream lin(line);
    std::string filename = "";
    lin >> filename;
    double sx, sy, sz, rx, ry, rz, tx, ty, tz;
    sx = sy = sz = 1.0;
    rx = ry = rz = tx = ty = tz = 0.0;
    double dPR = 0.0;
    bool inflates = false;
    double kLST, kAST, kVST;
    kLST = kAST = kVST = 0.1;
    double mass = 1;
    if (lin >> sx >> sy >> sz) {
      if (lin >> rx >> ry >> rz) {
        if (lin >> tx >> ty >> tz) {
          if (lin >> kLST >> kAST >> kVST) {
            if (lin >> mass) {
              if (lin >> dPR) {
                inflates = true;
              }
            }
          }
        }
      }
    }

    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << filename << std::endl;
    std::cout << sx << " " << sy << " " << sz << std::endl;
    std::cout << rx << " " << ry << " " << rz << std::endl;
    std::cout << tx << " " << ty << " " << tz << std::endl;
    std::cout << "Inflates: " << inflates << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

    std::shared_ptr<SceneSoftObject> softObject;
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->PostMultiply();
    transform->Scale(sx, sy, sz);
    transform->RotateX(rx);
    transform->RotateY(ry);
    transform->RotateZ(rz);
    transform->Translate(tx, ty, tz);
    softObject =
        ModelLoader::LoadSoft(filename, scene->softBodyWorldInfo, transform);
    softObject->softBody->m_worldInfo = &(scene->softBodyWorldInfo);
    softObject->softBody->getCollisionShape()->setMargin(0.1);
    std::cout << softObject->softBody->getCollisionShape()->getMargin()
              << std::endl;
    btSoftBody::Material *pm = softObject->softBody->appendMaterial();
    pm->m_kLST = kLST;
    pm->m_kAST = kAST;
    pm->m_kVST = kVST;
    softObject->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
    softObject->softBody->m_cfg.kDF = 1;
    softObject->softBody->m_cfg.kDP = 0.001; // fun factor...
    softObject->softBody->generateBendingConstraints(2, pm);
    softObject->softBody->randomizeConstraints();
    softObject->softBody->setTotalMass(mass, true);
    if (inflates) {
      std::shared_ptr<InflatingObject> io = std::make_shared<InflatingObject>();
      io->softBody = softObject->softBody;
      io->name = softObject->name;
      io->actor = softObject->actor;
      io->dPR = dPR;
      scene->AddSoftObject(std::static_pointer_cast<SceneSoftObject>(io));
    } else {
      scene->AddSoftObject(softObject);
    }
  }

  scene->Loop();
  return EXIT_SUCCESS;
}