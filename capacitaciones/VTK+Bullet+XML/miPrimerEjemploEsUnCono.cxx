
#include <iostream>
#include <pthread.h>
#include <unistd.h>

#include <btBulletDynamicsCommon.h>


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
#include <vtkMatrix4x4.h>
#include <vtkTransformFilter.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>


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





void * actualizar(void * att)
{
				sleep(1);
				btBroadphaseInterface* broadphase = new btDbvtBroadphase();

        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0, -10, 0));


        btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);

        btCollisionShape* fallShape = new btSphereShape(1);


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

				double delta = 50.0;

        for (int i = 0; i < 300; i++) {
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
								vtkSmartPointer<vtkTransform> transform1a =
									vtkSmartPointer<vtkTransform>::New();
								transform1a->PostMultiply();
								transform1a->Translate(0.0, trans.getOrigin().getY() , 0.0);
								delta = trans.getOrigin().getY();
		
								actor->SetUserTransform(transform1a);
								renderWindow->Render();

                std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
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
	//<leer>
	vtkSmartPointer<vtkXMLGenericDataObjectReader> reader =
    vtkSmartPointer<vtkXMLGenericDataObjectReader>::New();
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
  vtkPolyData * output = reader->GetPolyDataOutput();
    std::cout<<output->GetNumberOfPoints()<<std::endl;
  vtkSmartPointer<vtkNamedColors> colors =
    vtkSmartPointer<vtkNamedColors>::New();
  for(vtkIdType i =0; i<output->GetNumberOfPoints(); i++ ){
  		double p[3];
  		output ->GetPoint(i,p);
  		std:: cout<<"Point "<<i<<": ("<<p[0]<<","<<p[1]<<","<<p[2]<<")"<<std::endl;
  }
  mapper->SetInputConnection(reader->GetOutputPort());
  
  //</leer>
  
	//coneSource->SetResolution( 40 );
	  //coneSource->SetHeight( 3 );
	  //coneSource->SetRadius( 3 );
	  //coneSource->SetCenter( 0.0, 50.0, 0.0 );
	  //coneSource->Update();

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

