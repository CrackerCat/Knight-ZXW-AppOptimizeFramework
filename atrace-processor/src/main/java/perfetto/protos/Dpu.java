// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: protos/perfetto/trace/ftrace/dpu.proto

package perfetto.protos;

public final class Dpu {
  private Dpu() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }

  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistry registry) {
    registerAllExtensions(
        (com.google.protobuf.ExtensionRegistryLite) registry);
  }
  public interface DpuTracingMarkWriteFtraceEventOrBuilder extends
      // @@protoc_insertion_point(interface_extends:perfetto.protos.DpuTracingMarkWriteFtraceEvent)
      com.google.protobuf.MessageOrBuilder {

    /**
     * <code>optional int32 pid = 1;</code>
     * @return Whether the pid field is set.
     */
    boolean hasPid();
    /**
     * <code>optional int32 pid = 1;</code>
     * @return The pid.
     */
    int getPid();

    /**
     * <code>optional string trace_name = 2;</code>
     * @return Whether the traceName field is set.
     */
    boolean hasTraceName();
    /**
     * <code>optional string trace_name = 2;</code>
     * @return The traceName.
     */
    java.lang.String getTraceName();
    /**
     * <code>optional string trace_name = 2;</code>
     * @return The bytes for traceName.
     */
    com.google.protobuf.ByteString
        getTraceNameBytes();

    /**
     * <code>optional uint32 trace_begin = 3;</code>
     * @return Whether the traceBegin field is set.
     */
    boolean hasTraceBegin();
    /**
     * <code>optional uint32 trace_begin = 3;</code>
     * @return The traceBegin.
     */
    int getTraceBegin();

    /**
     * <code>optional string name = 4;</code>
     * @return Whether the name field is set.
     */
    boolean hasName();
    /**
     * <code>optional string name = 4;</code>
     * @return The name.
     */
    java.lang.String getName();
    /**
     * <code>optional string name = 4;</code>
     * @return The bytes for name.
     */
    com.google.protobuf.ByteString
        getNameBytes();

    /**
     * <code>optional uint32 type = 5;</code>
     * @return Whether the type field is set.
     */
    boolean hasType();
    /**
     * <code>optional uint32 type = 5;</code>
     * @return The type.
     */
    int getType();

    /**
     * <code>optional int32 value = 6;</code>
     * @return Whether the value field is set.
     */
    boolean hasValue();
    /**
     * <code>optional int32 value = 6;</code>
     * @return The value.
     */
    int getValue();
  }
  /**
   * Protobuf type {@code perfetto.protos.DpuTracingMarkWriteFtraceEvent}
   */
  public static final class DpuTracingMarkWriteFtraceEvent extends
      com.google.protobuf.GeneratedMessageV3 implements
      // @@protoc_insertion_point(message_implements:perfetto.protos.DpuTracingMarkWriteFtraceEvent)
      DpuTracingMarkWriteFtraceEventOrBuilder {
  private static final long serialVersionUID = 0L;
    // Use DpuTracingMarkWriteFtraceEvent.newBuilder() to construct.
    private DpuTracingMarkWriteFtraceEvent(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
      super(builder);
    }
    private DpuTracingMarkWriteFtraceEvent() {
      traceName_ = "";
      name_ = "";
    }

    @java.lang.Override
    @SuppressWarnings({"unused"})
    protected java.lang.Object newInstance(
        UnusedPrivateParameter unused) {
      return new DpuTracingMarkWriteFtraceEvent();
    }

    @java.lang.Override
    public final com.google.protobuf.UnknownFieldSet
    getUnknownFields() {
      return this.unknownFields;
    }
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return perfetto.protos.Dpu.internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return perfetto.protos.Dpu.internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.class, perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.Builder.class);
    }

    private int bitField0_;
    public static final int PID_FIELD_NUMBER = 1;
    private int pid_ = 0;
    /**
     * <code>optional int32 pid = 1;</code>
     * @return Whether the pid field is set.
     */
    @java.lang.Override
    public boolean hasPid() {
      return ((bitField0_ & 0x00000001) != 0);
    }
    /**
     * <code>optional int32 pid = 1;</code>
     * @return The pid.
     */
    @java.lang.Override
    public int getPid() {
      return pid_;
    }

    public static final int TRACE_NAME_FIELD_NUMBER = 2;
    @SuppressWarnings("serial")
    private volatile java.lang.Object traceName_ = "";
    /**
     * <code>optional string trace_name = 2;</code>
     * @return Whether the traceName field is set.
     */
    @java.lang.Override
    public boolean hasTraceName() {
      return ((bitField0_ & 0x00000002) != 0);
    }
    /**
     * <code>optional string trace_name = 2;</code>
     * @return The traceName.
     */
    @java.lang.Override
    public java.lang.String getTraceName() {
      java.lang.Object ref = traceName_;
      if (ref instanceof java.lang.String) {
        return (java.lang.String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        if (bs.isValidUtf8()) {
          traceName_ = s;
        }
        return s;
      }
    }
    /**
     * <code>optional string trace_name = 2;</code>
     * @return The bytes for traceName.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString
        getTraceNameBytes() {
      java.lang.Object ref = traceName_;
      if (ref instanceof java.lang.String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        traceName_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }

    public static final int TRACE_BEGIN_FIELD_NUMBER = 3;
    private int traceBegin_ = 0;
    /**
     * <code>optional uint32 trace_begin = 3;</code>
     * @return Whether the traceBegin field is set.
     */
    @java.lang.Override
    public boolean hasTraceBegin() {
      return ((bitField0_ & 0x00000004) != 0);
    }
    /**
     * <code>optional uint32 trace_begin = 3;</code>
     * @return The traceBegin.
     */
    @java.lang.Override
    public int getTraceBegin() {
      return traceBegin_;
    }

    public static final int NAME_FIELD_NUMBER = 4;
    @SuppressWarnings("serial")
    private volatile java.lang.Object name_ = "";
    /**
     * <code>optional string name = 4;</code>
     * @return Whether the name field is set.
     */
    @java.lang.Override
    public boolean hasName() {
      return ((bitField0_ & 0x00000008) != 0);
    }
    /**
     * <code>optional string name = 4;</code>
     * @return The name.
     */
    @java.lang.Override
    public java.lang.String getName() {
      java.lang.Object ref = name_;
      if (ref instanceof java.lang.String) {
        return (java.lang.String) ref;
      } else {
        com.google.protobuf.ByteString bs = 
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        if (bs.isValidUtf8()) {
          name_ = s;
        }
        return s;
      }
    }
    /**
     * <code>optional string name = 4;</code>
     * @return The bytes for name.
     */
    @java.lang.Override
    public com.google.protobuf.ByteString
        getNameBytes() {
      java.lang.Object ref = name_;
      if (ref instanceof java.lang.String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        name_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }

    public static final int TYPE_FIELD_NUMBER = 5;
    private int type_ = 0;
    /**
     * <code>optional uint32 type = 5;</code>
     * @return Whether the type field is set.
     */
    @java.lang.Override
    public boolean hasType() {
      return ((bitField0_ & 0x00000010) != 0);
    }
    /**
     * <code>optional uint32 type = 5;</code>
     * @return The type.
     */
    @java.lang.Override
    public int getType() {
      return type_;
    }

    public static final int VALUE_FIELD_NUMBER = 6;
    private int value_ = 0;
    /**
     * <code>optional int32 value = 6;</code>
     * @return Whether the value field is set.
     */
    @java.lang.Override
    public boolean hasValue() {
      return ((bitField0_ & 0x00000020) != 0);
    }
    /**
     * <code>optional int32 value = 6;</code>
     * @return The value.
     */
    @java.lang.Override
    public int getValue() {
      return value_;
    }

    private byte memoizedIsInitialized = -1;
    @java.lang.Override
    public final boolean isInitialized() {
      byte isInitialized = memoizedIsInitialized;
      if (isInitialized == 1) return true;
      if (isInitialized == 0) return false;

      memoizedIsInitialized = 1;
      return true;
    }

    @java.lang.Override
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      if (((bitField0_ & 0x00000001) != 0)) {
        output.writeInt32(1, pid_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        com.google.protobuf.GeneratedMessageV3.writeString(output, 2, traceName_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        output.writeUInt32(3, traceBegin_);
      }
      if (((bitField0_ & 0x00000008) != 0)) {
        com.google.protobuf.GeneratedMessageV3.writeString(output, 4, name_);
      }
      if (((bitField0_ & 0x00000010) != 0)) {
        output.writeUInt32(5, type_);
      }
      if (((bitField0_ & 0x00000020) != 0)) {
        output.writeInt32(6, value_);
      }
      getUnknownFields().writeTo(output);
    }

    @java.lang.Override
    public int getSerializedSize() {
      int size = memoizedSize;
      if (size != -1) return size;

      size = 0;
      if (((bitField0_ & 0x00000001) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(1, pid_);
      }
      if (((bitField0_ & 0x00000002) != 0)) {
        size += com.google.protobuf.GeneratedMessageV3.computeStringSize(2, traceName_);
      }
      if (((bitField0_ & 0x00000004) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeUInt32Size(3, traceBegin_);
      }
      if (((bitField0_ & 0x00000008) != 0)) {
        size += com.google.protobuf.GeneratedMessageV3.computeStringSize(4, name_);
      }
      if (((bitField0_ & 0x00000010) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeUInt32Size(5, type_);
      }
      if (((bitField0_ & 0x00000020) != 0)) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(6, value_);
      }
      size += getUnknownFields().getSerializedSize();
      memoizedSize = size;
      return size;
    }

    @java.lang.Override
    public boolean equals(final java.lang.Object obj) {
      if (obj == this) {
       return true;
      }
      if (!(obj instanceof perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent)) {
        return super.equals(obj);
      }
      perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent other = (perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent) obj;

      if (hasPid() != other.hasPid()) return false;
      if (hasPid()) {
        if (getPid()
            != other.getPid()) return false;
      }
      if (hasTraceName() != other.hasTraceName()) return false;
      if (hasTraceName()) {
        if (!getTraceName()
            .equals(other.getTraceName())) return false;
      }
      if (hasTraceBegin() != other.hasTraceBegin()) return false;
      if (hasTraceBegin()) {
        if (getTraceBegin()
            != other.getTraceBegin()) return false;
      }
      if (hasName() != other.hasName()) return false;
      if (hasName()) {
        if (!getName()
            .equals(other.getName())) return false;
      }
      if (hasType() != other.hasType()) return false;
      if (hasType()) {
        if (getType()
            != other.getType()) return false;
      }
      if (hasValue() != other.hasValue()) return false;
      if (hasValue()) {
        if (getValue()
            != other.getValue()) return false;
      }
      if (!getUnknownFields().equals(other.getUnknownFields())) return false;
      return true;
    }

    @java.lang.Override
    public int hashCode() {
      if (memoizedHashCode != 0) {
        return memoizedHashCode;
      }
      int hash = 41;
      hash = (19 * hash) + getDescriptor().hashCode();
      if (hasPid()) {
        hash = (37 * hash) + PID_FIELD_NUMBER;
        hash = (53 * hash) + getPid();
      }
      if (hasTraceName()) {
        hash = (37 * hash) + TRACE_NAME_FIELD_NUMBER;
        hash = (53 * hash) + getTraceName().hashCode();
      }
      if (hasTraceBegin()) {
        hash = (37 * hash) + TRACE_BEGIN_FIELD_NUMBER;
        hash = (53 * hash) + getTraceBegin();
      }
      if (hasName()) {
        hash = (37 * hash) + NAME_FIELD_NUMBER;
        hash = (53 * hash) + getName().hashCode();
      }
      if (hasType()) {
        hash = (37 * hash) + TYPE_FIELD_NUMBER;
        hash = (53 * hash) + getType();
      }
      if (hasValue()) {
        hash = (37 * hash) + VALUE_FIELD_NUMBER;
        hash = (53 * hash) + getValue();
      }
      hash = (29 * hash) + getUnknownFields().hashCode();
      memoizedHashCode = hash;
      return hash;
    }

    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        java.nio.ByteBuffer data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        java.nio.ByteBuffer data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return PARSER.parseFrom(data, extensionRegistry);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input, extensionRegistry);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseDelimitedWithIOException(PARSER, input);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input);
    }
    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return com.google.protobuf.GeneratedMessageV3
          .parseWithIOException(PARSER, input, extensionRegistry);
    }

    @java.lang.Override
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder() {
      return DEFAULT_INSTANCE.toBuilder();
    }
    public static Builder newBuilder(perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent prototype) {
      return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
    }
    @java.lang.Override
    public Builder toBuilder() {
      return this == DEFAULT_INSTANCE
          ? new Builder() : new Builder().mergeFrom(this);
    }

    @java.lang.Override
    protected Builder newBuilderForType(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      Builder builder = new Builder(parent);
      return builder;
    }
    /**
     * Protobuf type {@code perfetto.protos.DpuTracingMarkWriteFtraceEvent}
     */
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
        // @@protoc_insertion_point(builder_implements:perfetto.protos.DpuTracingMarkWriteFtraceEvent)
        perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEventOrBuilder {
      public static final com.google.protobuf.Descriptors.Descriptor
          getDescriptor() {
        return perfetto.protos.Dpu.internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor;
      }

      @java.lang.Override
      protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
          internalGetFieldAccessorTable() {
        return perfetto.protos.Dpu.internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_fieldAccessorTable
            .ensureFieldAccessorsInitialized(
                perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.class, perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.Builder.class);
      }

      // Construct using perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.newBuilder()
      private Builder() {

      }

      private Builder(
          com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
        super(parent);

      }
      @java.lang.Override
      public Builder clear() {
        super.clear();
        bitField0_ = 0;
        pid_ = 0;
        traceName_ = "";
        traceBegin_ = 0;
        name_ = "";
        type_ = 0;
        value_ = 0;
        return this;
      }

      @java.lang.Override
      public com.google.protobuf.Descriptors.Descriptor
          getDescriptorForType() {
        return perfetto.protos.Dpu.internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor;
      }

      @java.lang.Override
      public perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent getDefaultInstanceForType() {
        return perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.getDefaultInstance();
      }

      @java.lang.Override
      public perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent build() {
        perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent result = buildPartial();
        if (!result.isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return result;
      }

      @java.lang.Override
      public perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent buildPartial() {
        perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent result = new perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent(this);
        if (bitField0_ != 0) { buildPartial0(result); }
        onBuilt();
        return result;
      }

      private void buildPartial0(perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent result) {
        int from_bitField0_ = bitField0_;
        int to_bitField0_ = 0;
        if (((from_bitField0_ & 0x00000001) != 0)) {
          result.pid_ = pid_;
          to_bitField0_ |= 0x00000001;
        }
        if (((from_bitField0_ & 0x00000002) != 0)) {
          result.traceName_ = traceName_;
          to_bitField0_ |= 0x00000002;
        }
        if (((from_bitField0_ & 0x00000004) != 0)) {
          result.traceBegin_ = traceBegin_;
          to_bitField0_ |= 0x00000004;
        }
        if (((from_bitField0_ & 0x00000008) != 0)) {
          result.name_ = name_;
          to_bitField0_ |= 0x00000008;
        }
        if (((from_bitField0_ & 0x00000010) != 0)) {
          result.type_ = type_;
          to_bitField0_ |= 0x00000010;
        }
        if (((from_bitField0_ & 0x00000020) != 0)) {
          result.value_ = value_;
          to_bitField0_ |= 0x00000020;
        }
        result.bitField0_ |= to_bitField0_;
      }

      @java.lang.Override
      public Builder clone() {
        return super.clone();
      }
      @java.lang.Override
      public Builder setField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          java.lang.Object value) {
        return super.setField(field, value);
      }
      @java.lang.Override
      public Builder clearField(
          com.google.protobuf.Descriptors.FieldDescriptor field) {
        return super.clearField(field);
      }
      @java.lang.Override
      public Builder clearOneof(
          com.google.protobuf.Descriptors.OneofDescriptor oneof) {
        return super.clearOneof(oneof);
      }
      @java.lang.Override
      public Builder setRepeatedField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          int index, java.lang.Object value) {
        return super.setRepeatedField(field, index, value);
      }
      @java.lang.Override
      public Builder addRepeatedField(
          com.google.protobuf.Descriptors.FieldDescriptor field,
          java.lang.Object value) {
        return super.addRepeatedField(field, value);
      }
      @java.lang.Override
      public Builder mergeFrom(com.google.protobuf.Message other) {
        if (other instanceof perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent) {
          return mergeFrom((perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent)other);
        } else {
          super.mergeFrom(other);
          return this;
        }
      }

      public Builder mergeFrom(perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent other) {
        if (other == perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent.getDefaultInstance()) return this;
        if (other.hasPid()) {
          setPid(other.getPid());
        }
        if (other.hasTraceName()) {
          traceName_ = other.traceName_;
          bitField0_ |= 0x00000002;
          onChanged();
        }
        if (other.hasTraceBegin()) {
          setTraceBegin(other.getTraceBegin());
        }
        if (other.hasName()) {
          name_ = other.name_;
          bitField0_ |= 0x00000008;
          onChanged();
        }
        if (other.hasType()) {
          setType(other.getType());
        }
        if (other.hasValue()) {
          setValue(other.getValue());
        }
        this.mergeUnknownFields(other.getUnknownFields());
        onChanged();
        return this;
      }

      @java.lang.Override
      public final boolean isInitialized() {
        return true;
      }

      @java.lang.Override
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        if (extensionRegistry == null) {
          throw new java.lang.NullPointerException();
        }
        try {
          boolean done = false;
          while (!done) {
            int tag = input.readTag();
            switch (tag) {
              case 0:
                done = true;
                break;
              case 8: {
                pid_ = input.readInt32();
                bitField0_ |= 0x00000001;
                break;
              } // case 8
              case 18: {
                traceName_ = input.readBytes();
                bitField0_ |= 0x00000002;
                break;
              } // case 18
              case 24: {
                traceBegin_ = input.readUInt32();
                bitField0_ |= 0x00000004;
                break;
              } // case 24
              case 34: {
                name_ = input.readBytes();
                bitField0_ |= 0x00000008;
                break;
              } // case 34
              case 40: {
                type_ = input.readUInt32();
                bitField0_ |= 0x00000010;
                break;
              } // case 40
              case 48: {
                value_ = input.readInt32();
                bitField0_ |= 0x00000020;
                break;
              } // case 48
              default: {
                if (!super.parseUnknownField(input, extensionRegistry, tag)) {
                  done = true; // was an endgroup tag
                }
                break;
              } // default:
            } // switch (tag)
          } // while (!done)
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          throw e.unwrapIOException();
        } finally {
          onChanged();
        } // finally
        return this;
      }
      private int bitField0_;

      private int pid_ ;
      /**
       * <code>optional int32 pid = 1;</code>
       * @return Whether the pid field is set.
       */
      @java.lang.Override
      public boolean hasPid() {
        return ((bitField0_ & 0x00000001) != 0);
      }
      /**
       * <code>optional int32 pid = 1;</code>
       * @return The pid.
       */
      @java.lang.Override
      public int getPid() {
        return pid_;
      }
      /**
       * <code>optional int32 pid = 1;</code>
       * @param value The pid to set.
       * @return This builder for chaining.
       */
      public Builder setPid(int value) {
        
        pid_ = value;
        bitField0_ |= 0x00000001;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 pid = 1;</code>
       * @return This builder for chaining.
       */
      public Builder clearPid() {
        bitField0_ = (bitField0_ & ~0x00000001);
        pid_ = 0;
        onChanged();
        return this;
      }

      private java.lang.Object traceName_ = "";
      /**
       * <code>optional string trace_name = 2;</code>
       * @return Whether the traceName field is set.
       */
      public boolean hasTraceName() {
        return ((bitField0_ & 0x00000002) != 0);
      }
      /**
       * <code>optional string trace_name = 2;</code>
       * @return The traceName.
       */
      public java.lang.String getTraceName() {
        java.lang.Object ref = traceName_;
        if (!(ref instanceof java.lang.String)) {
          com.google.protobuf.ByteString bs =
              (com.google.protobuf.ByteString) ref;
          java.lang.String s = bs.toStringUtf8();
          if (bs.isValidUtf8()) {
            traceName_ = s;
          }
          return s;
        } else {
          return (java.lang.String) ref;
        }
      }
      /**
       * <code>optional string trace_name = 2;</code>
       * @return The bytes for traceName.
       */
      public com.google.protobuf.ByteString
          getTraceNameBytes() {
        java.lang.Object ref = traceName_;
        if (ref instanceof String) {
          com.google.protobuf.ByteString b = 
              com.google.protobuf.ByteString.copyFromUtf8(
                  (java.lang.String) ref);
          traceName_ = b;
          return b;
        } else {
          return (com.google.protobuf.ByteString) ref;
        }
      }
      /**
       * <code>optional string trace_name = 2;</code>
       * @param value The traceName to set.
       * @return This builder for chaining.
       */
      public Builder setTraceName(
          java.lang.String value) {
        if (value == null) { throw new NullPointerException(); }
        traceName_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }
      /**
       * <code>optional string trace_name = 2;</code>
       * @return This builder for chaining.
       */
      public Builder clearTraceName() {
        traceName_ = getDefaultInstance().getTraceName();
        bitField0_ = (bitField0_ & ~0x00000002);
        onChanged();
        return this;
      }
      /**
       * <code>optional string trace_name = 2;</code>
       * @param value The bytes for traceName to set.
       * @return This builder for chaining.
       */
      public Builder setTraceNameBytes(
          com.google.protobuf.ByteString value) {
        if (value == null) { throw new NullPointerException(); }
        traceName_ = value;
        bitField0_ |= 0x00000002;
        onChanged();
        return this;
      }

      private int traceBegin_ ;
      /**
       * <code>optional uint32 trace_begin = 3;</code>
       * @return Whether the traceBegin field is set.
       */
      @java.lang.Override
      public boolean hasTraceBegin() {
        return ((bitField0_ & 0x00000004) != 0);
      }
      /**
       * <code>optional uint32 trace_begin = 3;</code>
       * @return The traceBegin.
       */
      @java.lang.Override
      public int getTraceBegin() {
        return traceBegin_;
      }
      /**
       * <code>optional uint32 trace_begin = 3;</code>
       * @param value The traceBegin to set.
       * @return This builder for chaining.
       */
      public Builder setTraceBegin(int value) {
        
        traceBegin_ = value;
        bitField0_ |= 0x00000004;
        onChanged();
        return this;
      }
      /**
       * <code>optional uint32 trace_begin = 3;</code>
       * @return This builder for chaining.
       */
      public Builder clearTraceBegin() {
        bitField0_ = (bitField0_ & ~0x00000004);
        traceBegin_ = 0;
        onChanged();
        return this;
      }

      private java.lang.Object name_ = "";
      /**
       * <code>optional string name = 4;</code>
       * @return Whether the name field is set.
       */
      public boolean hasName() {
        return ((bitField0_ & 0x00000008) != 0);
      }
      /**
       * <code>optional string name = 4;</code>
       * @return The name.
       */
      public java.lang.String getName() {
        java.lang.Object ref = name_;
        if (!(ref instanceof java.lang.String)) {
          com.google.protobuf.ByteString bs =
              (com.google.protobuf.ByteString) ref;
          java.lang.String s = bs.toStringUtf8();
          if (bs.isValidUtf8()) {
            name_ = s;
          }
          return s;
        } else {
          return (java.lang.String) ref;
        }
      }
      /**
       * <code>optional string name = 4;</code>
       * @return The bytes for name.
       */
      public com.google.protobuf.ByteString
          getNameBytes() {
        java.lang.Object ref = name_;
        if (ref instanceof String) {
          com.google.protobuf.ByteString b = 
              com.google.protobuf.ByteString.copyFromUtf8(
                  (java.lang.String) ref);
          name_ = b;
          return b;
        } else {
          return (com.google.protobuf.ByteString) ref;
        }
      }
      /**
       * <code>optional string name = 4;</code>
       * @param value The name to set.
       * @return This builder for chaining.
       */
      public Builder setName(
          java.lang.String value) {
        if (value == null) { throw new NullPointerException(); }
        name_ = value;
        bitField0_ |= 0x00000008;
        onChanged();
        return this;
      }
      /**
       * <code>optional string name = 4;</code>
       * @return This builder for chaining.
       */
      public Builder clearName() {
        name_ = getDefaultInstance().getName();
        bitField0_ = (bitField0_ & ~0x00000008);
        onChanged();
        return this;
      }
      /**
       * <code>optional string name = 4;</code>
       * @param value The bytes for name to set.
       * @return This builder for chaining.
       */
      public Builder setNameBytes(
          com.google.protobuf.ByteString value) {
        if (value == null) { throw new NullPointerException(); }
        name_ = value;
        bitField0_ |= 0x00000008;
        onChanged();
        return this;
      }

      private int type_ ;
      /**
       * <code>optional uint32 type = 5;</code>
       * @return Whether the type field is set.
       */
      @java.lang.Override
      public boolean hasType() {
        return ((bitField0_ & 0x00000010) != 0);
      }
      /**
       * <code>optional uint32 type = 5;</code>
       * @return The type.
       */
      @java.lang.Override
      public int getType() {
        return type_;
      }
      /**
       * <code>optional uint32 type = 5;</code>
       * @param value The type to set.
       * @return This builder for chaining.
       */
      public Builder setType(int value) {
        
        type_ = value;
        bitField0_ |= 0x00000010;
        onChanged();
        return this;
      }
      /**
       * <code>optional uint32 type = 5;</code>
       * @return This builder for chaining.
       */
      public Builder clearType() {
        bitField0_ = (bitField0_ & ~0x00000010);
        type_ = 0;
        onChanged();
        return this;
      }

      private int value_ ;
      /**
       * <code>optional int32 value = 6;</code>
       * @return Whether the value field is set.
       */
      @java.lang.Override
      public boolean hasValue() {
        return ((bitField0_ & 0x00000020) != 0);
      }
      /**
       * <code>optional int32 value = 6;</code>
       * @return The value.
       */
      @java.lang.Override
      public int getValue() {
        return value_;
      }
      /**
       * <code>optional int32 value = 6;</code>
       * @param value The value to set.
       * @return This builder for chaining.
       */
      public Builder setValue(int value) {
        
        value_ = value;
        bitField0_ |= 0x00000020;
        onChanged();
        return this;
      }
      /**
       * <code>optional int32 value = 6;</code>
       * @return This builder for chaining.
       */
      public Builder clearValue() {
        bitField0_ = (bitField0_ & ~0x00000020);
        value_ = 0;
        onChanged();
        return this;
      }
      @java.lang.Override
      public final Builder setUnknownFields(
          final com.google.protobuf.UnknownFieldSet unknownFields) {
        return super.setUnknownFields(unknownFields);
      }

      @java.lang.Override
      public final Builder mergeUnknownFields(
          final com.google.protobuf.UnknownFieldSet unknownFields) {
        return super.mergeUnknownFields(unknownFields);
      }


      // @@protoc_insertion_point(builder_scope:perfetto.protos.DpuTracingMarkWriteFtraceEvent)
    }

    // @@protoc_insertion_point(class_scope:perfetto.protos.DpuTracingMarkWriteFtraceEvent)
    private static final perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent DEFAULT_INSTANCE;
    static {
      DEFAULT_INSTANCE = new perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent();
    }

    public static perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent getDefaultInstance() {
      return DEFAULT_INSTANCE;
    }

    @java.lang.Deprecated public static final com.google.protobuf.Parser<DpuTracingMarkWriteFtraceEvent>
        PARSER = new com.google.protobuf.AbstractParser<DpuTracingMarkWriteFtraceEvent>() {
      @java.lang.Override
      public DpuTracingMarkWriteFtraceEvent parsePartialFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws com.google.protobuf.InvalidProtocolBufferException {
        Builder builder = newBuilder();
        try {
          builder.mergeFrom(input, extensionRegistry);
        } catch (com.google.protobuf.InvalidProtocolBufferException e) {
          throw e.setUnfinishedMessage(builder.buildPartial());
        } catch (com.google.protobuf.UninitializedMessageException e) {
          throw e.asInvalidProtocolBufferException().setUnfinishedMessage(builder.buildPartial());
        } catch (java.io.IOException e) {
          throw new com.google.protobuf.InvalidProtocolBufferException(e)
              .setUnfinishedMessage(builder.buildPartial());
        }
        return builder.buildPartial();
      }
    };

    public static com.google.protobuf.Parser<DpuTracingMarkWriteFtraceEvent> parser() {
      return PARSER;
    }

    @java.lang.Override
    public com.google.protobuf.Parser<DpuTracingMarkWriteFtraceEvent> getParserForType() {
      return PARSER;
    }

    @java.lang.Override
    public perfetto.protos.Dpu.DpuTracingMarkWriteFtraceEvent getDefaultInstanceForType() {
      return DEFAULT_INSTANCE;
    }

  }

  private static final com.google.protobuf.Descriptors.Descriptor
    internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor;
  private static final 
    com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_fieldAccessorTable;

  public static com.google.protobuf.Descriptors.FileDescriptor
      getDescriptor() {
    return descriptor;
  }
  private static  com.google.protobuf.Descriptors.FileDescriptor
      descriptor;
  static {
    java.lang.String[] descriptorData = {
      "\n&protos/perfetto/trace/ftrace/dpu.proto" +
      "\022\017perfetto.protos\"\201\001\n\036DpuTracingMarkWrit" +
      "eFtraceEvent\022\013\n\003pid\030\001 \001(\005\022\022\n\ntrace_name\030" +
      "\002 \001(\t\022\023\n\013trace_begin\030\003 \001(\r\022\014\n\004name\030\004 \001(\t" +
      "\022\014\n\004type\030\005 \001(\r\022\r\n\005value\030\006 \001(\005"
    };
    descriptor = com.google.protobuf.Descriptors.FileDescriptor
      .internalBuildGeneratedFileFrom(descriptorData,
        new com.google.protobuf.Descriptors.FileDescriptor[] {
        });
    internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor =
      getDescriptor().getMessageTypes().get(0);
    internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_fieldAccessorTable = new
      com.google.protobuf.GeneratedMessageV3.FieldAccessorTable(
        internal_static_perfetto_protos_DpuTracingMarkWriteFtraceEvent_descriptor,
        new java.lang.String[] { "Pid", "TraceName", "TraceBegin", "Name", "Type", "Value", });
  }

  // @@protoc_insertion_point(outer_class_scope)
}
