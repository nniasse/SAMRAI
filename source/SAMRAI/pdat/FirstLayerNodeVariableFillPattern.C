/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright
 * information, see COPYRIGHT and COPYING.LESSER.
 *
 * Copyright:     (c) 1997-2011 Lawrence Livermore National Security, LLC
 * Description:   Fill pattern class to provide interface for stencils
 *
 ************************************************************************/

#ifndef included_pdat_FirstLayerNodeVariableFillPattern_C
#define included_pdat_FirstLayerNodeVariableFillPattern_C

#include "SAMRAI/pdat/FirstLayerNodeVariableFillPattern.h"

#include "SAMRAI/hier/BoxContainerIterator.h"
#include "SAMRAI/pdat/NodeGeometry.h"
#include "SAMRAI/tbox/Utilities.h"

namespace SAMRAI {
namespace pdat {

const std::string FirstLayerNodeVariableFillPattern::s_name_id =
   "FIRST_LAYER_NODE_FILL_PATTERN";

/*
 *************************************************************************
 *
 * Constructor
 *
 *************************************************************************
 */

FirstLayerNodeVariableFillPattern::FirstLayerNodeVariableFillPattern(
   const tbox::Dimension& dim):
   d_dim(dim)
{
}

/*
 *************************************************************************
 *
 * Destructor
 *
 *************************************************************************
 */

FirstLayerNodeVariableFillPattern::~FirstLayerNodeVariableFillPattern()
{
}

/*
 *************************************************************************
 *
 * Calculate the overlap according to the desired pattern
 *
 *************************************************************************
 */

tbox::Pointer<hier::BoxOverlap>
FirstLayerNodeVariableFillPattern::calculateOverlap(
   const hier::BoxGeometry& dst_geometry,
   const hier::BoxGeometry& src_geometry,
   const hier::Box& dst_patch_box,
   const hier::Box& src_mask,
   const hier::Box& fill_box,
   const bool overwrite_interior,
   const hier::Transformation& transformation) const
{
   TBOX_DIM_ASSERT_CHECK_ARGS2(dst_patch_box, src_mask);

   hier::BoxContainer stencil_boxes;
   computeStencilBoxes(stencil_boxes, dst_patch_box);

   hier::BoxContainer dst_boxes;

   const NodeGeometry* t_dst =
      dynamic_cast<const NodeGeometry *>(&dst_geometry);
   const NodeGeometry* t_src =
      dynamic_cast<const NodeGeometry *>(&src_geometry);

   TBOX_ASSERT(t_dst);
   TBOX_ASSERT(t_src);

   t_dst->computeDestinationBoxes(dst_boxes, *t_src, src_mask, fill_box,
      overwrite_interior, transformation);

   dst_boxes.intersectBoxes(stencil_boxes);

   hier::BoxOverlap* overlap = new NodeOverlap(dst_boxes, transformation);
   return tbox::Pointer<hier::BoxOverlap>(overlap);

}

/*
 *************************************************************************
 *
 * Return the stencil width (0)
 *
 *************************************************************************
 */

const hier::IntVector& FirstLayerNodeVariableFillPattern::getStencilWidth()
{
   return hier::IntVector::getZero(d_dim);
}

/*
 *************************************************************************
 *
 * Return the string name identifier
 *
 *************************************************************************
 */

const std::string& FirstLayerNodeVariableFillPattern::getPatternName() const
{
   return s_name_id;
}

/*
 *************************************************************************
 *
 * Compute the boxes for the stencil around a given patch box
 *
 *************************************************************************
 */

void FirstLayerNodeVariableFillPattern::computeStencilBoxes(
   hier::BoxContainer& stencil_boxes,
   const hier::Box& dst_box) const
{
   TBOX_ASSERT(stencil_boxes.size() == 0);

   hier::Box dst_node_box(pdat::NodeGeometry::toNodeBox(dst_box));
   hier::Box interior_node_box(dst_node_box);
   interior_node_box.grow(hier::IntVector(dst_box.getDim(), -1));

   stencil_boxes.removeIntersections(dst_node_box, interior_node_box);
}

/*
 *************************************************************************
 *
 * Compute BoxOverlap that specifies data to be filled by refinement
 * operator.
 *
 *************************************************************************
 */

tbox::Pointer<hier::BoxOverlap>
FirstLayerNodeVariableFillPattern::computeFillBoxesOverlap(
   const hier::BoxContainer& fill_boxes,
   const hier::Box& patch_box,
   const hier::Box& data_box,
   const hier::PatchDataFactory& pdf) const
{
   NULL_USE(pdf);

   const tbox::Dimension& dim = patch_box.getDim();

   hier::BoxContainer stencil_boxes;
   computeStencilBoxes(stencil_boxes, patch_box);

   hier::BoxContainer overlap_boxes(fill_boxes);

   /*
    * This is the equivalent of converting every box in overlap_boxes
    * to a node centering, which must be done before intersecting with
    * stencil_boxes, which is node-centered.
    */
   for (hier::BoxContainer::Iterator b(overlap_boxes); b != overlap_boxes.end(); ++b) {
      b().growUpper(hier::IntVector::getOne(dim));
   }

   overlap_boxes.intersectBoxes(pdat::NodeGeometry::toNodeBox(data_box));

   overlap_boxes.intersectBoxes(stencil_boxes);

   /*
    * We need to coalesce the boxes to prevent redundant nodes in the
    * overlap, which can produce erroneous results accumulation
    * communication.
    */

   overlap_boxes.coalesce();

   hier::BoxOverlap* overlap =
      new pdat::NodeOverlap(
         overlap_boxes,
         hier::Transformation(hier::IntVector::getZero(dim)));
   return tbox::Pointer<hier::BoxOverlap>(overlap);
}

}
}
#endif