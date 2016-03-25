/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   A container of boxes with basic domain calculus operations
 *
 ************************************************************************/

#ifndef included_hier_BoxContainer
#define included_hier_BoxContainer

#include "SAMRAI/SAMRAI_config.h"
#include "SAMRAI/hier/BlockId.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/IntVector.h"
#include "SAMRAI/tbox/Array.h"

#include <iostream>
#include <list>
#include <set>


namespace SAMRAI {
namespace hier {

class BoxContainerIterator;
class BoxContainerConstIterator;
class BoxTree;
class MultiblockBoxTree;

/*!
 * @brief A container for Boxes.
 *
 * The BoxContainer stores a collection of Boxes and provides methods for
 * access and manipulation of that collection.
 *
 * A BoxContainer exists in either an "ordered" or "unordered" state.
 * The ordered state means that the Boxes have been ordered according to
 * the comparison operators defined in the BoxId class, while the unordered
 * state has no meaningful ordering of the Boxes besides the sequence that the
 * Boxes were added to the container.  Additionally the ordered state
 * requires that all Boxes in the container have a valid and unique BoxId,
 * while there is no such restriction for unordered containers.
 *
 * An ordered container can always have its state switched to unordered by
 * a call to the unorder() method.  An unordered container can also have its
 * state switched to ordered by a call to the order() method, but only under
 * certain conditions specified below in the comments for order().
 *
 * Certain methods in this class can only be called on ordered containers
 * while others can only be called on unordered containers.  Violating these
 * restrictions will result in a run-time error.
 *
 * Regardless of unordered/unordered state, all Boxes within a BoxContainer
 * must be of the same Dimension.  If a new Box added to a container has
 * a different Dimension than the Boxes already in the container, an assertion
 * failure will occur.
 *
 * @see Box
 * @see BoxId
 */
class BoxContainer
{
friend class BoxContainerIterator;
friend class BoxContainerConstIterator;

public:

   /*!
    * @brief The iterator for class BoxContainer.
    */
   typedef BoxContainerIterator Iterator;

   /*!
    * @brief The const iterator for class BoxContainer.
    */
   typedef BoxContainerConstIterator ConstIterator;

   //@{ @name Constructors, Destructors, Assignment

   /*!
    * @brief Default constructor creates empty container in unordered state.
    */
   explicit BoxContainer();

   /*!
    * @brief Creates empty container in state determined by boolean
    *
    * @param[in] ordered  Container will be ordered if true, unordered if false.
    */
   explicit BoxContainer(const bool ordered);

   /*!
    * @brief Create container containing members from another container.
    *
    * Members in the range [first, last) are copied to new container.
    *
    * @param[in] first
    * @param[in] last
    * @param[in] ordered  Container will be ordered if true, unordered if false.
    */
   explicit BoxContainer(
      ConstIterator first,
      ConstIterator last,
      const bool ordered = false);

   /*!
    * @brief Create a container with 1 box.
    *
    * @param[in] box  Box to copy into new container.
    * @param[in] ordered  Container will be ordered if true, unordered if false.
    */
   explicit BoxContainer(
      const Box& box,
      const bool ordered = false);

   /*!
    * @brief Copy constructor from another BoxContainer.
    *
    * All boxes and the ordered/unordered state will be copied to the new
    * BoxContainer.
    *
    * @param[in] other
    */
   BoxContainer(
      const BoxContainer& other);

   /*!
    * @brief Copy constructor from an array of tbox::DatabaseBox objects.
    *
    * The new BoxContainer will be unordered.
    *
    * @param[in] other
    */
   explicit BoxContainer(
      const tbox::Array<tbox::DatabaseBox>& other);

   /*!
    * @brief Constructor that copies only Boxes having the given BlockId
    * from the other container.
    *
    * The unordered or ordered state will be the same as that of the argument
    * container.
    *
    * @param[in] other
    * @param[in] block_id
    */
   explicit BoxContainer(
      const BoxContainer& other,
      const BlockId& block_id);

