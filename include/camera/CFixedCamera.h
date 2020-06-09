#pragma once

#include <camera/CICamera.h>

namespace engine
{
    /// Fixed-type of camera, which is placed at a fixed position and points at a fixed target point
    class CFixedCamera : public CICamera
    {
    public :

        /// Constructs a fixed-camera that points from an origin to a target point
        ///
        /// @param name         Unique identifier of the camera on the scene
        /// @param position     Position in world-space of this camera
        /// @param target_point Point in world-space that this camera points to
        /// @param up_axis      Axis used as world-up in the scene
        /// @param proj_data    Structs containing the parameters describing projection information
        CFixedCamera( const std::string& name,
                      const CVec3& position,
                      const CVec3& target_point,
                      const eAxis& up_axis,
                      const CCameraProjData& proj_data );

        /// Sets the world-space transform of this camera
        ///
        /// @param transform    New world-space transform of this camera
        void SetCameraTransform( const CMat4& transform );

        /// Returns the type-enum of all cameras from this class
        static eCameraType GetStaticType() { return eCameraType::FIXED; }

    protected :

        // Documentation inherited
        void _PositionChangedInternal() override;

        // Documentation inherited
        void _TargetPointChangedInternal() override;

        // Documentation inherited
        void _UpdateInternal() override {}

        // Documentation inherited
        void _ResizeInternal( int width, int height ) override {}

        // Documentation inherited
        std::string _ToStringInternal() const override;
    };
}