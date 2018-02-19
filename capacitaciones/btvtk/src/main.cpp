#include <fstream>
#include <iostream>
#include <memory>

#include "scene/Scene.h"
#include "scene/SceneObject.h"

int main(int argc, char **argv) {
  std::shared_ptr<Scene> scene(new Scene());

  return EXIT_SUCCESS;
}