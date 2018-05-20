#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "../scene/SceneRigidObject.h"
#include "../scene/SceneSoftObject.h"

#include <memory>
#include <string>
#include <vector>

class ModelLoader {
public:
  static std::shared_ptr<SceneRigidObject>
  LoadTXT(std::string fileName, btScalar mass = 1,
          vtkSmartPointer<vtkTransform> transform =
              vtkSmartPointer<vtkTransform>::New());
  static std::shared_ptr<SceneRigidObject>
  LoadXML(std::string fileName, btScalar mass = 1,
          vtkSmartPointer<vtkTransform> transform =
              vtkSmartPointer<vtkTransform>::New());
  static std::shared_ptr<SceneRigidObject>
  Load(std::string fileName, btScalar mass = 1,
       vtkSmartPointer<vtkTransform> transform =
           vtkSmartPointer<vtkTransform>::New());
  static std::shared_ptr<SceneSoftObject>
  LoadSoft(std::string fileName, btSoftBodyWorldInfo worldInfo,
           vtkSmartPointer<vtkTransform> transform =
               vtkSmartPointer<vtkTransform>::New());
  static std::shared_ptr<SceneSoftObject>
  LoadXMLSoft(std::string fileName, btSoftBodyWorldInfo worldInfo,
              vtkSmartPointer<vtkTransform> transform =
                  vtkSmartPointer<vtkTransform>::New());
  static std::shared_ptr<SceneSoftObject>
  LoadTXTSoft(std::string fileName, btSoftBodyWorldInfo worldInfo,
              vtkSmartPointer<vtkTransform> transform =
                  vtkSmartPointer<vtkTransform>::New());
};
#endif