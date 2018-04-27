#include "SceneSoftObject.h"

#include <memory>

#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"
#include "BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "../util/ToString.h"
#include "RigidMotionState.h"

SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor) {
  this->actor = actor;
}
SceneSoftObject::~SceneSoftObject() {}
void SceneSoftObject::InitSoftBody(btSoftBodyWorldInfo &worldInfo,
                                   btTransform transform) {
  vtkSmartPointer<vtkPolyData> polyData =
      vtkPolyData::SafeDownCast(this->actor->GetMapper()->GetInputAsDataSet());

  // Extraer puntos del polydata
  vtkSmartPointer<vtkCellArray> polys = polyData->GetPolys();

  polys->InitTraversal();
  vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

  // Creación de los arreglos
  int numVerts = polyData->GetNumberOfPoints();
  btScalar verts[numVerts][3];
  int numTris = polys->GetNumberOfCells();
  int tris[numTris][3];

  // Vértices transformados para VTK.
  vtkSmartPointer<vtkPoints> newpts = vtkSmartPointer<vtkPoints>::New();
  // Recorrer vértices existentes.
  vtkSmartPointer<vtkPoints> oldpts = polyData->GetPoints();
  for (vtkIdType i = 0; i < numVerts; i++) {
    // Obtener vértice.
    double p[3];
    oldpts->GetPoint(i, p);

    // Transformar vértice.
    btVector3 pt(p[0], p[1], p[2]);
    pt = transform * pt;

    // Insertar vértice transformado en arreglo para VTK.
    p[0] = pt[0];
    p[1] = pt[1];
    p[2] = pt[2];
    newpts->InsertNextPoint(p);

    // Insertar vértice transformado en arreglo para Bullet.
    verts[i][0] = pt[0];
    verts[i][1] = pt[1];
    verts[i][2] = pt[2];

    std::cout << "Vértice " << i << ": (" << p[0] << "," << p[1] << "," << p[2]
              << ")" << std::endl;
  }

  // Actualizar puntos de VTK.
  newpts->Modified();
  polyData->SetPoints(newpts);

  size_t triInd = 0;
  // Recorrer polígonos de VTK.
  while (polys->GetNextCell(idList)) {
    std::cout << std::endl;
    std::cout << "Polígono con " << idList->GetNumberOfIds()
              << " puntos:" << std::endl;

    // Recorrer vértices del polígono.
    for (vtkIdType pointId = 0; pointId < idList->GetNumberOfIds(); pointId++) {
      // Obtener punto.
      double p[3];
      vtkIdType id = idList->GetId(pointId);

      // Guardar id del vértice en arreglo para Bullet.
      tris[triInd][pointId] = id;
      polyData->GetPoint(id, p);

      std::cout << "Vértice " << id << ": (" << p[0] << "," << p[1] << ","
                << p[2] << ")" << std::endl;
    }
    triInd++;
  }

  // Crear softbody en Bullet a partir de los arreglos.
  this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(
          worldInfo, &verts[0][0], &tris[0][0], numTris, true));

  std::cout << "softBody: " << this->softBody->getTotalMass() << std::endl;
  // btScalar m_kLST; // Linear stiffness coefficient [0,1]
  // btScalar m_kAST; // Area/Angular stiffness coefficient [0,1]
  // btScalar m_kVST; // Volume stiffness coefficient [0,1]
  btSoftBody::Material *pm = this->softBody->appendMaterial();
  pm->m_kLST = 1;
  pm->m_kAST = 1;
  pm->m_kVST = 0.01;

  // btSoftBody::Config::kVCF; // Velocities correction factor (Baumgarte)
  // define the amount of correction per time step for drift solver (sometimes
  // referred as ERP in rigid bodies solvers).

  // btSoftBody::Config::kDP; // Damping coefficient [0,1]
  // damping, zero = no damping, one= full damping.

  // btSoftBody::Config::kDG; // Drag coefficient [0,+inf]
  // [aerodynamic] kDG=0 mean no drag.

  // btSoftBody::Config::kLF; // Lift coefficient [0,+inf]
  // [aerodynamic]=> is a factor of the lift force kLF=0 mean no lift

  // btSoftBody::Config::kPR; // Pressure coefficient [-inf,+inf]
  // [aerodynamic]=> is a factor of pressure.

  // btSoftBody::Config::kVC; // Volume conversation coefficient [0,+inf]
  // when 'setPose(true,...)' as been called, define the magnitude of the force
  // used to conserve volume.

  // btSoftBody::Config::kDF; // Dynamic friction coefficient [0,1]
  // friction, kDF=0 mean sliding, kDF=1 mean sticking.

  // btSoftBody::Config::kMT; // Pose matching coefficient [0,1]
  // when 'setPose(...,true)' as been called, define the factor used for pose
  // matching.

  // btSoftBody::Config::kCHR; // Rigid contacts hardness [0,1]
  // define how 'soft' contact with rigid bodies are, kCHR=0 mean no penetration
  // correction, 1 mean full correction.

  // btSoftBody::Config::kKHR; // Kinetic contacts hardness [0,1]
  // define how 'soft' contact with kinetic/static bodies are, kKHR=0 mean no
  // penetration correction, 1 mean full correction.

  // btSoftBody::Config::kSHR; // Soft contacts hardness [0,1]
  // define how 'soft' contact with other soft bodies are, kSHR=0 mean no
  // penetration correction, 1 mean full correction.

  // btSoftBody::Config::kAHR; // Anchors hardness [0,1]
  // define how 'soft' anchor constraint (joint) are, kAHR=0 mean no drift
  // correction, 1 mean full correction.

  // btSoftBody::Config::maxvolume; // Maximum volume ratio for pose
  this->softBody->generateBendingConstraints(4, pm);
  this->softBody->m_cfg.piterations = 5;
  this->softBody->m_cfg.kDF = 0.5;
  this->softBody->m_cfg.kMT = 0.05;
  this->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
  this->softBody->randomizeConstraints();
  this->softBody->setTotalMass(10, true);
  this->UpdateMesh();
}

