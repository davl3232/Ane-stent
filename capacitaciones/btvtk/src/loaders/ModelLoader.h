#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "../scene/SceneRigidObject.h"
#include "../scene/SceneSoftObject.h"

#include <vector>

class ModelLoader {
 public:
  static std::shared_ptr<SceneRigidObject> LoadTXT(std::string fileName);
  static std::shared_ptr<SceneRigidObject> LoadXML(std::string fileName);
  static std::shared_ptr<SceneRigidObject> Load(std::string fileName);
  static std::shared_ptr<SceneRigidObject> Load(
      std::vector<std::vector<double> > puntos, std::string name = "strip");
  static std::shared_ptr<SceneSoftObject> LoadSoft(
      std::string fileName, btSoftBodyWorldInfo worldInfo);
  static std::shared_ptr<SceneSoftObject> LoadXMLSoft(
      std::string fileName, btSoftBodyWorldInfo worldInfo);
  static std::shared_ptr<SceneSoftObject> LoadTXTSoft(
      std::string fileName, btSoftBodyWorldInfo worldInfo);
};
#endif
