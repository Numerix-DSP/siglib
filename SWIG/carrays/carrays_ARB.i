/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * carrays_ARB.i
 *
 * SWIG library file containing macros that can be used to manipulate simple
 * pointers as arrays.
 *
 * MODIFIED ABIAGIONI@DUKE-RIVER.COM to make the arrays safe from overflow, as
 * follows:
 * - the actual arrays are created of size = N+1
 * - array[0] stores the array size
 * - each read operation compares the passed index to array[0]; returns 0 if
 *   the index is >= array[0], otherwise returns array[index+1]
 * - each write operation compares the passed index to array[0]; does nothing if
 *   the index is >= array[0], otherwise updates array[index+1]
 *
 * NOTE:  this is a very-low-level approach to array safety...
 * ----------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 * %array_functions(TYPE,NAME)
 *
 * Generates functions for creating and accessing elements of a C array
 * (as pointers).  Creates the following functions:
 *
 *        TYPE *new_NAME(int nelements)
 *        void delete_NAME(TYPE *);
 *        TYPE NAME_getitem(TYPE *, int index);
 *        void NAME_setitem(TYPE *, int index, TYPE value);
 *
 * ----------------------------------------------------------------------------- */

%define %array_functions(TYPE,NAME)
%{
static TYPE *new_##NAME(int nelements) { %}
#ifdef __cplusplus
%{  TYPE *ary = new TYPE[nelements+1]; ary[0] = nelements; return ary+1; %}
#else
%{  TYPE *ary = calloc(nelements+1,sizeof(TYPE)); ary[0] = nelements; return (ary+1); %}
#endif
%{}

static void delete_##NAME(TYPE *ary) { %}
#ifdef __cplusplus
%{  delete [] (ary-1); %}
#else
%{  free(ary-1); %}
#endif
%{}

static TYPE NAME##_getitem(TYPE *ary, int index) {
    if ( index >= (ary-1)[0] ) return (TYPE)0;
    return ary[index];
}
static void NAME##_setitem(TYPE *ary, int index, TYPE value) {
    if ( index < (ary-1)[0] )
        ary[index] = value;
}
%}

TYPE *new_##NAME(int nelements);
void delete_##NAME(TYPE *ary);
TYPE NAME##_getitem(TYPE *ary, int index);
void NAME##_setitem(TYPE *ary, int index, TYPE value);

%enddef

/* -----------------------------------------------------------------------------
 * %array_class(TYPE,NAME)
 *
 * Generates a class wrapper around a C array.  The class has the following
 * interface:
 *
 *          struct NAME {
 *              NAME(int nelements);
 *             ~NAME();
 *              TYPE getitem(int index);
 *              void setitem(int index, TYPE value);
 *              TYPE * cast();
 *              static NAME *frompointer(TYPE *t);
  *         }
 *
 * ----------------------------------------------------------------------------- */

%define %array_class(TYPE,NAME)
%{
typedef TYPE NAME;
%}
typedef struct NAME {
  /* Put language specific enhancements here */

} NAME;

%extend NAME {

#ifdef __cplusplus
NAME(int nelements) {
  TYPE *ary = new TYPE[nelements+1]; ary[0] = nelements; return ary;
}
~NAME() {
  delete [] self;
}
#else
NAME(int nelements) {
  TYPE *ary = calloc(nelements+1,sizeof(TYPE)); ary[0] = nelements; return ary;
}
~NAME() {
  free(self);
}
#endif

TYPE getitem(int index) {
  if ( index >= self[0] ) return (TYPE)0;
  return self[index+1];
}
void setitem(int index, TYPE value) {
  if ( index < self[0] )
    self[index+1] = value;
}
TYPE * cast() {
  return self;
}
static NAME *frompointer(TYPE *t) {
  return (NAME *) t;
}

};

%types(NAME = TYPE);

%enddef
