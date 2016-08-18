/*
    File: fli.h
*/

/*
Copyright (c) 2016, Christian E. Schafmeister
Copyright (c) 2016, Frank Gönninger

CLASP is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

See directory 'clasp/licenses' for full details.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* -^- */

// ===========================================================================
//
//  FLI - Foreign Language Interface for allocating, setting,
//        getting and freeing foreign memory and calling foreign
//        functions.
//
//  Augiust 2016 by Frank Goenninger, Gönninger B&T UG, Germany
//
// ===========================================================================

// --- IMPLEMEMTATION NOTES ---
//
// The complete FLI is comprised of the following files:
// .../src/core/fli.cc            - corresponding .cc file
// .../include/clasp/core/fli.h   - this file
// .../src/lisp/kernel/fli.lsp    - Lisp land macros and functions
//
// --- END OF IMPLEMEMTATION NOTES ---

#if !defined(__CLASP_CORE_FLI_H__)
#define __CLASP_CORE_FLI_H__ __FILE__" $Id$"

// ---------------------------------------------------------------------------
//    SYSTEM INCLUDES
// ---------------------------------------------------------------------------

// --- Standard C++ Includes ---
// NONE

// --- Platform-specific Includes ---
// NONE

// ---------------------------------------------------------------------------
//    APPLICATION INCLUDES
// ---------------------------------------------------------------------------

#include <clasp/core/foundation.h>
#include <clasp/core/clasp_ffi_package.fwd.h>
#include <clasp/core/vectorObjects.h>
#include <clasp/core/externalObject.h>

#if defined(__cplusplus)

// ---------------------------------------------------------------------------
//   NAMESPACE
// ---------------------------------------------------------------------------

namespace clasp_ffi {

// ---------------------------------------------------------------------------
//   LISP SYMBOLS
// ---------------------------------------------------------------------------

  // Keyword symbols designating ForeignData_O kinds
  SYMBOL_EXPORT_SC_(KeywordPkg,clasp_foreign_data_kind_dynamic_library);
  SYMBOL_EXPORT_SC_(KeywordPkg,clasp_foreign_data_kind_built_in_type);
  SYMBOL_EXPORT_SC_(KeywordPkg,clasp_foreign_data_kind_data);
  SYMBOL_EXPORT_SC_(KeywordPkg,clasp_foreign_data_kind_pointer);
  SYMBOL_EXPORT_SC_(KeywordPkg,clasp_foreign_data_kind_symbol_pointer);

  // The Foreign Type Spec Table, accessible from Lisp
  SYMBOL_EXPORT_SC_(Clasp_ffi_pkg,STARforeign_type_spec_tableSTAR);

// ---------------------------------------------------------------------------
//   TYPEDEFS & CLASSES & METHODS & FUNCTIONS
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
  // CLASS ForeignData_O

  SMART(ForeignData);

  class ForeignData_O : public core::ExternalObject_O {
    LISP_CLASS(clasp_ffi, Clasp_ffi_pkg,
               ForeignData_O, "ForeignData", core::ExternalObject_O);

  public:

    // CTOR & DTOR
    explicit ForeignData_O();
    virtual ~ForeignData_O(); // non-trivial

    // OVERLADED FUNCTIONS
    void *externalObject() const;
    bool eql_(core::T_sp obj) const;

    // SLOT ACCESS

    // -- SETTER & GETTER --
    const core::T_sp kind() { return m_kind; };
    void set_kind( core::T_sp kind ) { this->m_kind = kind; };

    const int ownership_flags() { return m_ownership_flags; };
    const void *orig_data_ptr() { return m_orig_data_ptr; };
    void *raw_data() { return m_raw_data; };

    // -- TRANSFORMING ACCESS --
    template <class T>
      T data() { return reinterpret_cast<T>(this->raw_data()); };

    // LISP EXPOSED SETTER & GETTER METHODS
    CL_DEFMETHOD core::T_sp PERCENTkind();
    CL_DEFMETHOD core::Integer_sp PERCENTownership_flags();
    CL_DEFMETHOD core::Integer_sp PERCENTforeign_data_address();

    // MAKE AND CREATE
    static ForeignData_sp create(const cl_intptr_t address = 0);
    static ForeignData_sp create(void * p_address = nullptr);

    CL_DEFMETHOD void PERCENTfree_foreign_object();
    CL_DEFMETHOD void PERCENTfree_foreign_data();

    // POINTER ADDRESS MANIPULATION
    CL_DEFMETHOD ForeignData_sp PERCENTinc_pointer(core::Integer_sp offset);

    // OBJECT PRINTING
    string __repr__() const;
    bool null_pointer_p();

    // MENORY MGMT
    void allocate(core::T_sp kind, core::ForeignDataFlagEnum ownership_flags, size_t size);
    void free();

  private:

    // SLOTS
    core::T_sp m_kind;
    core::ForeignDataFlagEnum m_ownership_flags;
    size_t m_size;

