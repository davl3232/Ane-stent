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
  actor->GetProperty()->SetColor(1, 1, 1);

  // Crear collider de cubo.
  std::shared_ptr<btCollisionShape> collider(
      new btBoxShape(btVector3(100, 0.5, 100)));

  // Crear SceneRigidObject
  std::shared_ptr<SceneRigidObject> softObject(
      new SceneRigidObject(actor, collider));
  softObject->InitRigidBody(0);
  softObject->name = "Plane";
  return softObject;
}

int main(int argc, char **argv) {
  std::cout << btGetVersion() << std::endl;
  // Añadir plano
  std::shared_ptr<Scene> scene(new Scene());
  scene->AddRigidObject(loadPlane());

  if (argc < 2) {
    std::cerr << "Exception: Bad arguments." << std::endl;
    std::cerr << "Usage: main <input_file_path>." << std::endl;
    return EXIT_FAILURE;
  }

  std::ifstream fin(argv[1]);

  std::string line = "";
  while (std::getline(fin, line)) {
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
    bool isRigid = 1;
    btScalar kVCF = 1;      // Velocities correction factor (Baumgarte)
    btScalar kDP = 0;       // Damping coefficient [0,1]
    btScalar kDG = 0;       // Drag coefficient [0,+inf]
    btScalar kLF = 0;       // Lift coefficient [0,+inf]
    btScalar kPR = 0;       // Pressure coefficient [-inf,+inf]
    btScalar kVC = 0;       // Volume conversation coefficient [0,+inf]
    btScalar kDF = 0.2;     // Dynamic friction coefficient [0,1]
    btScalar kMT = 0;       // Pose matching coefficient [0,1]
    btScalar kCHR = 1;      // Rigid contacts hardness [0,1]
    btScalar kKHR = 0.1;    // Kinetic contacts hardness [0,1]
    btScalar kSHR = 1;      // Soft contacts hardness [0,1]
    btScalar kAHR = 0.7;    // Anchors hardness [0,1]
    btScalar maxvolume = 1; // Maximum volume ratio for pose
    btScalar timescale = 1; // Time scale
    int viterations = 0;    // Velocities solver iterations
    int piterations = 1;    // Positions solver iterations
    int diterations = 0;    // Drift solver iterations
    if (lin >> isRigid) {
      if (isRigid) {
        if (lin >> sx >> sy >> sz) {
          if (lin >> rx >> ry >> rz) {
            if (lin >> tx >> ty >> tz) {
              lin >> mass;
            }
          }
        }
      } else {
        if (lin >> sx >> sy >> sz) {
          if (lin >> rx >> ry >> rz) {
            if (lin >> tx >> ty >> tz) {
              if (lin >> kLST >> kAST >> kVST) {
                if (lin >> mass) {
                  if (lin >> kVCF >> kDP >> kDG >> kLF >> kPR >> kVC >> kDF >>
                      kMT >> kCHR >> kKHR >> kSHR >> kAHR >> maxvolume >>
                      timescale >> viterations >> piterations >> diterations) {
                    if (lin >> dPR) {
                      inflates = true;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << filename << std::endl;
    std::cout << "Is rigid: " << isRigid << std::endl;
    std::cout << "Scale: (" << sx << "," << sy << "," << sz << ")" << std::endl;
    std::cout << "Rotation: (" << rx << "," << ry << "," << rz << ")"
              << std::endl;
    std::cout << "Translation: (" << tx << "," << ty << "," << tz << ")"
              << std::endl;
    std::cout << "Mass: " << mass << std::endl;
    if (!isRigid) {
      std::cout << kLST << " " << kAST << " " << kVST << std::endl;
      std::cout << "kVCF: " << kVCF << std::endl;
      std::cout << "kDP: " << kDP << std::endl;
      std::cout << "kDG: " << kDG << std::endl;
      std::cout << "kLF: " << kLF << std::endl;
      std::cout << "kPR: " << kPR << std::endl;
      std::cout << "kVC: " << kVC << std::endl;
      std::cout << "kDF: " << kDF << std::endl;
      std::cout << "kMT: " << kMT << std::endl;
      std::cout << "kCHR: " << kCHR << std::endl;
      std::cout << "kKHR: " << kKHR << std::endl;
      std::cout << "kSHR: " << kSHR << std::endl;
      std::cout << "kAHR: " << kAHR << std::endl;
      std::cout << "maxvolume: " << maxvolume << std::endl;
      std::cout << "timescale: " << timescale << std::endl;
      std::cout << "viterations: " << viterations << std::endl;
      std::cout << "piterations: " << piterations << std::endl;
      std::cout << "diterations: " << diterations << std::endl;
      std::cout << "Inflates: " << inflates << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

    if (isRigid) {
      std::shared_ptr<SceneRigidObject> rigidObject;
      vtkSmartPointer<vtkTransform> transform =
          vtkSmartPointer<vtkTransform>::New();
      transform->Scale(sx, sy, sz);
      btQuaternion rotation;
      rotation.setEuler(ry * 3.141592653589793 / 180.0,
                        rx * 3.141592653589793 / 180.0,
                        rz * 3.141592653589793 / 180.0);
      rigidObject = ModelLoader::Load(filename, mass, transform);
      rigidObject->rigidBody->setFriction(0.5);
      // rigidObject->rigidBody->getCollisionShape()->setMargin(0.1);
      rigidObject->rigidBody->setWorldTransform(
          btTransform(rotation, btVector3(tx, ty, tz)));

      scene->AddRigidObject(rigidObject);
    } else {
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
      // softObject->softBody->getCollisionShape()->setMargin(0.1);
      btSoftBody::Material *pm = softObject->softBody->appendMaterial();
      pm->m_kLST = kLST;
      pm->m_kAST = kAST;
      pm->m_kVST = kVST;
      softObject->softBody->generateBendingConstraints(2, pm);
      softObject->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
      softObject->softBody->m_cfg.kVCF = kVCF;
      softObject->softBody->m_cfg.kDP = kDP;
      softObject->softBody->m_cfg.kDG = kDG;
      softObject->softBody->m_cfg.kLF = kLF;
      softObject->softBody->m_cfg.kPR = kPR;
      softObject->softBody->m_cfg.kVC = kVC;
      softObject->softBody->m_cfg.kDF = kDF;
      softObject->softBody->m_cfg.kMT = kMT;
      softObject->softBody->m_cfg.kCHR = kCHR;
      softObject->softBody->m_cfg.kKHR = kKHR;
      softObject->softBody->m_cfg.kSHR = kSHR;
      softObject->softBody->m_cfg.kAHR = kAHR;
      softObject->softBody->m_cfg.maxvolume = maxvolume;
      softObject->softBody->m_cfg.timescale = timescale;
      softObject->softBody->m_cfg.viterations = viterations;
      softObject->softBody->m_cfg.piterations = piterations;
      softObject->softBody->m_cfg.diterations = diterations;
      softObject->softBody->randomizeConstraints();
      softObject->softBody->setTotalMass(mass, true);
      if (inflates) {
        std::shared_ptr<InflatingObject> io =
            std::make_shared<InflatingObject>();
        io->softBody = softObject->softBody;
        io->name = softObject->name;
        io->actor = softObject->actor;
        io->dPR = dPR;
        scene->AddSoftObject(std::static_pointer_cast<SceneSoftObject>(io));
      } else {
        scene->AddSoftObject(softObject);
      }
    }
  }

  scene->Loop();
  return EXIT_SUCCESS;
}