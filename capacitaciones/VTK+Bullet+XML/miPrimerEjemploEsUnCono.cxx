#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <fstream>


#include <btBulletDynamicsCommon.h>

#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkGeometryFilter.h>
#include <vtkPointSource.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkGenericDataObjectReader.h>
#include <vtkXMLGenericDataObjectReader.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkStructuredGrid.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformFilter.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>


std::vector <std::vector < double > > puntos;


vtkSmartPointer<vtkXMLGenericDataObjectReader> reader =
	vtkSmartPointer<vtkXMLGenericDataObjectReader>::New();

vtkSmartPointer<vtkSphereSource> coneSource =
	vtkSmartPointer<vtkSphereSource>::New();

//vtkSmartPointer<vtkPolyDataMapper> mapper =
//    vtkSmartPointer<vtkPolyDataMapper>::New();
vtkSmartPointer<vtkDataSetMapper> mapper =
    vtkSmartPointer<vtkDataSetMapper>::New();

vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();

vtkSmartPointer<vtkRenderWindow> renderWindow =
    vtkSmartPointer<vtkRenderWindow>::New();

vtkSmartPointer<vtkActor> actor =
    vtkSmartPointer<vtkActor>::New();

vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();

		vtkSmartPointer<vtkGeometryFilter> geometryFilter =
	    vtkSmartPointer<vtkGeometryFilter>::New();

			vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
		vtkSmartPointer<vtkDataSetSurfaceFilter>::New();

		vtkSmartPointer<vtkGenericDataObjectReader> reader2 =
		 vtkSmartPointer<vtkGenericDataObjectReader>::New();
vtkPolyData * output;


void * actualizar(void * att)
{
				sleep(1);
				btBroadphaseInterface* broadphase = new btDbvtBroadphase();

        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));


        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 20);

        btConvexHullShape* fallShapeConvexHull = new btConvexHullShape();
				//vtkPolyData* output = reader2->GetPolyDataOutput();
				for(int i =0; i < output->GetNumberOfPoints(); i++ ){
						fallShapeConvexHull->addPoint(btVector3(puntos[i][0], puntos[i][1], puntos[i][2]));
				}


        btCollisionShape* fallShape = fallShapeConvexHull;

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
        btRigidBody::btRigidBodyConstructionInfo
                groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
        dynamicsWorld->addRigidBody(groundRigidBody);


        btDefaultMotionState* fallMotionState =
                new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
        btScalar mass = 1;
        btVector3 fallInertia(0, 0, 0);
        fallShape->calculateLocalInertia(mass, fallInertia);
        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
        btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);

        //edit
        //fallRigidBody->setRestitution(0.5);
        //
        dynamicsWorld->addRigidBody(fallRigidBody);


        for (int i = 0; i < 500; i++) {
                dynamicsWorld->stepSimulation(1 / 60.f, 10);

                btTransform trans;
                fallRigidBody->getMotionState()->getWorldTransform(trans);

								//aca se pone.
								//coneSource->SetCenter( 0,  trans.getOrigin().getY(), 0);
								//coneSource->Update();
								//mapper->SetInputConnection(coneSource->GetOutputPort());
								//actor->SetMapper(mapper);
								//renderer->AddActor(actor);
								//actor->SetPosition(0.0,trans.getOrigin().getY(),0.0);


								btQuaternion rotado = trans.getRotation();

								vtkSmartPointer<vtkTransform> transform1a =
									vtkSmartPointer<vtkTransform>::New();
								transform1a->PostMultiply();


								transform1a->RotateWXYZ(rotado.getAngle(),rotado.getAxis().getX(),rotado.getAxis().getY(),rotado.getAxis().getZ());
								transform1a->Translate(trans.getOrigin().getX() , trans.getOrigin().getY() , trans.getOrigin().getZ());
								transform1a->Update();
								actor->SetUserTransform(transform1a);
								renderWindow->Render();

                //std::cout << "rotation: " << rotado.getAngle() << " " << rotado.getAxis().getX() << " " << rotado.getAxis().getY() << " " << rotado.getAxis().getZ() << std::endl;
                usleep(1000);
        }

        dynamicsWorld->removeRigidBody(fallRigidBody);
        delete fallRigidBody->getMotionState();
        delete fallRigidBody;

        dynamicsWorld->removeRigidBody(groundRigidBody);
        delete groundRigidBody->getMotionState();
        delete groundRigidBody;


        delete fallShape;

        delete groundShape;


        delete dynamicsWorld;
        delete solver;
        delete collisionConfiguration;
        delete dispatcher;
        delete broadphase;
	pthread_exit(NULL);
}


