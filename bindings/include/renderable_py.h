#pragma once

#include <common_py.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>

namespace py = pybind11;

namespace engine
{
    void bindings_renderableBase( py::module& m );
    void bindings_renderableMesh( py::module& m );
    void bindings_renderableModel( py::module& m );

    // helper functions to grab batches of vectors and indices
    void grabBatchOfVector3FromNpArray( const py::array_t<float32>& src_arr,
                                        std::vector<CVec3>& dst_vec );
    void grabBatchOfVector2FromNpArray( const py::array_t<float32>& src_arr,
                                        std::vector<CVec2>& dst_vec );
    void grabBatchOfInd3FromNpArray( const py::array_t<uint32>& src_arr,
                                     std::vector<CInd3>& dst_vec );
}