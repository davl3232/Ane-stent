#include <fstream>
#include <iostream>
#include <memory>

#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkNamedColors.h>


#include "loaders/ModelLoader.h"
#include "scene/SceneObject.h"

int main(int argc, char **argv) {
  vtkSmartPointer<vtkNamedColors> colors =
			vtkSmartPointer<vtkNamedColors>::New();
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " InputFilename" << std::endl;
    return EXIT_FAILURE;
  }
  std::string fileName(argv[1]);
  std::shared_ptr<SceneObject> object = ModelLoader::Load(fileName);

  // Crear Renderer, RenderWindow y RenderWindowInteractor
  vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow =
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);

  // Crear y agregar ejes
  vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
  axes->SetTotalLength(2, 2, 2);
  axes->SetTipTypeToUserDefined();
  axes->AxisLabelsOff();

  renderer->AddActor(axes);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
  // Agregar actor de objeto
  std::cout << "Cargando actores...";
  double color[3] = {1, 1, 1};
  object->actor->GetProperty()->SetColor(color);

  renderWindow->Render();
  renderer->AddActor(object->actor);
  std::cout << "Finalizado" << std::endl;

  renderer->SetBackground(0, 0, 0); // Background color dark red

  // Renderizar e interactuar

  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}