#include <fstream>
#include <memory>
#include <string>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include "../src/loaders/ModelLoader.h"
#include "../src/scene/SceneRigidObject.h"
#include "../src/util/ToString.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "Usage:" << std::endl
              << "carga-vertices <ruta_modelo> <ruta_vertices_prueba>"
              << std::endl;
    return 1;
  }

  std::cout << "argv1: " << argv[1] << std::endl;
  std::cout << "argv2: " << argv[2] << std::endl;

  // Cargar modelo
  std::string archivoModelo(argv[1]);
  std::shared_ptr<SceneRigidObject> object = ModelLoader::Load(archivoModelo);

  // Revisar vertices cargados
  std::ifstream fin(argv[2]);
  vtkSmartPointer<vtkActor> actor = object->actor;
  vtkSmartPointer<vtkPolyData> polyData =
      vtkPolyData::SafeDownCast(actor->GetMapper()->GetInputAsDataSet());
  for (size_t i = 0; i < polyData->GetNumberOfPoints(); i++) {
    double pTest[3];
    fin >> pTest[0] >> pTest[1] >> pTest[2];

    std::cout << "--------------------------------------------------------"
              << std::endl;
    std::cout << ToString::Vec3Arr(pTest) << std::endl;

    double pVtk[] = {0, 0, 0};
    polyData->GetPoint(i, pVtk);

    std::cout << ToString::Vec3Arr(pVtk) << std::endl;

    if (!(pTest[0] == pVtk[0] && pTest[1] == pVtk[1] && pTest[2] == pVtk[2])) {
      return 1;
    }
  }
  return 0;
}