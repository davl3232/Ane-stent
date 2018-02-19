#include "Scene.h"

#include <btBulletDynamicsCommon.h>

#include <vtkAxesActor.h>
#include <vtkNamedColors.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <chrono>
#include <memory>

Scene::Scene() {
  this->Init();
  this->backgroundColor[0] = 0;
  this->backgroundColor[1] = 0;
  this->backgroundColor[2] = 0;
}
Scene::~Scene() {
  for (size_t i = 0; i < this->objects.size(); i++) {
    std::shared_ptr<SceneObject> sceneObject = this->objects[i];
    this->dynamicsWorld->removeRigidBody(sceneObject->rigidBody.get());
  }
}

void Scene::Init() {
  this->InitPhysics();
  this->InitGraphics();
  this->Loop();
}
void Scene::InitPhysics() {
  // Algoritmo de emparejamiento de colisionadores (Broadphase).
  // En este caso un Dynamic AABBTree.
  btBroadphaseInterface *broadphase = new btDbvtBroadphase();

  // Configuración de detección de colisiones entre dos pares y el despachador.
  btDefaultCollisionConfiguration *collisionConfiguration =
      new btDefaultCollisionConfiguration();
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
      std::shared_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(
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
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();

  // Crear Renderer, RenderWindow y RenderWindowInteractor.
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  // Crear y agregar ejes.
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  axes->SetTotalLength(2, 2, 2);
  axes->SetTipTypeToUserDefined();
  axes->AxisLabelsOff();

  renderer->AddActor(axes);
  renderer->SetBackground(this->backgroundColor);

  // Agregar actores.
  std::cout << "Cargando actores...";

  // Agregar objetos a la lista de rigidBodies.
  for (size_t i = 0; i < this->objects.size(); i++) {
    std::shared_ptr<SceneObject> sceneObject = this->objects[i];
    renderer->AddActor(sceneObject->actor);
  }

  std::cout << "Finalizado" << std::endl;

  // Renderizar e interactuar.
  renderWindow->Render();
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Start();
}
void Scene::Loop() {
  // Medir tiempo.
  std::chrono::time_point<std::chrono::steady_clock> prevTime =
      std::chrono::steady_clock::now();
  std::chrono::time_point<std::chrono::steady_clock> newTime;
  std::chrono::duration<double> deltaTime(1. / 60);

  for (unsigned long i = 0; i < 600; i++) {
    // Medir tiempo.
    newTime = std::chrono::steady_clock::now();

    // Calcular tiempo transcurrido.
    deltaTime = newTime - prevTime;

    this->Update(deltaTime);

    // Reiniciar tiempo.
    prevTime = newTime;
  }
}
void Scene::Update(std::chrono::duration<double> deltaTime) {
  this->PhysicsUpdate(deltaTime);
}
void Scene::PhysicsUpdate(std::chrono::duration<double> deltaTime) {
  this->dynamicsWorld->stepSimulation(deltaTime.count(), 10);

  // Llamar actualización de física de cada objeto.
  for (size_t i = 0; i < this->objects.size(); i++) {
    this->objects[i]->PhysicsUpdate(deltaTime);
  }
}