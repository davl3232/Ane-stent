#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

class Converter {
public:
  vtkSmartPointer<vtkActor>
  PolyDataToActor(vtkSmartPointer<vtkPolyData> polyData);
};
#endif