void SceneSoftObject::UpdateMesh() {
  // std::cout << "ACTUALIZANDO MAYA: " << this->name << std::endl;
  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();
  // std::cout << "Puntos del collisionShape: " <<
  // this->softBody->m_nodes.size()
  //           << std::endl;

  vtkSmartPointer<vtkPolyData> polyData =
      vtkPolyData::SafeDownCast(this->actor->GetMapper()->GetInputAsDataSet());

  // Vértices transformados para VTK.
  vtkSmartPointer<vtkPoints> newpts = vtkSmartPointer<vtkPoints>::New();
  for (size_t i = 0; i < this->softBody->m_nodes.size(); i++) {
    btVector3 pos = this->softBody->m_nodes[i].m_x;
    // std::cout << "Punto " << i << ": " << ToString::btVector3(pos) <<
    // std::endl;

    // Insertar vértice transformado en arreglo para VTK.
    newpts->InsertNextPoint(pos[0], pos[1], pos[2]);
  }
  // Actualizar puntos de VTK.
  newpts->Modified();
  polyData->SetPoints(newpts);

  // std::cout << "ACTUALIZADA." << this->name << std::endl;
}

btVector3 SceneSoftObject::getCenterOfGeometry() {
  btVector3 centerOfGeometry(0, 0, 0);
  for (size_t i = 0; i < this->softBody->m_nodes.size(); i++) {
    btSoftBody::Node node = this->softBody->m_nodes[i];
    centerOfGeometry += node.m_x;
  }
  return centerOfGeometry / double(this->softBody->m_nodes.size());
}

void SceneSoftObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {
  btVector3 centerOfGeometry = this->getCenterOfGeometry();
  for (size_t i = 0; i < this->softBody->m_nodes.size(); i++) {
    btSoftBody::Node node = this->softBody->m_nodes[i];
    btVector3 pos = node.m_x;
    btVector3 normal = pos - centerOfGeometry;
    btVector3 force = normal * 1000;
    this->softBody->addForce(force, i);
    // btVector3 force = node.m_n * 100;
    // btVector3 pos = node.m_x + force * deltaTime.count();
    if (i == 0) {
      std::cout << "Punto " << i << ": " << ToString::btVector3(pos)
                << std::endl;
      std::cout << "Fuerza: " << ToString::btVector3(force) << std::endl;
    }
  }
  // this->softBody->updatePose();
  this->softBody->applyForces();
}