    void *m_orig_data_ptr;
    // If we allocate memory then we save the ptr to the original address.
    // This enables changing the pointer of m_raw_data without loosing the
    // ability to free the originally allocated memory.

    void *m_raw_data;

  }; // ForeignData_O

  // ---------------------------------------------------------------------------
  // MAKE AND TEST FOREIGN DATA POINTER
  CL_DEFUN ForeignData_sp PERCENTallocate_foreign_object(core::T_sp kind);
  CL_DEFUN ForeignData_sp PERCENTallocate_foreign_data(core::Integer_sp size);

  CL_DEFUN ForeignData_sp PERCENTmake_pointer(core::T_sp address);
  CL_DEFUN core::T_sp PERCENTpointerp( core::T_sp obj );

  CL_DEFUN ForeignData_sp PERCENTmake_nullpointer();
  CL_DEFUN core::T_sp PERCENTnull_pointer_p( core::T_sp obj );

  // ---------------------------------------------------------------------------
  // FOREIGN TYPE SIZE AND ALIGNMENT
  CL_DEFUN core::Fixnum_sp PERCENTforeign_type_alignment(core::Symbol_sp atype);
  CL_DEFUN core::Fixnum_sp PERCENTforeign_type_size(core::Symbol_sp atype);

  // LISP MEMORY ACEESS / MEMORY CONTENT CONVERSION
  CL_DEFUN core::T_sp PERCENTmem_ref( core::T_sp address_or_foreign_data_ptr,
                                      core::T_sp type,
                                      core::Integer_sp offset);
  CL_DEFUN void PERCENTmem_set( core::T_sp address_or_foreign_data_ptr,
                                core::T_sp type,
                                core::Integer_sp offset,
                                core::T_sp value);

  // ---------------------------------------------------------------------------
  // FOREIGN MEMORY DIRECT ACCESS
  template <class T>
    T mem_ref(cl_intptr_t address);

  // ---------------------------------------------------------------------------
  // DYNAMIC LIBRARY HANDLING
  CL_DEFUN core::T_sp PERCENTdlopen( core::T_sp path_designator);
  CL_DEFUN core::T_sp PERCENTdlclose( ForeignData_sp handle );
  CL_DEFUN core::T_sp PERCENTdlsym( core::Str_sp name );

}; // namespace clasp_ffi

// GC Policy Info for ForeignPataPtr_O instances
  template <>
    struct gctools::GCInfo<clasp_ffi::ForeignData_O> {
    static bool constexpr NeedsInitialization = false;
    static bool constexpr NeedsFinalization = true;
    static GCInfo_policy constexpr Policy = normal;
  };

namespace clasp_ffi {

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
  // CLASS ForeignTypeSpec_O

  SMART(ForeignTypeSpec);

  class ForeignTypeSpec_O : public core::General_O {
    LISP_CLASS(clasp_ffi, Clasp_ffi_pkg,
               ForeignTypeSpec_O, "ForeignTypeSpec", core::General_O);

  public:

    // CTOR & DTOR
    explicit ForeignTypeSpec_O();
    virtual ~ForeignTypeSpec_O();

    // OVERLADED FUNCTIONS
    bool eql_(ForeignTypeSpec_sp sp_obj) const;

    // OBJECT PRINTING
    string __repr__() const;

    // MAKE AND CREATE - LISP EXPOSED FUNCTIONS

    static ForeignTypeSpec_sp create( core::Symbol_sp   lisp_symbol,
                                      core::Str_sp      lisp_name,
                                      core::Integer_sp  size,
                                      core::Fixnum_sp   alignment,
                                      core::Str_sp      cxx_name );

    // SLOT ACCESS
    CL_DEFMETHOD core::Symbol_sp   PERCENTlisp_symbol() { return m_lisp_symbol; };
    CL_DEFMETHOD core::Str_sp      PERCENTlisp_name() { return m_lisp_name; };
    CL_DEFMETHOD core::Integer_sp  PERCENTsize() { return m_size; };
    CL_DEFMETHOD core::Fixnum_sp   PERCENTalignment() { return m_alignment; };
    CL_DEFMETHOD core::Str_sp      PERCENTcxx_name() { return m_cxx_name; };

    // SLOTS
    core::Symbol_sp   m_lisp_symbol;
    core::Str_sp      m_lisp_name;
    core::Integer_sp  m_size;
    core::Fixnum_sp   m_alignment;
    core::Str_sp      m_cxx_name;

  }; // ForeignTypeSpec_O

}; // namespace clasp_fffi

// GC Policy Info for ForeignTypeSpec_O instances
template <>
struct gctools::GCInfo<clasp_ffi::ForeignTypeSpec_O> {
  static bool constexpr NeedsInitialization = false;
  static bool constexpr NeedsFinalization = false;
  static GCInfo_policy constexpr Policy = normal;
};


// ---------------------------------------------------------------------------
//   END OF FILE
// ---------------------------------------------------------------------------

#endif // __cplusplus
#endif // __CLASP_CORE_FLI_H__