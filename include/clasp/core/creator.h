#ifndef creator_h
#define creator_h


namespace core {
  FORWARD(Creator);
  FORWARD(InstanceCreator);
};

template <>
struct gctools::GCInfo<core::Creator_O> {
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};

template <>
struct gctools::GCInfo<core::InstanceCreator_O> {
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};


namespace core {

  class Creator_O : public General_O {
    LISP_CLASS(core,CorePkg,Creator_O,"Creator",General_O);
  public:
  // Some Creators don't actually allocate anything -
  // classes that don't have default allocators
    virtual bool allocates() const { return true; };
  /*! If this is the allocator for a primary CxxAdapter class then return true, */
    virtual int duplicationLevel() const { return 0; };
    virtual size_t templatedSizeof() const = 0;
    virtual Creator_sp duplicateForClassName(core::Symbol_sp className) {
      printf("Subclass must implement Creator::duplicateForClassName\n");
      abort();
    };
    virtual void describe() const = 0;
    virtual core::T_sp allocate() = 0;
  };



    template <class _W_>
    class LispObjectCreator : public core::Creator_O {
  public:
    typedef core::Creator_O TemplatedBase;
  public:
    DISABLE_NEW();
    size_t templatedSizeof() const { return sizeof(LispObjectCreator<_W_>); };
    virtual void describe() const {
      printf("LispObjectCreator for class %s  sizeof_instances-> %zu\n", _rep_(reg::lisp_classSymbol<_W_>()).c_str(), sizeof(_W_));
    }
    virtual core::T_sp allocate() {
      GC_ALLOCATE(_W_, obj);
      return obj;
    }
    virtual void searcher(){};
  };
};

template <typename T>
class gctools::GCKind<core::LispObjectCreator<T>> {
 public:
  static gctools::GCKindEnum const Kind = gctools::GCKind<typename core::LispObjectCreator<T>::TemplatedBase>::Kind;
};
template <typename T>
struct gctools::GCInfo<core::LispObjectCreator<T>> {
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};

namespace core {
  class InstanceCreator_O : public Creator_O {
    LISP_CLASS(core,CorePkg,InstanceCreator_O,"InstanceCreator",Creator_O);
  public:
    Symbol_sp _className;
  public:
  InstanceCreator_O(Symbol_sp className) : _className(className){};
    void describe() const {
      printf("InstanceAllocatorFunctor for class %s\n", _rep_(this->_className).c_str());
    };
    T_sp allocate();
    virtual size_t templatedSizeof() const { return sizeof(InstanceCreator_O); };
  };
};



#endif // ifndef creator_h