   /*!
    * @brief Assignment from other BoxContainer.
    *
    * All boxes and the ordered/unordered state will be copied to the
    * assigned BoxContainer.  Any previous state of the assigned
    * BoxContainer is discarded.
    *
    * @param[in] rhs
    */
   BoxContainer&
   operator = (
      const BoxContainer& rhs);

   /*!
    * @brief Assignment from an array of tbox::DatabaseBox objects.
    *
    * The assigned BoxContainer will be unordered.  Any previous state of the
    * assigned BoxContainer is discarded.
    *
    * @param[in] rhs
    */
   BoxContainer&
   operator = (
      const tbox::Array<tbox::DatabaseBox>& rhs);

   /*!
    * @brief The destructor releases all storage.
    */
   ~BoxContainer();

   //@}

   //@{ @name Methods that may be called on ordered or unordered BoxContainers.

   /*!
    * @brief Return the number of boxes in the container.
    *
    * @return The number of boxes in the container.
    */
   int
   size() const;

   /*!
    * @brief Returns true if there are no boxes in the container
    *
    * @return True if the container is empty.
    */
   bool
   isEmpty() const;

   /*!
    * @brief Return a ConstIterator pointing to the start of the container.
    *
    * @return An immutable iterator pointing to the first box.
    */
   ConstIterator
   begin() const;

   /*!
    * @brief Return a ConstIterator pointing to the end of the container.
    *
    * @return An immutable iterator pointing beyond the last box.
    */
   ConstIterator
   end() const;

   /*!
    * @brief Return an Iterator pointing to the start of the container.
    *
    * @return A mutable iterator pointing to the first box.
    */
   Iterator
   begin();

   /*!
    * @brief Return an Iterator pointing to the end of the container.
    *
    * @return A mutable iterator pointing beyond the last box.
    */
   Iterator
   end();

   /*!
    * @brief Returns the first element in the container.
    *
    * @return A const reference to the first Box in the container.
    */
   const Box&
   front() const;

   /*!
    * @brief Returns the first element in the container.
    *
    * @return A const reference to the last Box in the container.
    */
   const Box&
   back() const;

   /*!
    * @brief Remove the member of the container pointed to by "iter".
    *
    * Can be called on ordered or unordered containers.
    *
    * @param[in] iter
    */
   void
   erase(
      Iterator iter);

   /*!
    * @brief Remove the members of the container in the range [first, last).
    *
    * Can be called on ordered or unordered containers.
    *
    * @param[in] first
    * @param[in] last
    */
   void
   erase(
      Iterator first,
      Iterator last);

   /*!
    * @brief Removes all the members of the container.
    *
    * Can be called on unordered or unordered containers.  Sets the state to
    * unordered.
    */
   void
   clear();

   /*!
    * @brief  Swap all contents and state with another BoxContainer.
    *
    * This container and other container exchange all member Boxes and
    * ordered/unordered state.
    *
    * @param[in,out] other  Other container for swap.
    */
   void
   swap(BoxContainer& other);

   /*!
    * @brief  Get all of the ranks that own Boxes in this container
    *
    * The rank of every member of this container is inserted into the set.
    *
    * @param[out] owners
    */
   void
   getOwners(
      std::set<int>& owners) const;

   /*!
    * @brief Grow boxes in the container by the specified ghost cell width.
    *
    * @param[in] ghosts
    */
   void
   grow(
      const IntVector& ghosts);

   /*!
    * @brief Shift boxes in the container by the specified offset.
    *
    * @param[in] offset
    */
   void
   shift(
      const IntVector& offset);

   /*!
    * @brief Refine boxes in container by the specified refinement ratio.
    *
    * @param[in] ratio
    */
   void
   refine(
      const IntVector& ratio);

   /*!
    * @brief Coarsen boxes in container by the specified coarsening ratio.
    *
    * @param[in] ratio
    */
   void
   coarsen(
      const IntVector& ratio);

