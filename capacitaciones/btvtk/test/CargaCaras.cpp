#include <fstream>
#include <memory>
#include <string>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>

#include "../src/loaders/ModelLoader.h"
#include "../src/scene/SceneRigidObject.h"
#include "../src/util/ToString.h"

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cout << "Usage:" << std::endl
              << "carga-caras <ruta_modelo> <ruta_caras_prueba>" << std::endl;
    return 1;
  }

  std::cout << "argv1: " << argv[1] << std::endl;
  std::cout << "argv2: " << argv[2] << std::endl;

  // Cargar modelo
  std::string archivoModelo(argv[1]);
  std::shared_ptr<SceneRigidObject> object = ModelLoader::Load(archivoModelo);

  // Revisar caras cargados
  std::ifstream fin(argv[2]);
  vtkSmartPointer<vtkActor> actor = object->actor;
  vtkSmartPointer<vtkPolyData> polyData =
      vtkPolyData::SafeDownCast(actor->GetMapper()->GetInputAsDataSet());
  vtkSmartPointer<vtkCellArray> polys = polyData->GetPolys();
  for (vtkIdType i = 0; i < polys->GetNumberOfCells(); i++)
  {
    vtkIdType idsTest[3];
    fin >> idsTest[0] >> idsTest[1] >> idsTest[2];

    std::cout << "--------------------------------------------------------"
              << std::endl;
    std::cout << ToString::Vec3Arr(idsTest) << std::endl;

    vtkIdType idsVtk[3];
    polys->GetNextCell(i, idsTest);

    std::cout << ToString::Vec3Arr(idsVtk) << std::endl;

    if (!(idsTest[0] == idsVtk[0] &&idsTest[1] == idsVtk[1] &&idsTest[2] == idsVtk[2]) {
      return 1;
    }
  }
  return 0;
}