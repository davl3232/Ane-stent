#include "ModelLoader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "../scene/SceneObject.h"

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkGeometryFilter.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLGenericDataObjectReader.h>

std::shared_ptr<btCollisionShape>
createConvexHullCollider(vtkSmartPointer<vtkPolyData> polyData) {
  std::vector<std::vector<double>> puntos;

  // Extraer puntos del PolyData
  for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++) {
    double p[3];
    polyData->GetPoint(i, p);

    std::vector<double> nuevo(3, 0);
    nuevo[0] = (p[0]);
    nuevo[1] = (p[1]);
    nuevo[2] = (p[2]);

    std::cout << "Point " << i << ":(" << p[0] << "," << p[1] << "," << p[2]
              << ")" << std::endl;

    puntos.push_back(nuevo);
  }

  // Meter puntos en colisionador
  std::shared_ptr<btConvexHullShape> convexHullShape(new btConvexHullShape());
  for (int i = 0; i < puntos.size(); i++) {
    convexHullShape->addPoint(
        btVector3(puntos[i][0], puntos[i][1], puntos[i][2]));
  }

  return convexHullShape;
}

std::shared_ptr<SceneObject> ModelLoader::Load(std::string fileName) {
  // Abrir archivo
  std::ifstream myReadFile;
  myReadFile.open(fileName);

  if (myReadFile.is_open()) {
    // Leer primer caracter
    char firstFileChar;
    myReadFile >> firstFileChar;

    // Si el primer caracter del archivo es un '<'...
    if (firstFileChar == '<') {
      return ModelLoader::LoadXML(fileName);
    } else {
      return ModelLoader::LoadTXT(fileName);
    }
  }
  return NULL;
}

std::shared_ptr<SceneObject> ModelLoader::LoadXML(std::string fileName) {
  // Leer como XML
  std::cout << "Cargando XML..." << std::endl;

  vtkSmartPointer<vtkXMLGenericDataObjectReader> reader =
      vtkSmartPointer<vtkXMLGenericDataObjectReader>::New();
  reader->SetFileName(fileName.c_str());
  reader->Update();

  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  // Determinar tipo de contenido e instanciar polyData
  if (vtkPolyData::SafeDownCast(reader->GetOutput())) {
    std::cout << "\tEl archivo es un PolyData." << std::endl;
    polyData = reader->GetPolyDataOutput();
    std::cout << polyData->GetNumberOfPoints() << std::endl;
  } else {
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkGeometryFilter> geometryFilter =
        vtkSmartPointer<vtkGeometryFilter>::New();

    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    geometryFilter->SetInputData(unstructuredGrid);
    geometryFilter->Update();
    surfaceFilter->SetInputData(unstructuredGrid);
    surfaceFilter->Update();
    polyData = surfaceFilter->GetOutput();
    std::cout << polyData->GetNumberOfPoints() << std::endl;
  }

  // Crear actor
  std::cout << "\tCreando actor...";

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

  std::cout << "Terminado." << std::endl;

  // Crear colisionador
  std::cout << "\tCreando colisionador...";

  std::shared_ptr<btCollisionShape> convexHullShape(
      createConvexHullCollider(polyData));

  std::cout << "Terminado." << std::endl;

  // Crear SceneObject
  std::cout << "\tCreando SceneObject...";

  std::shared_ptr<SceneObject> object(new SceneObject(actor, convexHullShape));

  std::cout << "Terminado." << std::endl;

  return object;
}

std::shared_ptr<SceneObject> ModelLoader::LoadTXT(std::string fileName) {
  // Leer como TXT
  std::cout << "Cargando TXT..." << std::endl;
  vtkSmartPointer<vtkGenericDataObjectReader> reader =
      vtkSmartPointer<vtkGenericDataObjectReader>::New();
  reader->SetFileName(fileName.c_str());
  reader->Update();

  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

  // Determinar tipo de contenido e instanciar polyData
  if (vtkPolyData::SafeDownCast(reader->GetOutput())) {
    std::cout << "\tEl archivo es un PolyData." << std::endl;
    polyData = reader->GetPolyDataOutput();
    std::cout << polyData->GetNumberOfPoints() << std::endl;
  } else {
    vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid =
        vtkSmartPointer<vtkUnstructuredGrid>::New();
    vtkSmartPointer<vtkGeometryFilter> geometryFilter =
        vtkSmartPointer<vtkGeometryFilter>::New();

    vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
        vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    geometryFilter->SetInputData(unstructuredGrid);
    geometryFilter->Update();
    surfaceFilter->SetInputData(unstructuredGrid);
    surfaceFilter->Update();
    polyData = surfaceFilter->GetOutput();
    std::cout << polyData->GetNumberOfPoints() << std::endl;
  }

  // Crear actor
  std::cout << "\tCreando actor...";

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

  std::cout << "Terminado." << std::endl;

  // Crear colisionador
  std::cout << "\tCreando colisionador...";

  std::shared_ptr<btCollisionShape> convexHullShape(
      createConvexHullCollider(polyData));

  std::cout << "Terminado." << std::endl;

  // Crear SceneObject
  std::cout << "\tCreando SceneObject...";

  std::shared_ptr<SceneObject> object(new SceneObject(actor, convexHullShape));

  std::cout << "Terminado." << std::endl;

  return object;
}