   /*!
    * @brief Count total number of indices in the boxes in the container.
    *
    * @return Total number of indices of all boxes in the container.
    */
   int
   getTotalSizeOfBoxes() const;

   /*!
    * @brief Determine if "idx" lies within bounds of boxes in container.
    *
    * Only boxes of the given BlockId will be checked.
    *
    * @return true if idx lies within bounds of boxes in container.
    *
    * @param[in] idx
    * @param[in] block_id
    */
   bool
   contains(
      const Index& idx,
      const BlockId& block_id) const;

   /*!
    * @brief  Returns the bounding box for all the boxes in the container.
    *
    * A run-time error will occur if Boxes in this container have different
    * BlockIds.
    */
   Box
   getBoundingBox() const;

   /*!
    * @brief  Returns the bounding box for all the boxes in the container
    *         having the given BlockId.
    */
   Box
   getBoundingBox(const BlockId& block_id) const;

   /*!
    * @brief Check for non-empty intersection among boxes in container.
    *
    * @return Returns true if there exists any non-empty intersection among
    * the boxes in the container.
    */
   bool
   boxesIntersect() const;

   //@}

   //@{ @name Methods to change or query ordered/unordered state

   /*!
    * @brief Changes state of this container to ordered.
    *
    * This can be called on an unordered container with the restriction that
    * every member of the container must have a valid and unique BoxId.
    * If this restriction is not met, then a run-time error will occur.
    *
    * If called on a container that is already ordered, nothing changes.
    */
   void order();

   /*!
    * @brief Changes state of this container to unordered.
    *
    * This method can be called on any container.
    */
   void unorder();

   /*!
    * @brief Return whether this container is ordered.
    *
    * @return  True if ordered, false if unordered.
    */
   bool isOrdered() const;

   //@}

   //@{ Methods that may only be called on unordered containers.

   /*!
    * @brief Adds "item" to the "front" of the container.
    *
    * Makes "item" the member of the container that will be returned by
    * front() in an unordered container.
    *
    * @param[in] item
    */
   void
   pushFront(
      const Box& item);

   /*!
    * @brief Adds "item" to the "end" of the container.
    *
    * Makes "item" the member of the container that will be returned by
    * back() in an unordered container.
    *
    * @param[in] item
    */
   void
   pushBack(
      const Box& item);

   /*!
    * @brief Add "item" to specific place in the container.
    *
    * Places "item" immediately before the member of the container pointed
    * to by "iter" in an unordered container.
    *
    * @param[in] iter Location to add item before.
    * @param[in] item Box to add to container.
    */
   void
   insertBefore(
      Iterator iter,
      const Box& item);

   /*!
    * @brief Add "item" to specific place in the container.
    *
    * Places "item" immediately after the member of the container pointed
    * to by "iter" in an unordered container.
    *
    * @param[in] iter Location to add item after.
    * @param[in] item Box to add to container.
    */
   void
   insertAfter(
      Iterator iter,
      const Box& item);

   /*!
    * @brief Prepends the Boxes in "boxes" to this BoxContainer.
    *
    * "boxes" will be empty following this operation.
    *
    * @param[in] boxes
    */
   void
   spliceFront(
      BoxContainer& boxes);

   /*!
    * @brief Appends the Boxes in "boxes" to this BoxContainer.
    *
    * "boxes" will be empty following this operation.
    *
    * @param[in] boxes
    */
   void
   spliceBack(
      BoxContainer& boxes);

   /*!
    * @brief Remove the first member of the unordered container.
    */
   void
   popFront();

   /*!
    * @brief Remove the last member of the unordered container.
    */
   void
   popBack();

