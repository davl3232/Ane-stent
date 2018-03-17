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
  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();

  // Creación de los arreglos
  btVector3 v[dataSet->GetNumberOfPoints()];
  btScalar s[(dataSet->GetNumberOfPoints()* 3)];
  int triangles [dataSet->GetNumberOfPoints()];
  // Extraer puntos del DataSet
  for (vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++) {
    double p[3];
    dataSet->GetPoint(i, p);
    btVector3 vi(p[0], p[1], p[2]);
    std::cout << std::endl;
    std::cout << "Punto " << i << ": (" << p[0] << "," << p[1] << "," << p[2]
              << ")" << std::endl;

    v[i] = vi;
    triangles[i]=i;
    s[i*3]=p[0];
    s[(i*3)+1]=p[1];
    s[(i*3)+2]=p[2];
  }

  std::cout << "Softbody loaded: Inserted " << dataSet->GetNumberOfPoints()
            << " vertices." << std::endl;
/***
 *
btSoftBody * btSoftBodyHelpers::CreateFromTriMesh 	( 	btSoftBodyWorldInfo &  	worldInfo,
		const btScalar *  	vertices,
		const int *  	triangles,
		int  	ntriangles,
		bool  	randomizeConstraints = true 
	) 	
 * 
*/
   for (int i = 0; i < dataSet->GetNumberOfPoints()*3; i++) {
       std::cout<<s[i]<<std::endl;
    
   }
this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(
          worldInfo, &s[0], &triangles[0],dataSet->GetNumberOfPoints()/3.0 , true));

  /*this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromConvexHull(
          worldInfo, &v[0], dataSet->GetNumberOfPoints(), true));*/
  std::cout<<"softBody: "<<this->softBody->getTotalMass()<<std::endl;
  btSoftBody::Material *pm = this->softBody->appendMaterial();
  pm->m_kLST = 0.75;

  this->softBody->generateBendingConstraints(4, pm);
  this->softBody->m_cfg.piterations = 2;
  this->softBody->m_cfg.kDF = 0.75;
  this->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;
  this->softBody->randomizeConstraints();
  // this->softBody->getCollisionShape()->setMargin(0.1f);
  // this->UpdateMesh();
}

void SceneSoftObject::UpdateMesh() {
  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();

  this->softBody->getCollisionShape()->getShapeType();
  // std::cout << "ShapeType: "
  //           << this->softBody->getCollisionShape()->isSoftBody() <<
  //           std::endl;
  btConvexHullShape *btc =
      dynamic_cast<btConvexHullShape *>(this->softBody->getCollisionShape());
  std::cout << "Puntos del collisionShape: " << btc->getNumPoints()
            << std::endl;
  const btVector3 *points = btc->getPoints();
  for (int i = 0; i < btc->getNumPoints(); i++) {
    btVector3 p = points[i];
    std::cout << std::endl;
    std::cout << "Punto " << i << ": (" << p[0] << "," << p[1] << "," << p[2]
              << ")" << std::endl;
  }

}
