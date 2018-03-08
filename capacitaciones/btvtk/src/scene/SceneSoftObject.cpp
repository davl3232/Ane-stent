#include "SceneSoftObject.h"

#include <memory>

#include "BulletSoftBody/btSoftBody.h"
#include "BulletSoftBody/btSoftBodyHelpers.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "RigidMotionState.h"

SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor) {
  this->actor = actor;
}
SceneSoftObject::~SceneSoftObject() {}
void SceneSoftObject::UpdateSoftBody(btSoftBodyWorldInfo& worldInfo,
                                     btScalar mass) {
  btAlignedObjectArray<btScalar> vertices;
  btAlignedObjectArray<int> indices;
  //std::vector<btVector3> v;
  // loop through all the shapes and add vertices and indices
  int offset = 0;

  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();
  btVector3 v[dataSet->GetNumberOfPoints()];
  // Extraer puntos del DataSet
  for (vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++) {
    double p[3];
    dataSet->GetPoint(i, p);
    btVector3 vi(p[0], p[1], p[2]);
    //for (char j = 0; j < 3; j++) {
     // vertices.push_back(p[j]);
    //}
    v[i]=vi;
    indices.push_back(i);
  }
  std::cout << "[INFO] Obj loaded: Extracted " << vertices.size()
            << " vertices, " << indices.size() << std::endl;

 // this->softBody =
   //   std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(
     //     worldInfo, vertices, indices, indices.size() / 3));
     /**
     CreateFromConvexHull(	btSoftBodyWorldInfo& worldInfo,
		const btVector3* vertices,
		int nvertices,
		bool randomizeConstraints = true);
     **/
 std::cout<<"llegue aca mother"<<worldInfo.air_density<<std::endl;
   //  this-> softBody = std:: shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(worldInfo, &vertices[0], &(indices[0]), indices.size()));
  this-> softBody = std:: shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromConvexHull(worldInfo, &v[0], dataSet->GetNumberOfPoints(),true ));
/*

 btVector3 scaling(0.1, 0.1, 0.1); 
 
   btSoftBody::Material* pm=psb->appendMaterial();
   pm->m_kLST =	0.75;
   pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
   psb->scale(scaling);
   psb->generateBendingConstraints(4,pm);
   psb->m_cfg.piterations = 2;  
   psb->m_cfg.kDF = 0.75;
   psb->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;	
   psb->randomizeConstraints();  

   btMatrix3x3	m;
   btVector3 x(0,10,0);
   btVector3 a(0,0,0);
   m.setEulerZYX(a.x(),a.y(),a.z());
   psb->transform(btTransform(m,x));  
   psb->setTotalMass(1); 
   psb->getCollisionShape()->setMargin(0.1f);
   getSoftDynamicsWorld()->addSoftBody(psb); 
*/
    btVector3 scaling(0.1, 0.1, 0.1); 
     btSoftBody::Material* pm=this->softBody->appendMaterial();
     pm->m_kLST =	0.75;
      std::cout<<"llegue aca mother1"<<std::endl;
     pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
     this->softBody->scale(scaling);
     this->softBody->generateBendingConstraints(4,pm);
     this->softBody->m_cfg.piterations = 2; 
      std::cout<<"llegue aca mother2"<<std::endl; 
     this->softBody->m_cfg.kDF = 0.75;
     this->softBody->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;	
      std::cout<<"llegue aca mother3"<<std::endl;
     this->softBody->randomizeConstraints();  
 std::cout<<"llegue aca mother4"<<std::endl;
     btMatrix3x3	m;
   btVector3 x(0,10,0);
   btVector3 a(0,0,0);
    std::cout<<"llegue aca mother5"<<std::endl;
   m.setEulerZYX(a.x(),a.y(),a.z());
   this->softBody->transform(btTransform(m,x));  
     std::cout<<"llegue aca mother6"<<std::endl;
  this->softBody->setTotalMass(1); 
   this->softBody->getCollisionShape()->setMargin(0.1f);
     std::cout<<"llegue aca mother7"<<std::endl;

}