   /*!
    * @brief Place the boxes in the container into a canonical ordering.
    *
    * The canonical ordering for boxes is defined such that boxes that lie
    * next to each other in higher dimensions are coalesced together before
    * boxes that lie next to each other in lower dimensions.  This ordering
    * provides a standard representation that can be used to compare box
    * containers.  The canonical ordering also does not allow any overlap
    * between the boxes in the container.  This routine is potentially
    * expensive, since the running time is \f$O(N^2)\f$ for N boxes.  None
    * of the domain calculus routines call simplify(); all calls to simplify
    * the boxes must be explicit.  Note that this routine is distinct from
    * coalesce(), which is not guaranteed to produce a canonical ordering.
    */
   void
   simplify();

   /*!
    * @brief Combine any boxes in the container which may be coalesced.
    *
    * Two boxes may be coalesced if their union is a box (recall that boxes
    * are not closed under index set unions).  Empty boxes in the container
    * are removed during this process.  Note that this is potentially an
    * expensive calculation (e.g., it will require \f$(N-1)!\f$ box
    * comparisons for a box container with \f$N\f$ boxes in the worst
    * possible case).  So this routine should be used sparingly.  Also note
    * that this routine is different than simplify() since it does not
    * produce a canonical ordering.  In particular, this routine processes
    * the boxes in the order in which they appear in the container, rather
    * than attempting to coalesce boxes along specific coordinate directions
    * before others.
    */
   void
   coalesce();

   /*!
    * @brief Rotate boxes in container according to a RotationIdentifier
    *
    * Can only be called on an unordered container.  The reason it may
    * not be called on an ordered container is that it rotates the  
    * Boxes' spatial indices to a different coordinate system but does not
    * update their BlockId.  A run-time error will occur if called on a
    * container with member Boxes having different BlockId values.
    *
    * @note Works only in 2D or 3D.
    *
    * @param[in] rotation_ident
    */
   void
   rotate(
      const Transformation::RotationIdentifier rotation_ident);

   /*!
    * @brief Remove from each box the portions that intersect takeaway.
    *
    * This operation can be thought of as a set difference defined over the
    * abstract AMR box index space.  Performing the set difference will
    * require \f$O(N)\f$ time for a container with \f$N\f$ boxes.  For each
    * box, b, in this container this operation computes b-(b^takeaway) where
    * '^' indicates intersection.
    *
    * @param[in] takeaway What to exclude from each box in the container.
    */
   void
   removeIntersections(
      const Box& takeaway);

   /*!
    * @brief Remove from each box portions intersecting boxes in takeaway.
    *
    * For each box, b, in this container and for each box, t, in takeaway
    * this operation computes b-(b^t) where '^' indicates intersection.
    *
    * @param[in] takeaway What to exclude from each box in the container.
    */
   void
   removeIntersections(
      const BoxContainer& takeaway);

   /*!
    * @brief Remove from each box portions intersecting boxes in takeaway.
    *
    * BoxTree has an efficient overlap search method so this
    * version of removeIntersection is relatively fast.
    * For each box, b, in this container and for each box, t, in takeaway
    * this operation computes b-(b^t) where '^' indicates intersection.
    *
    * @param[in] takeaway What to exclude from each box in the container.
    */
   void
   removeIntersections(
      const BoxTree& takeaway);


   /*!
    * @brief Remove from each box portions intersecting boxes in takeaway.
    *
    * Use extra data to provide needed information in a multiblock setting.
    * MultiblockBoxTree has an efficient overlap search method so this
    * version of removeIntersection is relatively fast.
    *
    * @param[in] refinement_ratio  Assume all boxes in this BoxContainer
    * belong in this refinement ratio.
    *
    * @param[in] takeaway  The boxes to take away from this BoxContainer.
    *
    * @param[in] include_singularity_block_neighbors  
    */
   void
   removeIntersections(
      const IntVector& refinement_ratio,
      const MultiblockBoxTree& takeaway,
      const bool include_singularity_block_neighbors = false);

   /*!
    * @brief Remove from box the portions intersecting takeaway.
    *
    * This is special version for the case where the container is empty
    * initially.  Upon completion this container contains the result of the
    * removal from box of the intersection of box with takeaway.  If the
    * boxes do not intersect, box is simply added to this container.  This
    * routine is primarily suited for applications which are looking only
    * for the intersection of two boxes.  This operation computes
    * box-(box^takeaway) where '^' indicates intersection.
    *
    * @param[in] box
    * @param[in] takaway
    */
   void
   removeIntersections(
      const Box& box,
      const Box& takeaway);

