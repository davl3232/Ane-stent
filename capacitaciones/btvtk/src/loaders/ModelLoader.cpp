#include "ModelLoader.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../scene/SceneRigidObject.h"
#include "../scene/SceneSoftObject.h"

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkGeometryFilter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSmartPointer.h>
#include <vtkTriangleStrip.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLGenericDataObjectReader.h>

std::shared_ptr<btCollisionShape> createConvexHullCollider(
    std::vector<std::vector<double>> vertices) {
  // Meter puntos en colisionador
  std::shared_ptr<btConvexHullShape> convexHullShape(new btConvexHullShape());
  for (int i = 0; i < vertices.size(); i++) {
    convexHullShape->addPoint(
        btVector3(vertices[i][0], vertices[i][1], vertices[i][2]));
    std::cout << "Point " << i << ":(" << vertices[i][0] << ","
              << vertices[i][1] << "," << vertices[i][2] << ")" << std::endl;
  }

  return convexHullShape;
}
std::shared_ptr<btCollisionShape> createConvexHullCollider(
    vtkSmartPointer<vtkPolyData> polyData) {
  std::vector<std::vector<double>> vertices;

  // Extraer puntos del PolyData
  for (vtkIdType i = 0; i < polyData->GetNumberOfPoints(); i++) {
    double p[3];
    polyData->GetPoint(i, p);

    std::vector<double> np(3, 0);
    np[0] = p[0];
    np[1] = p[1];
    np[2] = p[2];

    vertices.push_back(np);
  }

  return createConvexHullCollider(vertices);
}

std::shared_ptr<SceneRigidObject> ModelLoader::Load(std::string fileName) {
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

std::shared_ptr<SceneRigidObject> ModelLoader::Load(
    std::vector<std::vector<double>> vertices, std::string name) {
  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  for (size_t i = 0; i < vertices.size(); i++) {
    points->InsertNextPoint(0, 0, 0);
  }

  vtkSmartPointer<vtkTriangleStrip> triangleStrip =
      vtkSmartPointer<vtkTriangleStrip>::New();
  triangleStrip->GetPointIds()->SetNumberOfIds(4);
  triangleStrip->GetPointIds()->SetId(0, 0);
  triangleStrip->GetPointIds()->SetId(1, 1);
  triangleStrip->GetPointIds()->SetId(2, 2);
  triangleStrip->GetPointIds()->SetId(3, 3);

  vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
  cells->InsertNextCell(triangleStrip);

  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
  polyData->SetPoints(points);
  polyData->SetStrips(cells);

  std::cout << polyData->GetNumberOfPoints() << std::endl;

  // Crear actor
  std::cout << "\tCreando actor...";

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputData(polyData);
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  std::cout << "Terminado." << std::endl;

  // Crear colisionador
  std::cout << "\tCreando colisionador...";

  std::shared_ptr<btCollisionShape> convexHullShape(
      createConvexHullCollider(polyData));

  std::cout << "Terminado." << std::endl;

  // Crear SceneRigidObject
  std::cout << "\tCreando SceneRigidObject...";

  std::shared_ptr<SceneRigidObject> object(
      new SceneRigidObject(actor, convexHullShape));
  object->UpdateRigidBody(1);
  object->name = name;

  std::cout << "Terminado." << std::endl;

  return object;
}

std::shared_ptr<SceneRigidObject> ModelLoader::LoadXML(std::string fileName) {
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
  polyData = reader->GetPolyDataOutput();
  std::cout << polyData->GetNumberOfPoints() << std::endl;

  // Crear actor
  std::cout << "\tCreando actor...";

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);

  std::cout << "Terminado." << std::endl;

  // Crear colisionador
  std::cout << "\tCreando colisionador...";

  std::shared_ptr<btCollisionShape> convexHullShape(
      createConvexHullCollider(polyData));

  std::cout << "Terminado." << std::endl;

  // Crear SceneRigidObject
  std::cout << "\tCreando SceneRigidObject...";

  std::shared_ptr<SceneRigidObject> object(
      new SceneRigidObject(actor, convexHullShape));
  object->UpdateRigidBody(1);
  object->name = fileName;

  std::cout << "Terminado." << std::endl;

  return object;
}

std::shared_ptr<SceneRigidObject> ModelLoader::LoadTXT(std::string fileName) {
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
  actor->SetMapper(mapper);
  std::cout << "Terminado." << std::endl;

  // Crear colisionador
  std::cout << "\tCreando colisionador...";

  std::shared_ptr<btCollisionShape> convexHullShape(
      createConvexHullCollider(polyData));

  std::cout << "Terminado." << std::endl;

  // Crear SceneRigidObjects
  std::cout << "\tCreando SceneRigidObject...";

  std::shared_ptr<SceneRigidObject> object(
      new SceneRigidObject(actor, convexHullShape));
  object->UpdateRigidBody(1);
  object->name = fileName;
  std::cout << "Terminado." << std::endl;

  return object;
}

std::shared_ptr<SceneSoftObject> ModelLoader::LoadSoft(
    std::string fileName, btSoftBodyWorldInfo worldInfo) {
  // Abrir archivo
  std::ifstream myReadFile;
  myReadFile.open(fileName);

  if (myReadFile.is_open()) {
    // Leer primer caracter
    char firstFileChar;
    myReadFile >> firstFileChar;

    // Si el primer caracter del archivo es un '<'...
    if (firstFileChar == '<') {
      return ModelLoader::LoadXMLSoft(fileName, worldInfo);
    } else {
      return ModelLoader::LoadTXTSoft(fileName, worldInfo);
    }
  }
  return NULL;
}

std::shared_ptr<SceneSoftObject> ModelLoader::LoadXMLSoft(
    std::string fileName, btSoftBodyWorldInfo worldInfo) {
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
  polyData = reader->GetPolyDataOutput();
  std::cout << polyData->GetNumberOfPoints() << std::endl;

  // Crear actor
  std::cout << "\tCreando actor...";

  vtkSmartPointer<vtkDataSetMapper> mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
  mapper->SetInputConnection(reader->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  std::cout << "Terminado." << std::endl;

  // Crear SceneSoftObject
  std::cout << "\tCreando SceneSoftObject...";

  std::shared_ptr<SceneSoftObject> object(new SceneSoftObject(actor));
  object->UpdateSoftBody(worldInfo);
  //   object->softBody->setCollisionShape(createConvexHullCollider(polyData).get());
  object->name = fileName;

  std::cout << "Terminado." << std::endl;

  return object;
}

std::shared_ptr<SceneSoftObject> ModelLoader::LoadTXTSoft(
    std::string fileName, btSoftBodyWorldInfo worldInfo) {
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
  actor->SetMapper(mapper);
  std::cout << "Terminado." << std::endl;

  // Crear SceneSoftObject
  std::cout << "\tCreando SceneSoftObject...";

  std::shared_ptr<SceneSoftObject> object(new SceneSoftObject(actor));
  object->UpdateSoftBody(worldInfo);
  //   object->softBody->setCollisionShape(createConvexHullCollider(polyData).get());
  object->name = fileName;
  std::cout << "Terminado." << std::endl;

  return object;
}