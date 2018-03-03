#include "SceneSoftObject.h"

#include <memory>

#include "BulletSoftBody/btSoftBodyHelpers.h"

#include <vtkDataSet.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

#include "RigidMotionState.h"

SceneSoftObject::SceneSoftObject(vtkSmartPointer<vtkActor> actor) {
  this->actor = actor;
}

void SceneSoftObject::UpdateSoftBody(btSoftBodyWorldInfo& worldInfo,
                                     btScalar mass) {
  btAlignedObjectArray<btScalar> vertices;
  btAlignedObjectArray<int> indices;

  // loop through all the shapes and add vertices and indices
  int offset = 0;

  vtkSmartPointer<vtkMapper> mapper = this->actor->GetMapper();

  vtkSmartPointer<vtkDataSet> dataSet = mapper->GetInput();

  // Extraer puntos del DataSet
  for (vtkIdType i = 0; i < dataSet->GetNumberOfPoints(); i++) {
    double p[3];
    dataSet->GetPoint(i, p);

    for (char j = 0; j < 3; j++) {
      vertices.push_back(p[j]);
    }
    indices.push_back(i);
  }
  std::cout << "[INFO] Obj loaded: Extracted " << vertices.size()
            << " vertices, " << indices.size() << std::endl;

  this->softBody =
      std::shared_ptr<btSoftBody>(btSoftBodyHelpers::CreateFromTriMesh(
          worldInfo, &vertices[0], &(indices[0]), indices.size() / 3));
}