int main (int argc, char * argv [])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " InputFilename" << std::endl;
    return EXIT_FAILURE;
	}
	//leer cualquier tipo de archivo
	ifstream myReadFile;
	 myReadFile.open(argv[1]);
	 char file[100];
	 if (myReadFile.is_open()) {
    myReadFile >> file;
		 	if(file[0] == '<'){
				std::cout<<"xml"<<std::endl;

			  reader->SetFileName(argv[1]);
			  reader->Update();

				if(vtkPolyData::SafeDownCast(reader->GetOutput()))
				{
					std::cout << "File is a polydata" << std::endl;

				}
				else if(vtkUnstructuredGrid::SafeDownCast(reader->GetOutput()))
				{
					std::cout << "File is an unstructured grid" << std::endl;
			  }
				output = reader->GetPolyDataOutput();
				  mapper->SetInputConnection(reader->GetOutputPort());
			}else{
					std::cout<<"texto"<<std::endl;
					reader2->SetFileName(argv[1]);
					reader2->Update();

					if(reader2->IsFilePolyData())
				     {
				     std::cout << "output is a polydata" << std::endl;
				    output = reader2->GetPolyDataOutput();
				     std::cout << "output has " << output->GetNumberOfPoints() << " points." << std::endl;

					 }else{
						 vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid =
				    vtkSmartPointer<vtkUnstructuredGrid>::New();
						geometryFilter->SetInputData(unstructuredGrid);
				  	geometryFilter->Update();
						 surfaceFilter->SetInputData(unstructuredGrid);
						 surfaceFilter->Update();
						 std::cout << "output has " << output->GetNumberOfPoints() << " points." << std::endl;
						 output = surfaceFilter->GetOutput();
					 }
					  mapper->SetInputConnection(reader2->GetOutputPort());
			}
	 }


					    std::cout<<output->GetNumberOfPoints()<<std::endl;
					  vtkSmartPointer<vtkNamedColors> colors =
					    vtkSmartPointer<vtkNamedColors>::New();
					  int cont=0;
					  for(vtkIdType i =0; i<output->GetNumberOfPoints(); i++ ){
					  		double p[3];
					  		output ->GetPoint(i,p);
					  		std::vector <double> nuevo;
					  		nuevo.push_back(p[0]);
					  		nuevo.push_back(p[1]);
					  		nuevo.push_back(p[2]);
					  		puntos.push_back(nuevo);
					  		cont++;
					  		//std:: cout<<"Point "<<i<<": ("<<p[0]<<","<<p[1]<<","<<p[2]<<")"<<std::endl;
					  }

	vtkSmartPointer<vtkAxesActor> axes =
    vtkSmartPointer<vtkAxesActor>::New();
    axes->SetTotalLength( 1, 1 , 1 );
    axes->SetTipTypeToUserDefined();
    axes->AxisLabelsOff();


  //mapper->SetInputConnection(coneSource->GetOutputPort());


  actor->SetMapper(mapper);
	actor->GetProperty()->SetColor(colors->GetColor3d("Moccasin").GetData());

	vtkSmartPointer<vtkTransform> transform1a =
    vtkSmartPointer<vtkTransform>::New();
  transform1a->PostMultiply();
  transform1a->Translate(0.0, 50.0, 0.0);

  actor->SetUserTransform(transform1a);

  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize( 600, 600 );

  renderWindowInteractor->SetRenderWindow(renderWindow);

  //Add the actors to the scene
  renderer->AddActor(actor);
  renderer->AddActor(axes);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

 	pthread_t hilo;
	int rc = pthread_create(&hilo, NULL, actualizar, NULL);
	if (rc)
	{
		perror("Hola mundo");
		exit(-1);
	}

  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();



  pthread_exit(NULL);
}
