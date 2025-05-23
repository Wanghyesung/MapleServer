// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Transform.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Transform_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Transform_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3017000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3017001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Transform_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Transform_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Transform_2eproto;
namespace Protocol {
class S_TRANSFORM;
struct S_TRANSFORMDefaultTypeInternal;
extern S_TRANSFORMDefaultTypeInternal _S_TRANSFORM_default_instance_;
}  // namespace Protocol
PROTOBUF_NAMESPACE_OPEN
template<> ::Protocol::S_TRANSFORM* Arena::CreateMaybeMessage<::Protocol::S_TRANSFORM>(Arena*);
PROTOBUF_NAMESPACE_CLOSE
namespace Protocol {

// ===================================================================

class S_TRANSFORM final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:Protocol.S_TRANSFORM) */ {
 public:
  inline S_TRANSFORM() : S_TRANSFORM(nullptr) {}
  ~S_TRANSFORM() override;
  explicit constexpr S_TRANSFORM(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  S_TRANSFORM(const S_TRANSFORM& from);
  S_TRANSFORM(S_TRANSFORM&& from) noexcept
    : S_TRANSFORM() {
    *this = ::std::move(from);
  }

  inline S_TRANSFORM& operator=(const S_TRANSFORM& from) {
    CopyFrom(from);
    return *this;
  }
  inline S_TRANSFORM& operator=(S_TRANSFORM&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const S_TRANSFORM& default_instance() {
    return *internal_default_instance();
  }
  static inline const S_TRANSFORM* internal_default_instance() {
    return reinterpret_cast<const S_TRANSFORM*>(
               &_S_TRANSFORM_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(S_TRANSFORM& a, S_TRANSFORM& b) {
    a.Swap(&b);
  }
  inline void Swap(S_TRANSFORM* other) {
    if (other == this) return;
    if (GetOwningArena() == other->GetOwningArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(S_TRANSFORM* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline S_TRANSFORM* New() const final {
    return new S_TRANSFORM();
  }

  S_TRANSFORM* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<S_TRANSFORM>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const S_TRANSFORM& from);
  void MergeFrom(const S_TRANSFORM& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(S_TRANSFORM* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "Protocol.S_TRANSFORM";
  }
  protected:
  explicit S_TRANSFORM(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kSceneFieldNumber = 1,
    kLayerIdFieldNumber = 2,
    kXFieldNumber = 3,
    kYFieldNumber = 4,
    kZFieldNumber = 5,
  };
  // string scene = 1;
  void clear_scene();
  const std::string& scene() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_scene(ArgT0&& arg0, ArgT... args);
  std::string* mutable_scene();
  PROTOBUF_FUTURE_MUST_USE_RESULT std::string* release_scene();
  void set_allocated_scene(std::string* scene);
  private:
  const std::string& _internal_scene() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_scene(const std::string& value);
  std::string* _internal_mutable_scene();
  public:

  // uint32 layer_id = 2;
  void clear_layer_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 layer_id() const;
  void set_layer_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_layer_id() const;
  void _internal_set_layer_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // float x = 3;
  void clear_x();
  float x() const;
  void set_x(float value);
  private:
  float _internal_x() const;
  void _internal_set_x(float value);
  public:

  // float y = 4;
  void clear_y();
  float y() const;
  void set_y(float value);
  private:
  float _internal_y() const;
  void _internal_set_y(float value);
  public:

  // float z = 5;
  void clear_z();
  float z() const;
  void set_z(float value);
  private:
  float _internal_z() const;
  void _internal_set_z(float value);
  public:

  // @@protoc_insertion_point(class_scope:Protocol.S_TRANSFORM)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr scene_;
  ::PROTOBUF_NAMESPACE_ID::uint32 layer_id_;
  float x_;
  float y_;
  float z_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Transform_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// S_TRANSFORM

// string scene = 1;
inline void S_TRANSFORM::clear_scene() {
  scene_.ClearToEmpty();
}
inline const std::string& S_TRANSFORM::scene() const {
  // @@protoc_insertion_point(field_get:Protocol.S_TRANSFORM.scene)
  return _internal_scene();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void S_TRANSFORM::set_scene(ArgT0&& arg0, ArgT... args) {
 
 scene_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:Protocol.S_TRANSFORM.scene)
}
inline std::string* S_TRANSFORM::mutable_scene() {
  // @@protoc_insertion_point(field_mutable:Protocol.S_TRANSFORM.scene)
  return _internal_mutable_scene();
}
inline const std::string& S_TRANSFORM::_internal_scene() const {
  return scene_.Get();
}
inline void S_TRANSFORM::_internal_set_scene(const std::string& value) {
  
  scene_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArenaForAllocation());
}
inline std::string* S_TRANSFORM::_internal_mutable_scene() {
  
  return scene_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArenaForAllocation());
}
inline std::string* S_TRANSFORM::release_scene() {
  // @@protoc_insertion_point(field_release:Protocol.S_TRANSFORM.scene)
  return scene_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArenaForAllocation());
}
inline void S_TRANSFORM::set_allocated_scene(std::string* scene) {
  if (scene != nullptr) {
    
  } else {
    
  }
  scene_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), scene,
      GetArenaForAllocation());
  // @@protoc_insertion_point(field_set_allocated:Protocol.S_TRANSFORM.scene)
}

// uint32 layer_id = 2;
inline void S_TRANSFORM::clear_layer_id() {
  layer_id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 S_TRANSFORM::_internal_layer_id() const {
  return layer_id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 S_TRANSFORM::layer_id() const {
  // @@protoc_insertion_point(field_get:Protocol.S_TRANSFORM.layer_id)
  return _internal_layer_id();
}
inline void S_TRANSFORM::_internal_set_layer_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  layer_id_ = value;
}
inline void S_TRANSFORM::set_layer_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_layer_id(value);
  // @@protoc_insertion_point(field_set:Protocol.S_TRANSFORM.layer_id)
}

// float x = 3;
inline void S_TRANSFORM::clear_x() {
  x_ = 0;
}
inline float S_TRANSFORM::_internal_x() const {
  return x_;
}
inline float S_TRANSFORM::x() const {
  // @@protoc_insertion_point(field_get:Protocol.S_TRANSFORM.x)
  return _internal_x();
}
inline void S_TRANSFORM::_internal_set_x(float value) {
  
  x_ = value;
}
inline void S_TRANSFORM::set_x(float value) {
  _internal_set_x(value);
  // @@protoc_insertion_point(field_set:Protocol.S_TRANSFORM.x)
}

// float y = 4;
inline void S_TRANSFORM::clear_y() {
  y_ = 0;
}
inline float S_TRANSFORM::_internal_y() const {
  return y_;
}
inline float S_TRANSFORM::y() const {
  // @@protoc_insertion_point(field_get:Protocol.S_TRANSFORM.y)
  return _internal_y();
}
inline void S_TRANSFORM::_internal_set_y(float value) {
  
  y_ = value;
}
inline void S_TRANSFORM::set_y(float value) {
  _internal_set_y(value);
  // @@protoc_insertion_point(field_set:Protocol.S_TRANSFORM.y)
}

// float z = 5;
inline void S_TRANSFORM::clear_z() {
  z_ = 0;
}
inline float S_TRANSFORM::_internal_z() const {
  return z_;
}
inline float S_TRANSFORM::z() const {
  // @@protoc_insertion_point(field_get:Protocol.S_TRANSFORM.z)
  return _internal_z();
}
inline void S_TRANSFORM::_internal_set_z(float value) {
  
  z_ = value;
}
inline void S_TRANSFORM::set_z(float value) {
  _internal_set_z(value);
  // @@protoc_insertion_point(field_set:Protocol.S_TRANSFORM.z)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Protocol

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Transform_2eproto
