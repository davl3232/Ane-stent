#ifndef INSTANCE_OF
#define INSTANCE_OF

#define InstanceOf(ObjectPtr, Class) \
  (std::dynamic_cast<Class *>(&Object) != NULL)
#endif