   /*!
    * @brief Keep the intersection of the container's boxes and keep.
    *
    * Performing the intersection will require \f$O(N)\f$ time for a
    * container with \f$N\f$ boxes.  The complement of removeIntersections.
    *
    * @param[in] keep
    */
   void
   intersectBoxes(
      const Box& keep);

   /*!
    * @brief Keep the intersection of the container's boxes and keep's boxes
    *
    * Intersect the boxes in the current container against the boxes in the
    * specified container.  The intersection calculation will require
    * \f$O(N^2)\f$ time for containers with \f$N\f$ boxes.  The complement
    * of removeIntersections.
    *
    * @param[in] keep
    */
   void
   intersectBoxes(
      const BoxContainer& keep);

   /*!
    * @brief Keep the intersection of the container's boxes and keep's boxes
    *
    * BoxTree has an efficient overlap search method so this
    * version of intersectBoxes is relatively fast.  The complement of
    * removeIntersections.
    *
    * @param[in] keep
    */
   void
   intersectBoxes(
      const BoxTree& keep);

   /*!
    * @brief Keep the intersection of the container's boxes and keep's boxes
    *
    * Use extra data to provide needed information in a multiblock setting.
    * MultiblockBoxTree has an efficient overlap search method so this
    * version of intersectBoxes is relatively fast.  The complement of
    * removeIntersection.
    *
    * @param[in]  refinement_ratio  Assume all boxes in this BoxContainer
    * belong in this refefinement ratio.
    *
    * @param[in] keep  The boxes to intersect with this BoxContainer.
    *
    * @param[in] include_singularity_block_neighbors
    */
   void
   intersectBoxes(
      const IntVector& refinement_ratio,
      const MultiblockBoxTree& keep,
      bool include_singularity_block_neighbors = false);

   //@}

   //@{ @name Ordered insert methods

   /*!
    * The insert methods are used to add Boxes to ordered containers.  They
    * may be called on an unordered container only if the size of the unordered
    * container is zero.  If called on such an empty unordered container,
    * the state of the container will be changed to ordered.  A run-time error
    * will occur if called on a non-empty unordered container.
    */

   /*!
    * @brief  Insert a single Box.
    *
    * The Box will be added to the container unless the container already
    * contains a Box with the same BoxId.  If a Box with the same BoxId
    * does already exist in the contianer, the container will not be changed.
    *
    * @return  True if the container did not already have a Box with the
    *          same BoxId, false otherwise.
    *
    * @param[in]  Box to attempt to insert into the container.
    */ 
   bool insert(const Box& box);

   /*!
    * @brief  Insert a single Box.
    *
    * The Box will be added to the container unless the container already
    * contains a Box with the same BoxId.  If a Box with the same BoxId
    * does already exist in the contianer, the container will not be changed.
    *
    * This version of insert includes an Iterator argument pointing somewhere
    * in this container.  This Iterator indicates a position in the ordered
    * container where the search for the proper place to insert the given
    * Box will begin.
    *
    * The Iterator argument does not determine the place the Box will end up
    * in the ordered container, as that is always determined by BoxId; it
    * is intended only to provide a means of optimization when the calling
    * code knows something about the ordering of the container.
    *
    * @return  Iterator pointing to the newly-added Box if the container
    *          did not already have a Box with the same BoxId.  If the
    *          container did have a Box with the same BoxId, the returned
    *          Iterator points to that Box.
    *
    * @param[in] position  Location to begin searching for place to insert Box
    * @param[in] box       Box to attempt to insert into the container
    */
   Iterator insert ( Iterator position,
                     const Box& box );

