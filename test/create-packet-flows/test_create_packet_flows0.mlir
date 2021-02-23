// NOTE: Assertions have been autogenerated by utils/update_mlir_test_checks.py
// Note: This test *might* fail due to the random order that the code statements are generated

// RUN: aie-opt --aie-create-packet-flows %s | FileCheck %s

// one-to-many, single arbiter
module @test_create_packet_flows0 {
// CHECK-LABEL:   module @test_create_packet_flows0 {
// CHECK:           %[[VAL_0:.*]] = AIE.tile(1, 1)
// CHECK:           %[[VAL_1:.*]] = AIE.switchbox(%[[VAL_0]]) {
// The actual indices used for the amsel arguments is unimportant.
// CHECK:           %[[VAL_6:.*]] = AIE.amsel<0> (0)
// CHECK:           %[[VAL_7:.*]] = AIE.amsel<0> (1)
// CHECK:           %[[VAL_4:.*]] = AIE.masterset(Core : 0, %[[VAL_2:.*]])
// CHECK:           %[[VAL_5:.*]] = AIE.masterset(Core : 1, %[[VAL_3:.*]])
// CHECK:           AIE.packetrules(West : 0) {
// CHECK:             AIE.rule(31, 1, %[[VAL_3]])
// CHECK:             AIE.rule(31, 0, %[[VAL_2]])
// CHECK:           }
// CHECK:         }
// CHECK:       }
  %t11 = AIE.tile(1, 1)

  AIE.packet_flow(0x0) {
    AIE.packet_source<%t11, West : 0>
    AIE.packet_dest<%t11, Core : 0>
  }

  AIE.packet_flow(0x1) {
    AIE.packet_source<%t11, West : 0>
    AIE.packet_dest<%t11, Core : 1>
  }
}
