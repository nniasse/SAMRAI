/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Basic templated edge-centered patch data operations.
 *
 ************************************************************************/

#ifndef included_math_PatchEdgeDataBasicOps
#define included_math_PatchEdgeDataBasicOps

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/EdgeData.h"
#include "SAMRAI/tbox/Complex.h"
#include "SAMRAI/math/ArrayDataBasicOps.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/tbox/Pointer.h"

namespace SAMRAI {
namespace math {

/**
 * Class PatchEdgeDataBasicOps provides access to a collection
 * of basic numerical operations that may be applied to numerical edge-
 * centered patch data.  These operations include simple arithmetic
 * operations as well as min and max, etc.  The primary intent of this
 * class is to provide the interface to these standard operations for
 * an PatchEdgeDataOps<DIM> object which provides access to a complete set
 * of operations that may be used to manipulate edge-centered patch data
 * objects.   Each member function accepts a box argument indicating the
 * region of index space on which the operation should be performed.  The
 * operation will be performed on the intersection of this box and those
 * boxes corresponding to the patch data objects involved.
 *
 * These operations typically apply only to the numerical standard built-in
 * types, such as double, float, and int, and the complex type (which may or
 * may not be a built-in type depending on the C++ compiler).  Thus, this
 * templated class should only be used to instantiate objects with those
 * types as the template parameter.  None of the operations are implemented
 * for any other type.
 *
 * @see math::ArrayDataBasicOps
 */

template<class TYPE>
class PatchEdgeDataBasicOps
{
public:
   /**
    * Empty constructor and destructor.
    */
   PatchEdgeDataBasicOps();

   virtual ~PatchEdgeDataBasicOps<TYPE>();

   /**
    * Set dst = alpha * src, elementwise.
    */
   void
   scale(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const TYPE& alpha,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src,
      const hier::Box& box) const;

   /**
    * Set dst = src + alpha, elementwise.
    */
   void
   addScalar(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src,
      const TYPE& alpha,
      const hier::Box& box) const;

   /**
    * Set dst = src1 + src2, elementwise.
    */
   void
   add(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = src1 - src2, elementwise.
    */
   void
   subtract(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = src1 * src2, elementwise.
    */
   void
   multiply(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = src1 / src2, elementwise.  No check for division by zero.
    */
   void
   divide(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = 1 / src, elementwise.  No check for division by zero.
    */
   void
   reciprocal(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src,
      const hier::Box& box) const;

   /**
    * Set dst = alpha * src1 + beta * src2, elementwise.
    */
   void
   linearSum(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const TYPE& alpha,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const TYPE& beta,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = alpha * src1 + src2, elementwise.
    */
   void
   axpy(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const TYPE& alpha,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Set dst = alpha * src1 - src2, elementwise.
    */
   void
   axmy(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const TYPE& alpha,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src1,
      const tbox::Pointer<pdat::EdgeData<TYPE> >& src2,
      const hier::Box& box) const;

   /**
    * Return the minimum patch data component entry  When the data is
    * complex, the result is the data element with the smallest norm.
    */
   TYPE
   min(
      const tbox::Pointer<pdat::EdgeData<TYPE> >& data,
      const hier::Box& box) const;

   /**
    * Return the maximum patch data component entry  When the data is
    * complex, the result is the data element with the largest norm.
    */
   TYPE
   max(
      const tbox::Pointer<pdat::EdgeData<TYPE> >& data,
      const hier::Box& box) const;

   /**
    * Set patch data to random values.  See the operations in the
    * ArrayDataBasicOps class for details on the generation
    * of the random values for each data type.
    */
   void
   setRandomValues(
      tbox::Pointer<pdat::EdgeData<TYPE> >& dst,
      const TYPE& width,
      const TYPE& low,
      const hier::Box& box) const;

private:
   // The following are not implemented:
   PatchEdgeDataBasicOps(
      const PatchEdgeDataBasicOps<TYPE>&);
   void
   operator = (
      const PatchEdgeDataBasicOps<TYPE>&);

   ArrayDataBasicOps<TYPE> d_array_ops;
};

}
}

#ifdef INCLUDE_TEMPLATE_IMPLEMENTATION
#include "SAMRAI/math/PatchEdgeDataBasicOps.C"
#endif

#endif