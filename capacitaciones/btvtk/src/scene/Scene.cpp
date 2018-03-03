#include "Scene.h"

#include <btBulletDynamicsCommon.h>

#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"


#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>

#include <chrono>
#include <memory>

class vtkTimerCallback : public vtkCommand {
 public:
  std::shared_ptr<Scene> scene;

  static vtkTimerCallback *New() {
    vtkTimerCallback *cb = new vtkTimerCallback;
    cb->TimerCount = 0;
    cb->deltaTime = std::chrono::duration<double>(1. / 60);
    return cb;
  }

  virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long eventId,
                       void *vtkNotUsed(callData)) {
    if (vtkCommand::TimerEvent == eventId) {
      // Medir tiempo.
      this->newTime = std::chrono::steady_clock::now();

      // Calcular tiempo transcurrido.
      this->deltaTime = this->newTime - this->prevTime;

      scene->Update(deltaTime);

      // Reiniciar tiempo.
      prevTime = newTime;

      ++this->TimerCount;
    }
  }

 private:
  int TimerCount;
  std::chrono::time_point<std::chrono::steady_clock> prevTime;
  std::chrono::time_point<std::chrono::steady_clock> newTime;
  std::chrono::duration<double> deltaTime;
};
Scene::Scene() { this->Init(); }
Scene::~Scene() {
  for (size_t i = 0; i < this->objects.size(); i++) {
    std::shared_ptr<SceneObject> sceneObject = this->objects[i];
    this->dynamicsWorld->removeRigidBody(sceneObject->rigidBody.get());
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
      std::shared_ptr<btDiscreteDynamicsWorld>(new btSoftRigidDynamicsWorld(
          dispatcher, broadphase, solver, collisionConfiguration));

  this-> dynamicsWorldAux =
   std::shared_ptr<btSoftRigidDynamicsWorld>(new btSoftRigidDynamicsWorld(
          dispatcher, broadphase, solver, collisionConfiguration));
  // Se aplica gravedad sobre el eje y.
  this->dynamicsWorld->setGravity(btVector3(0, -10, 0));

  // Agregar objetos a la lista de rigidBodies.
  for (size_t i = 0; i < this->objects.size(); i++) {
    std::shared_ptr<SceneObject> sceneObject = this->objects[i];
    this->dynamicsWorld->addRigidBody(sceneObject->rigidBody.get());
  }
}
void Scene::InitGraphics() {
  // Crear Renderer, RenderWindow y RenderWindowInteractor.
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->SetSize(768, 768);
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
  camera->SetPosition(0, 10, 50);
  camera->SetFocalPoint(0, 0, 0);
  camera->SetClippingRange(0.1, 100000);
  renderer->SetActiveCamera(camera);

  // Crear y agregar ejes.
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  axes->SetTotalLength(1, 1, 1);
  axes->AxisLabelsOff();

  renderer->AddActor(axes);

  // Agregar actores al renderer.
  for (size_t i = 0; i < this->objects.size(); i++) {
    std::shared_ptr<SceneObject> sceneObject = this->objects[i];
    renderer->AddActor(sceneObject->actor);
  }

  vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();  // like
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

  int timerId = renderWindowInteractor->CreateRepeatingTimer(16);

  this->renderWindowInteractor->Start();
}
void Scene::Update(std::chrono::duration<double> deltaTime) {
  this->UpdatePhysics(deltaTime);
  this->renderWindowInteractor->GetRenderWindow()->Render();
}
void Scene::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  this->dynamicsWorld->stepSimulation(deltaTime.count(), 10);

  // Llamar actualización de física de cada objeto.
  for (size_t i = 0; i < this->objects.size(); i++) {
    this->objects[i]->UpdatePhysics(deltaTime);
  }
}
void Scene::AddRigidObject(std::shared_ptr<SceneObject> object) {
  this->objects.push_back(object);
  this->dynamicsWorld->addRigidBody(object->rigidBody.get());
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->AddActor(object->actor);
}
void Scene::AddSoftObject(std::shared_ptr<SceneSoftObject> object) {
  this->softObjects.push_back(object);
  dynamicsWorldAux->addSoftBody(object->softBody.get());
 // (btSoftRigidDynamicsWorld)(this->dynamicsWorld) = dynamicsWorldAux;
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->AddActor(object->actor);
}
std::vector<std::shared_ptr<SceneObject>> Scene::GetRigidObjects() {
  return this->objects;
}
void Scene::SetBackgroundColor(double r, double g, double b) {
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->SetBackground(r, g, b);
}