   /*!
    * @brief  Insert all Boxes within a range.
    *
    * Boxes in the range [first, last) are added to the ordered container, as
    * long as they do not have a BoxId matching that of a Box already in the
    * container.
    *
    * @param[in] first
    * @param[in] last
    */
   void insert ( ConstIterator first,
                 ConstIterator last );

   //@} 

   //@{ @name Methods that may only be called on an ordered container

   /*!
    * @brief  Find a box in an ordered container.
    *
    * Search for a Box having the same BoxId as the given box argument.  This
    * may only be called on an ordered container.
    *
    * @return  If a Box with the same BoxId as the argument is found,
    *          the Iterator points to that Box in this container, otherwise
    *          end() for this container is returned.
    *
    * @param[in]  box  Box serving as key for the find operation.  Only
    *                  its BoxId is compared to members of this container. 
    */  
   Iterator find(const Box& box) const;

   /*!
    * @brief  Get lower bound Iterator for a given Box.
    *
    * This may only be called on an ordered container.
    *
    * @return  Iterator pointing to the first member of this container
    *          with a BoxId value greater than or equal to the BoxId of
    *          the argument Box.
    *
    * @param[in]  box  Box serving as key for the lower bound search.
    */
   Iterator lowerBound(const Box& box) const;

   /*!
    * @brief  Get upper bound Iterator for a given Box.
    *
    * This may only be called on an ordered container.
    *
    * @return  Iterator pointing to the first member of this container
    *          with a BoxId value greater than the BoxId of the argument
    *          Box.  Will return end() if there are no members with a greater
    *          BoxId value.
    *
    * @param[in]  box  Box serving as key for the upper bound search.
    */
   Iterator upperBound(const Box& box) const;

   /*!
    * @brief  Erase a Box from the container.
    *
    * This may only be called on an ordered container.  If a member of the
    * container has the same BoxId as the argument Box, it will be erased
    * from the container.  If no such member is found, the container is
    * unchanged.
    *
    * @return  1 if a Box is erased, 0 otherwise.
    *
    * @param[in]  box  Box serving as key to find a Box to be erased.
    */
   int erase(const Box& box);

   // The following may only be called on ordered containers.

   /*!
    * @brief Copy the members of this BoxContainer into two vector<Box>
    * objects, one containing real Boxes and one containing their
    * periodic images.
    *
    * Put the results in the output vectors.  For flexibility and
    * efficiency, the output containers are NOT cleared first, so users
    * may want to clear them before calling this method.
    *
    * @param[out] real_mapped_box_vector
    * @param[out] periodic_image_mapped_box_vector
    */
   void
   separatePeriodicImages(
      std::vector<Box>& real_mapped_box_vector,
      std::vector<Box>& periodic_image_mapped_box_vector) const;

   /*!
    * @brief  Any members of this container that are periodic images will
    *         be erased.
    */
   void
   removePeriodicImageBoxes();

   /*!
    * @brief  Place unshifted versions of Boxes into a BoxContainer.
    *
    * For all members of this container that are periodic images, create
    * an unshifted copy the member box and add insert it the output container.
    * Additionally, insert all members of the container that are not
    * periodic images to the output container.
    *
    * For flexibility and efficiency, the output container is NOT cleared
    * first, so users may want to clear it before calling this method.
    *
    * @param[out] output_mapped_boxes
    *
    * @param[in] refinement_ratio Refinement ratio where the boxes live.
    */
   void
   unshiftPeriodicImageBoxes(
      BoxContainer& output_mapped_boxes,
      const IntVector& refinement_ratio) const;

   //@}

   /*!
    * @brief  Equality operator
    *
    * @return  If ordered, return true if all BoxIds in this and rhs are
    *          identical.  If unordered, return true if a Boxes in this
    *          and rhs are spatially equal.
    *
    * @param[in] rhs
    */
   bool
   operator == (
      const BoxContainer& rhs) const;

   /*!
    * @brief  Inequality operator
    *
    * @return  Return true if operator== would return false.
    *
    * @param[in] rhs
    */
   bool
   operator != (
      const BoxContainer& rhs) const;

