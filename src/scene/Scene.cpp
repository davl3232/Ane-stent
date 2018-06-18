#include "Scene.h"

#include <btBulletDynamicsCommon.h>

#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"

#include <vtkArrowSource.h>
#include <vtkAssembly.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkTransform.h>
#include <vtkVertexGlyphFilter.h>

#include <chrono>
#include <fstream>
#include <memory>
#include <sstream>

class vtkTimerCallback : public vtkCommand {
public:
  std::shared_ptr<Scene> scene;

  static vtkTimerCallback *New() {
    vtkTimerCallback *cb = new vtkTimerCallback;
    cb->TimerCount = 0;
    cb->deltaTime = std::chrono::duration<double>(1. / 60);
    cb->isFirstExecution = true;
    return cb;
  }

  virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eventId,
                       void *vtkNotUsed(callData)) {
    if (vtkCommand::TimerEvent == eventId) {
      // Medir tiempo.
      this->newTime = std::chrono::steady_clock::now();

      // Calcular tiempo transcurrido.
      if (this->isFirstExecution) {
        this->isFirstExecution = false;
      } else {
        this->deltaTime = this->newTime - this->prevTime;
      }

      if (this->deltaTime > std::chrono::duration<double>(2)) {
        this->deltaTime = std::chrono::duration<double>(1. / 60);
      }

      double count = this->deltaTime.count();
      // std::cout << "DT: " << count << std::endl;

      scene->Update(this->deltaTime);
      // Reiniciar tiempo.
      this->prevTime = this->newTime;

      ++this->TimerCount;
    }
  }

private:
  int TimerCount;
  bool isFirstExecution;
  std::chrono::time_point<std::chrono::steady_clock> prevTime;
  std::chrono::time_point<std::chrono::steady_clock> newTime;
  std::chrono::duration<double> deltaTime;
};
Scene::Scene() { this->Init(); }
Scene::~Scene() {
  for (size_t i = 0; i < this->rigidObjects.size(); i++) {
    std::shared_ptr<SceneRigidObject> SceneRigidObject = this->rigidObjects[i];
    this->dynamicsWorld->removeRigidBody(SceneRigidObject->rigidBody.get());
  }
}
void Scene::Init() {
  this->InitPhysics();
  this->InitGraphics();
}
void Scene::InitPhysics() {
  // Algoritmo de emparejamiento de colisionadores (Broadphase).
  // En este caso un Dynamic AABBTree.
  btBroadphaseInterface *broadphase = new btDbvtBroadphase();

  // Configuración de detección de colisiones entre dos pares y el despachador.
  btDefaultCollisionConfiguration *collisionConfiguration =
      new btSoftBodyRigidBodyCollisionConfiguration();
  // Soporta colisiones convex-convex y convex-concave.
  btCollisionDispatcher *dispatcher =
      new btCollisionDispatcher(collisionConfiguration);
  // Para soportar otros tipos de colisionadores ver clase
  // btGImpactCollisionAlgoritm.

  // Solucionador del estado siguiente de la simulación a partir de la lógica de
  // simulación. Es un punto importante para el rendimiento de la simulación.
  // Este solucionador implementa PGS para LCP.
  btSequentialImpulseConstraintSolver *solver =
      new btSequentialImpulseConstraintSolver;

  // Instanciar el mundo con el despachador, el algoritmo de broadphase, el
  // solucionador y la configuacion de colisiones.
  this->dynamicsWorld =
      std::shared_ptr<btSoftRigidDynamicsWorld>(new btSoftRigidDynamicsWorld(
          dispatcher, broadphase, solver, collisionConfiguration));

  // Se aplica gravedad sobre el eje y.
  this->dynamicsWorld->setGravity(btVector3(0, -10, 0));

  // Configuración para la simulación de cuerpos suaves.
  this->softBodyWorldInfo.m_broadphase = broadphase;
  this->softBodyWorldInfo.m_dispatcher = dispatcher;
  this->softBodyWorldInfo.m_gravity = this->dynamicsWorld->getGravity();
  this->softBodyWorldInfo.m_sparsesdf.Initialize();
}

void getCenterOfMass() {}
void Scene::InitGraphics() {
  // Crear Renderer, RenderWindow y RenderWindowInteractor.
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(1024, 1024);
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
  camera->SetPosition(0, 10, 20);
  camera->SetFocalPoint(0, 5, 0);
  camera->SetClippingRange(0.1, 100000);
  renderer->SetActiveCamera(camera);

  // Crear y agregar ejes.
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  axes->SetTotalLength(1, 1, 1);
  axes->AxisLabelsOff();

  renderer->AddActor(axes);

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New(); // like
                                                                 // paraview

  this->renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();

  this->renderWindowInteractor->SetInteractorStyle(style);

  this->renderWindowInteractor->SetRenderWindow(renderWindow);
}
void Scene::Loop() {
  // Initialize must be called prior to creating timer events.
  renderWindowInteractor->Initialize();

  // Sign up to receive TimerEvent
  vtkSmartPointer<vtkTimerCallback> callback =
      vtkSmartPointer<vtkTimerCallback>::New();

  callback->scene = std::shared_ptr<Scene>(this);
  renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, callback);
  // std::cout << "Mass: " << this->softObjects[0]->softBody->getTotalMass()
  //           << std::endl;
  int timerId = renderWindowInteractor->CreateRepeatingTimer(16);
  this->renderWindowInteractor->Start();
}
void Scene::Update(std::chrono::duration<double> deltaTime) {
  this->UpdatePhysics(deltaTime);
  this->renderWindowInteractor->GetRenderWindow()->Render();
}
void Scene::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  if (frameNum < 1000) {
    ofstream file;
    std::stringstream ss;
    int iterations = this->softObjects[0]->softBody->m_cfg.viterations;
    std::cout << iterations << std::endl;
    ss << "/Users/davl3232/Documents/uni/tg/Ane-stent/tiemposPorCuadro/iterations/" << iterations << ".txt";
    std::string s = ss.str();
    if (frameNum == 0) {
      std::cout << "Ruta: '" << s << "'" << std::endl;
      file.open(s.c_str());
    } else {
      file.open(s.c_str(), ios::app);
    }
    if (!file.is_open()) {
      cerr << "open error\n";
    }
    cout << frameNum << "\t" << deltaTime.count() << std::endl;
    if (!(file << frameNum++ << "\t" << deltaTime.count() << std::endl)) {
      cerr << "write error\n";
    }
    file.close();
  }

  for (size_t i = 0; i < this->rigidObjects.size(); i++) {
    this->rigidObjects[i]->UpdatePhysics(deltaTime);
  }
  for (size_t i = 0; i < this->softObjects.size(); i++) {
    this->softObjects[i]->UpdatePhysics(deltaTime);
  }
  this->dynamicsWorld->stepSimulation(deltaTime.count(), 1, btScalar(1.)/btScalar(120.));
  // Llamar actualización de física de cada objeto suave.
  for (size_t i = 0; i < this->softObjects.size(); i++) {
    this->softObjects[i]->UpdateMesh();
  }
}
void Scene::AddRigidObject(std::shared_ptr<SceneRigidObject> object) {
  this->rigidObjects.push_back(object);
  this->dynamicsWorld->addRigidBody(object->rigidBody.get());
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->AddActor(object->actor);
}
void Scene::AddSoftObject(std::shared_ptr<SceneSoftObject> object) {
  this->softObjects.push_back(object);
  this->dynamicsWorld->addSoftBody(object->softBody.get());
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->AddActor(object->actor);
}
void Scene::SetBackgroundColor(double r, double g, double b) {
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->SetBackground(r, g, b);
}