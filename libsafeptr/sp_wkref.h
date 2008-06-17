
// -*- c++ -*-
#include "safeptr.h"

namespace sp {

  //=======================================================================

  template<class T> class referee;

  //=======================================================================

  template<class T>
  class wkref : public base_ptr<T> {
  public:
    wkref (T *o) : _obj (o) { linkup (); }
    wkref (T &o) : _obj (&o) { linkup (); }
    wkref (const wkref<T> &r) : _obj (r._obj) { linkup (); }

    wkref () : _obj (NULL) {}
    ~wkref () { clear (); }

    wkref<T> &operator= (const wkref<T> &w2) {
      clear ();
      _obj = w2._obj;
      linkup ();
      return *this;
    }
    
    wkref<T> &operator= (T* o) {
      clear ();
      _obj = o;
      linkup ();
      return *this;
    }

    wkref<T> &operator= (T &o) {
      clear ();
      _obj = o;
      linkup ();
      return *this;
    }

    friend class referee<T>;

  private:
    void clear () { if (_obj) _obj->rm (this); _obj = NULL; }
    void linkup () { if (_obj) { _obj->add (this); } }

    const T *obj () const { return _obj; }
    T *obj () { return _obj; }
    list_entry<wkref<T> > _lnk;


    T *_obj;
  };
  

  //=======================================================================

  template<class T>
  class referee {
  public:
    referee () {}
    ~referee ()
    {
      while (_lst.first) {
	wkref<T> *i = _lst.first;
	assert (*i);
	i->clear (); // will remove i from list!
      }
    }
    void add (wkref<T> *i) { _lst.insert_head (i); } 
    void rm (wkref<T> *i) { _lst.remove (i); } 
  private:
    list<wkref<T>, &wkref<T>::_lnk> _lst;
  };

  //=======================================================================
};
