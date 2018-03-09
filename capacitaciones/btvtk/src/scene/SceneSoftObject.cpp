#include "SceneSoftObject.h"

#include <memory>

#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "RigidMotionState.h"

SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor) {
  this->actor = actor;
}
SceneSoftObject::~SceneSoftObject() {}
void SceneSoftObject::UpdateSoftBody(btSoftBodyWorldInfo &worldInfo) {
  btAlignedObjectArray<btScalar> vertices;
  btAlignedObjectArray<int> indices;
  // std::vector<btVector3> v;
  // loop through all the shapes and add vertices and indices
  int offset = 0;

  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();

  // creación de los arreglos

  btVector3 v[dataSet->GetNumberOfPoints()];

  // Extraer puntos del DataSet
  for (vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++) {
    double p[3];
    dataSet->GetPoint(i, p);
    btVector3 vi(p[0], p[1], p[2]);

    v[i] = vi;
  }
  std::cout << "Softbody loaded: Inserted " << dataSet->GetNumberOfPoints()
            << " vertices." << std::endl;

  this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromConvexHull(
          worldInfo, &v[0], dataSet->GetNumberOfPoints(), true));

  btSoftBody::Material *pm = this->softBody->appendMaterial();
  pm->m_kLST = 0.75;

  this->softBody->generateBendingConstraints(4, pm);
  this->softBody->m_cfg.piterations = 2;
  this->softBody->m_cfg.kDF = 0.75;
  this->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
  this->softBody->randomizeConstraints();
  this->softBody->getCollisionShape()->setMargin(0.1f);
}
