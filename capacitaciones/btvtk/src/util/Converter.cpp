#include "Converter.h"

#include <fstream>
#include <iostream>
#include <string>

#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkXMLGenericDataObjectReader.h>

vtkSmartPointer<vtkActor>
Converter::PolyDataToActor(vtkSmartPointer<vtkPolyData> polyData) {}