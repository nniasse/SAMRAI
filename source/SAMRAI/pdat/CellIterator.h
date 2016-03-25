/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Iterator for cell centered patch data types
 *
 ************************************************************************/

#ifndef included_pdat_CellIterator
#define included_pdat_CellIterator

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/CellGeometry.h"
#include "SAMRAI/pdat/CellIndex.h"
#include "SAMRAI/hier/Box.h"

namespace SAMRAI {
namespace pdat {

/**
 * Class CellIterator is an iterator that provides methods for
 * stepping through the index space associated with a cell centered box.
 * The indices are enumerated in column-major (e.g., Fortran) order.
 * The iterator should be used as follows:
 * \verbatim
 * hier::Box box;
 * ...
 * for (CellIterator c(box); c; c++) {
 *    // use index c of the box
 * }
 * \endverbatim
 * Note that the cell iterator may not compile to efficient code, depending
 * on your compiler.  Many compilers are not smart enough to optimize the
 * looping constructs and indexing operations.
 *
 * @see pdat::CellData
 * @see pdat::CellGeometry
 * @see pdat::CellIndex
 */

class CellIterator
{
public:
   /**
    * Constructor for the cell iterator.  The iterator will enumerate
    * the indices in the argument box.
    */
   explicit CellIterator(
      const hier::Box& box);

   /**
    * Copy constructor for the cell iterator
    */
   CellIterator(
      const CellIterator& iterator);

   /**
    * Assignment operator for the cell iterator.
    */
   CellIterator&
   operator = (
      const CellIterator& iterator);

   /**
    * Destructor for the cell iterator.
    */
   ~CellIterator();

   /**
    * Extract the cell index corresponding to the iterator position in the box.
    */
   const CellIndex&
   operator * () const;

   /**
    * Extract the cell index corresponding to the iterator position in the box.
    */
   const CellIndex&
   operator () () const;

   /**
    * Return true if the iterator points to a valid index within the box.
    */
   operator bool () const;

#ifndef LACKS_BOOL_VOID_RESOLUTION
   /**
    * Return a non-NULL if the iterator points to a valid index within the box.
    */
   operator const void
   * () const;
#endif

   /**
    * Return whether the iterator points to a valid index within the box.
    * This operator mimics the !p operation applied to a pointer p.
    */
   bool
   operator ! () const;

   /**
    * Increment the iterator to point to the next index in the box.
    */
   void
   operator ++ (
      int);

   /**
    * Test two iterators for equality (same index value).
    */
   bool
   operator == (
      const CellIterator& iterator) const;

   /**
    * Test two iterators for inequality (different index values).
    */
   bool
   operator != (
      const CellIterator& iterator) const;

private:
   CellIndex d_index;
   hier::Box d_box;
};

}
}
#ifdef SAMRAI_INLINE
#include "SAMRAI/pdat/CellIterator.I"
#endif
#endif