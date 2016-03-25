/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Operations for complex side-centered patch data.
 *
 ************************************************************************/

#ifndef included_math_PatchSideDataOpsComplex
#define included_math_PatchSideDataOpsComplex

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/SideData.h"
#include "SAMRAI/math/PatchSideDataBasicOps.h"
#include "SAMRAI/math/PatchSideDataNormOpsComplex.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/tbox/PIO.h"
#include "SAMRAI/tbox/Complex.h"
#include "SAMRAI/tbox/Pointer.h"

#include <iostream>

namespace SAMRAI {
namespace math {

/**
 * Class PatchSideDataOpsComplex provides a collection of operations
 * that may be used to manipulate complex side-centered patch data.  The
 * operations include basic arithmetic and norms.  With the
 * assertion of a few basic routines, this class inherits its interface (and
 * thus its functionality) from the base classes PatchSideDataBasicOps,
 * PatchSideDataNormOpsComplex from which it is derived.  The
 * name of each of these base classes is indicative of the set of
 * side-centered patch data operations that it provides.
 *
 * A similar set of operations is implemented for real (double and float) and
 * integer patch data in the classes PatchSideDataOpsReal and
 * PatchSideDataOpsInteger, repsectively.
 *
 * @see math::PatchSideDataBasicOps
 * @see math::PatchSideDataNormOpsComplex
 */

class PatchSideDataOpsComplex:
   public tbox::DescribedClass,
   public PatchSideDataBasicOps<dcomplex>,
   public PatchSideDataNormOpsComplex
{
public:
   /**
    * Empty constructor and destructor.
    */
   PatchSideDataOpsComplex();

   virtual ~PatchSideDataOpsComplex();

   /**
    * Copy dst data to src data over given box.
    */
   void
   copyData(
      tbox::Pointer<pdat::SideData<dcomplex> >& dst,
      const tbox::Pointer<pdat::SideData<dcomplex> >& src,
      const hier::Box& box) const;

   /**
    * Swap pointers for patch data objects.  Objects are checked for
    * consistency of depth, box, and ghost box.
    */
   void
   swapData(
      tbox::Pointer<hier::Patch> patch,
      const int data1_id,
      const int data2_id) const;

   /**
    * Print data entries over given box to given output stream.
    */
   void
   printData(
      const tbox::Pointer<pdat::SideData<dcomplex> >& data,
      const hier::Box& box,
      std::ostream& s = tbox::plog) const;

   /**
    * Initialize data to given scalar over given box.
    */
   void
   setToScalar(
      tbox::Pointer<pdat::SideData<dcomplex> >& dst,
      const dcomplex& alpha,
      const hier::Box& box) const;

private:
   // The following are not implemented:
   PatchSideDataOpsComplex(
      const PatchSideDataOpsComplex&);
   void
   operator = (
      const PatchSideDataOpsComplex&);

};

}
}
#endif