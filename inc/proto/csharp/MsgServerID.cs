// <auto-generated>
//     Generated by the protocol buffer compiler.  DO NOT EDIT!
//     source: MsgServerID.proto
// </auto-generated>
#pragma warning disable 1591, 0612, 3021
#region Designer generated code

using pb = global::Google.Protobuf;
using pbc = global::Google.Protobuf.Collections;
using pbr = global::Google.Protobuf.Reflection;
using scg = global::System.Collections.Generic;
/// <summary>Holder for reflection information generated from MsgServerID.proto</summary>
public static partial class MsgServerIDReflection {

  #region Descriptor
  /// <summary>File descriptor for MsgServerID.proto</summary>
  public static pbr::FileDescriptor Descriptor {
    get { return descriptor; }
  }
  private static pbr::FileDescriptor descriptor;

  static MsgServerIDReflection() {
    byte[] descriptorData = global::System.Convert.FromBase64String(
        string.Concat(
          "ChFNc2dTZXJ2ZXJJRC5wcm90bypiCgpTRVJWRVJUWVBFEg0KCURCX1NFUlZF",
          "UhAAEg8KC0dBVEVfU0VSVkVSEAESEAoMTE9HSU5fU0VSVkVSEAISEQoNTUFT",
          "VEVSX1NFUlZFUhADEg8KC0dBTUVfU0VSVkVSEARiBnByb3RvMw=="));
    descriptor = pbr::FileDescriptor.FromGeneratedCode(descriptorData,
        new pbr::FileDescriptor[] { },
        new pbr::GeneratedClrTypeInfo(new[] {typeof(global::SERVERTYPE), }, null));
  }
  #endregion

}
#region Enums
public enum SERVERTYPE {
  [pbr::OriginalName("DB_SERVER")] DbServer = 0,
  [pbr::OriginalName("GATE_SERVER")] GateServer = 1,
  [pbr::OriginalName("LOGIN_SERVER")] LoginServer = 2,
  [pbr::OriginalName("MASTER_SERVER")] MasterServer = 3,
  [pbr::OriginalName("GAME_SERVER")] GameServer = 4,
}

#endregion


#endregion Designer generated code