#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "../scene/SceneRigidObject.h"

#include <memory>
#include <string>
#include <vector>

class ModelLoader {
 public:
  static std::shared_ptr<SceneRigidObject> LoadTXT(std::string fileName);
  static std::shared_ptr<SceneRigidObject> LoadXML(std::string fileName);
  static std::shared_ptr<SceneRigidObject> Load(std::string fileName);
  static std::shared_ptr<SceneRigidObject> Load(
      std::vector<std::vector<double> > puntos, std::string name = "strip");
  static std::shared_ptr<SceneRigidObject> LoadSoftBody(std::string fileName);
};
#endif