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
#include <memory>

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

      if (this->deltaTime > std::chrono::duration<double>(1. / 60) * 4) {
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
  renderWindow->SetSize(1800, 400);
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
  camera->SetPosition(0, 10, 20);
  camera->SetFocalPoint(0, 0, 0);
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
  // this->DebugNormals();
  this->renderWindowInteractor->GetRenderWindow()->Render();
}
void Scene::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  for (size_t i = 0; i < this->rigidObjects.size(); i++) {
    this->rigidObjects[i]->UpdatePhysics(deltaTime);
  }
  for (size_t i = 0; i < this->softObjects.size(); i++) {
    this->softObjects[i]->UpdatePhysics(deltaTime);
  }
  this->dynamicsWorld->stepSimulation(deltaTime.count(), 10);
  // std::cout << "Soft objects: " << this->softObjects.size() << std::endl;
  // Llamar actualización de física de cada objeto suave.
  for (size_t i = 0; i < this->softObjects.size(); i++) {
    this->softObjects[i]->UpdateMesh();
  }
}
vtkSmartPointer<vtkAssembly>
GetNormalsProp(std::shared_ptr<btSoftBody> softBody) {
  vtkSmartPointer<vtkAssembly> assembly = vtkSmartPointer<vtkAssembly>::New();

  size_t numNodes = softBody->m_nodes.size();

  for (size_t i = 0; i < numNodes; i++) {
    btSoftBody::Node node = softBody->m_nodes[i];
    btVector3 p = node.m_x;
    btVector3 n = node.m_n;

    double origin[3];
    origin[0] = p.getX();
    origin[1] = p.getY();
    origin[2] = p.getZ();

    // Compute a basis
    double normalizedX[3];
    double normalizedY[3];
    double normalizedZ[3];

    normalizedX[0] = n.getX();
    normalizedX[1] = n.getY();
    normalizedX[2] = n.getZ();

    // The X axis is a vector from start to end
    double length = vtkMath::Norm(normalizedX);
    vtkMath::Normalize(normalizedX);

    // The Z axis is an arbitrary vector cross X
    double arbitrary[3];
    arbitrary[0] = 1;
    arbitrary[1] = 1;
    arbitrary[2] = 1;
    vtkMath::Cross(normalizedX, arbitrary, normalizedZ);
    vtkMath::Normalize(normalizedZ);

    // The Y axis is Z cross X
    vtkMath::Cross(normalizedZ, normalizedX, normalizedY);
    vtkSmartPointer<vtkMatrix4x4> matrix = vtkSmartPointer<vtkMatrix4x4>::New();

    // Create the direction cosine matrix
    matrix->Identity();
    for (unsigned int i = 0; i < 3; i++) {
      matrix->SetElement(i, 0, normalizedX[i]);
      matrix->SetElement(i, 1, normalizedY[i]);
      matrix->SetElement(i, 2, normalizedZ[i]);
    }

    // Setup the arrows
    vtkSmartPointer<vtkArrowSource> arrowSource =
        vtkSmartPointer<vtkArrowSource>::New();
    arrowSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> vectorMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    vectorMapper->SetInputConnection(arrowSource->GetOutputPort());
    vtkSmartPointer<vtkActor> vectorActor = vtkSmartPointer<vtkActor>::New();
    vectorActor->SetMapper(vectorMapper);

    // Apply the transforms
    vtkSmartPointer<vtkTransform> transform =
        vtkSmartPointer<vtkTransform>::New();
    transform->Translate(origin);
    transform->Concatenate(matrix);
    transform->Scale(length, length, length);
    vectorActor->SetUserTransform(transform);

    assembly->AddPart(vectorActor);
  }

  return assembly;
}
void Scene::DebugNormals() {
  std::cout << "NORMALS" << std::endl;
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->RemoveActor(this->normals);
  this->normals = GetNormalsProp(this->softObjects[0]->softBody);
  this->renderWindowInteractor->GetRenderWindow()
      ->GetRenderers()
      ->GetFirstRenderer()
      ->AddActor(this->normals);
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