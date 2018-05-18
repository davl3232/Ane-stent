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

SceneSoftObject::SceneSoftObject() {}
SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor) {
  this->actor = actor;
}
SceneSoftObject::~SceneSoftObject() {}
void SceneSoftObject::InitSoftBody(btSoftBodyWorldInfo &worldInfo,
                                   vtkSmartPointer<vtkTransform> transform) {
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
    double pk[4];
    oldpts->GetPoint(i, pk);
    pk[3] = 0;
    double p[4];
    transform->TransformPoint(pk, p);
    newpts->InsertNextPoint(p);

    // Transformar vértice.
    btVector3 pt(p[0], p[1], p[2]);

    // Insertar vértice transformado en arreglo para Bullet.
    verts[i][0] = pt[0];
    verts[i][1] = pt[1];
    verts[i][2] = pt[2];

    // std::cout << "Vértice " << i << ": (" << p[0] << "," << p[1] << "," <<
    // p[2]
    //           << ")" << std::endl;
  }

  // Actualizar puntos de VTK.
  newpts->Modified();
  polyData->SetPoints(newpts);

  size_t triInd = 0;
  // Recorrer polígonos de VTK.
  while (polys->GetNextCell(idList)) {
    // std::cout << std::endl;
    // std::cout << "Polígono con " << idList->GetNumberOfIds()
    //           << " puntos:" << std::endl;

    // Recorrer vértices del polígono.
    for (vtkIdType pointId = 0; pointId < idList->GetNumberOfIds(); pointId++) {
      // Obtener punto.
      double p[3];
      vtkIdType id = idList->GetId(pointId);

      // Guardar id del vértice en arreglo para Bullet.
      tris[triInd][pointId] = id;
      polyData->GetPoint(id, p);

      // std::cout << "Vértice " << id << ": (" << p[0] << "," << p[1] << ","
      //           << p[2] << ")" << std::endl;
    }
    triInd++;
  }

  // Crear softbody en Bullet a partir de los arreglos.
  this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(
          worldInfo, &verts[0][0], &tris[0][0], numTris, true));
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

void SceneSoftObject::UpdatePhysics(std::chrono::duration<double> deltaTime) {}