//
// Created by yueqian on 30/11/2021.
// Refer to this site for more information
// http://gazebosim.org/tutorials?tut=set_velocity
//

#ifndef CUSTOM_GAZEBO_PLUGINS_GAZEBO_ROS_JOINT_MOTOR_HPP
#define CUSTOM_GAZEBO_PLUGINS_GAZEBO_ROS_JOINT_MOTOR_HPP

#include <gazebo/common/Plugin.hh>

// For std::unique_ptr, could be removed
#include <memory>

namespace gazebo_plugins
{
    // Forward declaration of private data class.
    class GazeboRosTemplatePrivate;

    /// Example ROS-powered Gazebo plugin with some useful boilerplate.
    /// \details This is a `ModelPlugin`, but it could be any supported Gazebo plugin type, such as
    /// System, Visual, GUI, World, Sensor, etc.
    class GazeboRosTemplate : public gazebo::ModelPlugin
    {
    public:
        /// Constructor
        GazeboRosTemplate();

        /// Destructor
        virtual ~GazeboRosTemplate();

        /// Gazebo calls this when the plugin is loaded.
        /// \param[in] model Pointer to parent model. Other plugin types will expose different entities,
        /// such as `gazebo::sensors::SensorPtr`, `gazebo::physics::WorldPtr`,
        /// `gazebo::rendering::VisualPtr`, etc.
        /// \param[in] sdf SDF element containing user-defined parameters.
        void Load(gazebo::physics::ModelPtr model, sdf::ElementPtr sdf) override;

    protected:
        /// Optional callback to be called at every simulation iteration.
        virtual void OnUpdate();

    private:
        /// Recommended PIMPL pattern. This variable should hold all private
        /// data members.
        std::unique_ptr<GazeboRosTemplatePrivate> impl_;
    };
} // namespace gazebo_plugins

#endif //CUSTOM_GAZEBO_PLUGINS_GAZEBO_ROS_JOINT_MOTOR_HPP