   //@{ @name I/O

   /*!
    * @brief Write the BoxContainer to a database.
    */
   void
   putToDatabase(
      tbox::Database& database) const;

   /*!
    * @brief Read the BoxContainer from a database.
    */
   void
   getFromDatabase(
      tbox::Database& database);

   /*!
    * @brief Conversion from BoxContainer to tbox::Array<tbox::DatabaseBox>.
    */
   operator tbox::Array<tbox::DatabaseBox>() const;

   /*!
    * @brief Print each box in the container to the specified output stream.
    *
    * @param[in] os
    */
   void
   print(
      std::ostream& os = tbox::plog) const;

   /*!
    * @brief Intermediary between BoxContainer and output streams,
    * adding ability to control the output.  See
    * BoxContainer::format().
    */
   class Outputter
   {

      friend std::ostream&
      operator << (
         std::ostream& s,
         const Outputter& f);

private:
      friend class BoxContainer;

      /*!
       * @brief Construct the Outputter with a BoxContainer and the
       * parameters needed to output the BoxContainer to a stream.
       */
      Outputter(
         const BoxContainer& mapped_box_set,
         const std::string& border,
         int detail_depth = 0);

      void
      operator = (
         const Outputter& rhs);               // Unimplemented private.

      const BoxContainer& d_set;

      const std::string d_border;

      const int d_detail_depth;
   };

   /*!
    * @brief Return a object to that can format the BoxContainer for
    * inserting into output streams.
    *
    * Usage example (printing with a tab indentation):
    * @verbatim
    *    cout << "my mapped_boxes:\n" << mapped_boxes.format("\t") << endl;
    * @endverbatim
    *
    * @param[in] border Left border of the output
    *
    * @param[in] detail_depth How much detail to print.
    */
   Outputter
   format(
      const std::string& border = std::string(),
      int detail_depth = 0) const;

   //@}

private:

   /*
    * Static integer constant describing class's version number.
    */
   static const int HIER_BOX_CONTAINER_VERSION;

   /*!
    * @brief Break up bursty against solid and adds the pieces to container.
    *
    * The bursting is done on dimensions 0 through dimension-1, starting
    * with lowest dimensions first to try to maintain the canonical
    * representation for the bursted domains.
    *
    * @param[in] bursty
    * @param[in] solid
    * @param[in] dimension
    */
   void
   burstBoxes(
      const Box& bursty,
      const Box& solid,
      const int dimension);

   /*!
    * @brief Break up bursty against solid and adds the pieces to container
    * starting at location pointed to by itr.
    *
    * The bursting is done on dimensions 0 through dimension-1, starting
    * with lowest dimensions first to try to maintain the canonical
    * representation for the bursted domains.
    *
    * @param[in] bursty
    * @param[in] solid
    * @param[in] dimension
    * @param[in] itr
    */
   void
   burstBoxes(
      const Box& bursty,
      const Box& solid,
      const int dimension,
      Iterator& itr);

   /*!
    * @brief Remove from each box in the sublist of this container defined
    * by sublist_start and sublist_end portions intersecting takeaway.
    *
    * @param[in] takeaway
    * @param[in] sublist_start
    * @param[in] sublist_end
    * @param[in] insertion_pt Where to put new boxes created by this
    * operation.
    */
   void
   removeIntersectionsFromSublist(
      const Box& takeaway,
      Iterator& sublist_start,
      Iterator& sublist_end,
      Iterator& insertion_pt);

   /*!
    * List that provides the internal storage for the member Boxes.
    */
   std::list<Box> d_list;

   /*!
    * Set of Box* used for ordered containers.  Each Box* in the set
    * points to a member of d_list.
    */
   std::set<Box*, Box::id_less> d_set;

   bool d_ordered;
};

}
}

#ifdef SAMRAI_INLINE
#include "SAMRAI/hier/BoxContainer.I"
#endif

#endif