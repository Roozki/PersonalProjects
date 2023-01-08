// Generated by gencpp from file sb_msgs/ArmPosition.msg
// DO NOT EDIT!


#ifndef SB_MSGS_MESSAGE_ARMPOSITION_H
#define SB_MSGS_MESSAGE_ARMPOSITION_H


#include <string>
#include <vector>
#include <memory>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>

#include <std_msgs/Header.h>

namespace sb_msgs
{
template <class ContainerAllocator>
struct ArmPosition_
{
  typedef ArmPosition_<ContainerAllocator> Type;

  ArmPosition_()
    : header()
    , positions()  {
    }
  ArmPosition_(const ContainerAllocator& _alloc)
    : header(_alloc)
    , positions(_alloc)  {
  (void)_alloc;
    }



   typedef  ::std_msgs::Header_<ContainerAllocator>  _header_type;
  _header_type header;

   typedef std::vector<double, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<double>> _positions_type;
  _positions_type positions;





  typedef boost::shared_ptr< ::sb_msgs::ArmPosition_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::sb_msgs::ArmPosition_<ContainerAllocator> const> ConstPtr;

}; // struct ArmPosition_

typedef ::sb_msgs::ArmPosition_<std::allocator<void> > ArmPosition;

typedef boost::shared_ptr< ::sb_msgs::ArmPosition > ArmPositionPtr;
typedef boost::shared_ptr< ::sb_msgs::ArmPosition const> ArmPositionConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::sb_msgs::ArmPosition_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::sb_msgs::ArmPosition_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::sb_msgs::ArmPosition_<ContainerAllocator1> & lhs, const ::sb_msgs::ArmPosition_<ContainerAllocator2> & rhs)
{
  return lhs.header == rhs.header &&
    lhs.positions == rhs.positions;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::sb_msgs::ArmPosition_<ContainerAllocator1> & lhs, const ::sb_msgs::ArmPosition_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace sb_msgs

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsMessage< ::sb_msgs::ArmPosition_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::sb_msgs::ArmPosition_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sb_msgs::ArmPosition_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::sb_msgs::ArmPosition_<ContainerAllocator> const>
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sb_msgs::ArmPosition_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::sb_msgs::ArmPosition_<ContainerAllocator> const>
  : TrueType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::sb_msgs::ArmPosition_<ContainerAllocator> >
{
  static const char* value()
  {
    return "df5e3630bdea268eaebb93698a507d99";
  }

  static const char* value(const ::sb_msgs::ArmPosition_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xdf5e3630bdea268eULL;
  static const uint64_t static_value2 = 0xaebb93698a507d99ULL;
};

template<class ContainerAllocator>
struct DataType< ::sb_msgs::ArmPosition_<ContainerAllocator> >
{
  static const char* value()
  {
    return "sb_msgs/ArmPosition";
  }

  static const char* value(const ::sb_msgs::ArmPosition_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::sb_msgs::ArmPosition_<ContainerAllocator> >
{
  static const char* value()
  {
    return "std_msgs/Header header\n"
"float64[] positions\n"
"\n"
"================================================================================\n"
"MSG: std_msgs/Header\n"
"# Standard metadata for higher-level stamped data types.\n"
"# This is generally used to communicate timestamped data \n"
"# in a particular coordinate frame.\n"
"# \n"
"# sequence ID: consecutively increasing ID \n"
"uint32 seq\n"
"#Two-integer timestamp that is expressed as:\n"
"# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')\n"
"# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')\n"
"# time-handling sugar is provided by the client library\n"
"time stamp\n"
"#Frame this data is associated with\n"
"string frame_id\n"
;
  }

  static const char* value(const ::sb_msgs::ArmPosition_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::sb_msgs::ArmPosition_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.header);
      stream.next(m.positions);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct ArmPosition_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::sb_msgs::ArmPosition_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::sb_msgs::ArmPosition_<ContainerAllocator>& v)
  {
    s << indent << "header: ";
    s << std::endl;
    Printer< ::std_msgs::Header_<ContainerAllocator> >::stream(s, indent + "  ", v.header);
    s << indent << "positions[]" << std::endl;
    for (size_t i = 0; i < v.positions.size(); ++i)
    {
      s << indent << "  positions[" << i << "]: ";
      Printer<double>::stream(s, indent + "  ", v.positions[i]);
    }
  }
};

} // namespace message_operations
} // namespace ros

#endif // SB_MSGS_MESSAGE_ARMPOSITION_H