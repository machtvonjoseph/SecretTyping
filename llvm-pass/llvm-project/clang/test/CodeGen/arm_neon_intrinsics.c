// RUN: %clang_cc1 -triple thumbv7s-apple-darwin -target-abi apcs-gnu\
// RUN:  -target-cpu swift \
// RUN:  -target-feature +fullfp16 -ffreestanding \
// RUN:  -flax-vector-conversions=none \
// RUN:  -disable-O0-optnone -emit-llvm -o - %s \
// RUN:  | opt -S -passes=mem2reg | FileCheck %s

// REQUIRES: aarch64-registered-target || arm-registered-target

#include <arm_neon.h>

// CHECK-LABEL: @test_vaba_s8(
// CHECK:   [[VABD_V_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabds.v8i8(<8 x i8> %b, <8 x i8> %c)
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %a, [[VABD_V_I_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
int8x8_t test_vaba_s8(int8x8_t a, int8x8_t b, int8x8_t c) {
  return vaba_s8(a, b, c);
}

// CHECK-LABEL: @test_vaba_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabds.v4i16(<4 x i16> %b, <4 x i16> %c)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %a, [[VABD_V2_I_I]]
// CHECK:   ret <4 x i16> [[ADD_I]]
int16x4_t test_vaba_s16(int16x4_t a, int16x4_t b, int16x4_t c) {
  return vaba_s16(a, b, c);
}

// CHECK-LABEL: @test_vaba_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabds.v2i32(<2 x i32> %b, <2 x i32> %c)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %a, [[VABD_V2_I_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
int32x2_t test_vaba_s32(int32x2_t a, int32x2_t b, int32x2_t c) {
  return vaba_s32(a, b, c);
}

// CHECK-LABEL: @test_vaba_u8(
// CHECK:   [[VABD_V_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabdu.v8i8(<8 x i8> %b, <8 x i8> %c)
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %a, [[VABD_V_I_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
uint8x8_t test_vaba_u8(uint8x8_t a, uint8x8_t b, uint8x8_t c) {
  return vaba_u8(a, b, c);
}

// CHECK-LABEL: @test_vaba_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabdu.v4i16(<4 x i16> %b, <4 x i16> %c)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %a, [[VABD_V2_I_I]]
// CHECK:   ret <4 x i16> [[ADD_I]]
uint16x4_t test_vaba_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c) {
  return vaba_u16(a, b, c);
}

// CHECK-LABEL: @test_vaba_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabdu.v2i32(<2 x i32> %b, <2 x i32> %c)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %a, [[VABD_V2_I_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
uint32x2_t test_vaba_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c) {
  return vaba_u32(a, b, c);
}

// CHECK-LABEL: @test_vabaq_s8(
// CHECK:   [[VABDQ_V_I_I:%.*]] = call <16 x i8> @llvm.arm.neon.vabds.v16i8(<16 x i8> %b, <16 x i8> %c)
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %a, [[VABDQ_V_I_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
int8x16_t test_vabaq_s8(int8x16_t a, int8x16_t b, int8x16_t c) {
  return vabaq_s8(a, b, c);
}

// CHECK-LABEL: @test_vabaq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %c to <16 x i8>
// CHECK:   [[VABDQ_V2_I_I:%.*]] = call <8 x i16> @llvm.arm.neon.vabds.v8i16(<8 x i16> %b, <8 x i16> %c)
// CHECK:   [[VABDQ_V3_I_I:%.*]] = bitcast <8 x i16> [[VABDQ_V2_I_I]] to <16 x i8>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VABDQ_V2_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vabaq_s16(int16x8_t a, int16x8_t b, int16x8_t c) {
  return vabaq_s16(a, b, c);
}

// CHECK-LABEL: @test_vabaq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %c to <16 x i8>
// CHECK:   [[VABDQ_V2_I_I:%.*]] = call <4 x i32> @llvm.arm.neon.vabds.v4i32(<4 x i32> %b, <4 x i32> %c)
// CHECK:   [[VABDQ_V3_I_I:%.*]] = bitcast <4 x i32> [[VABDQ_V2_I_I]] to <16 x i8>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VABDQ_V2_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vabaq_s32(int32x4_t a, int32x4_t b, int32x4_t c) {
  return vabaq_s32(a, b, c);
}

// CHECK-LABEL: @test_vabaq_u8(
// CHECK:   [[VABDQ_V_I_I:%.*]] = call <16 x i8> @llvm.arm.neon.vabdu.v16i8(<16 x i8> %b, <16 x i8> %c)
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %a, [[VABDQ_V_I_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
uint8x16_t test_vabaq_u8(uint8x16_t a, uint8x16_t b, uint8x16_t c) {
  return vabaq_u8(a, b, c);
}

// CHECK-LABEL: @test_vabaq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %c to <16 x i8>
// CHECK:   [[VABDQ_V2_I_I:%.*]] = call <8 x i16> @llvm.arm.neon.vabdu.v8i16(<8 x i16> %b, <8 x i16> %c)
// CHECK:   [[VABDQ_V3_I_I:%.*]] = bitcast <8 x i16> [[VABDQ_V2_I_I]] to <16 x i8>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VABDQ_V2_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vabaq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c) {
  return vabaq_u16(a, b, c);
}

// CHECK-LABEL: @test_vabaq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %c to <16 x i8>
// CHECK:   [[VABDQ_V2_I_I:%.*]] = call <4 x i32> @llvm.arm.neon.vabdu.v4i32(<4 x i32> %b, <4 x i32> %c)
// CHECK:   [[VABDQ_V3_I_I:%.*]] = bitcast <4 x i32> [[VABDQ_V2_I_I]] to <16 x i8>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VABDQ_V2_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vabaq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c) {
  return vabaq_u32(a, b, c);
}

// CHECK-LABEL: @test_vabal_s8(
// CHECK:   [[VABD_V_I_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabds.v8i8(<8 x i8> %b, <8 x i8> %c)
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <8 x i8> [[VABD_V_I_I_I]] to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vabal_s8(int16x8_t a, int8x8_t b, int8x8_t c) {
  return vabal_s8(a, b, c);
}

// CHECK-LABEL: @test_vabal_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabds.v4i16(<4 x i16> %b, <4 x i16> %c)
// CHECK:   [[VABD_V3_I_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <4 x i16> [[VABD_V2_I_I_I]] to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vabal_s16(int32x4_t a, int16x4_t b, int16x4_t c) {
  return vabal_s16(a, b, c);
}

// CHECK-LABEL: @test_vabal_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabds.v2i32(<2 x i32> %b, <2 x i32> %c)
// CHECK:   [[VABD_V3_I_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <2 x i32> [[VABD_V2_I_I_I]] to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vabal_s32(int64x2_t a, int32x2_t b, int32x2_t c) {
  return vabal_s32(a, b, c);
}

// CHECK-LABEL: @test_vabal_u8(
// CHECK:   [[VABD_V_I_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabdu.v8i8(<8 x i8> %b, <8 x i8> %c)
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <8 x i8> [[VABD_V_I_I_I]] to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vabal_u8(uint16x8_t a, uint8x8_t b, uint8x8_t c) {
  return vabal_u8(a, b, c);
}

// CHECK-LABEL: @test_vabal_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabdu.v4i16(<4 x i16> %b, <4 x i16> %c)
// CHECK:   [[VABD_V3_I_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <4 x i16> [[VABD_V2_I_I_I]] to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vabal_u16(uint32x4_t a, uint16x4_t b, uint16x4_t c) {
  return vabal_u16(a, b, c);
}

// CHECK-LABEL: @test_vabal_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VABD_V2_I_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabdu.v2i32(<2 x i32> %b, <2 x i32> %c)
// CHECK:   [[VABD_V3_I_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I_I:%.*]] = zext <2 x i32> [[VABD_V2_I_I_I]] to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, [[VMOVL_I_I_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vabal_u32(uint64x2_t a, uint32x2_t b, uint32x2_t c) {
  return vabal_u32(a, b, c);
}

// CHECK-LABEL: @test_vabd_s8(
// CHECK:   [[VABD_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabds.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VABD_V_I]]
int8x8_t test_vabd_s8(int8x8_t a, int8x8_t b) {
  return vabd_s8(a, b);
}

// CHECK-LABEL: @test_vabd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VABD_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabds.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VABD_V3_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VABD_V2_I]]
int16x4_t test_vabd_s16(int16x4_t a, int16x4_t b) {
  return vabd_s16(a, b);
}

// CHECK-LABEL: @test_vabd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VABD_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabds.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VABD_V3_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VABD_V2_I]]
int32x2_t test_vabd_s32(int32x2_t a, int32x2_t b) {
  return vabd_s32(a, b);
}

// CHECK-LABEL: @test_vabd_u8(
// CHECK:   [[VABD_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabdu.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VABD_V_I]]
uint8x8_t test_vabd_u8(uint8x8_t a, uint8x8_t b) {
  return vabd_u8(a, b);
}

// CHECK-LABEL: @test_vabd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VABD_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabdu.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VABD_V3_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VABD_V2_I]]
uint16x4_t test_vabd_u16(uint16x4_t a, uint16x4_t b) {
  return vabd_u16(a, b);
}

// CHECK-LABEL: @test_vabd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VABD_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabdu.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VABD_V3_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VABD_V2_I]]
uint32x2_t test_vabd_u32(uint32x2_t a, uint32x2_t b) {
  return vabd_u32(a, b);
}

// CHECK-LABEL: @test_vabd_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VABD_V2_I:%.*]] = call <2 x float> @llvm.arm.neon.vabds.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   [[VABD_V3_I:%.*]] = bitcast <2 x float> [[VABD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x float> [[VABD_V2_I]]
float32x2_t test_vabd_f32(float32x2_t a, float32x2_t b) {
  return vabd_f32(a, b);
}

// CHECK-LABEL: @test_vabdq_s8(
// CHECK:   [[VABDQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vabds.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VABDQ_V_I]]
int8x16_t test_vabdq_s8(int8x16_t a, int8x16_t b) {
  return vabdq_s8(a, b);
}

// CHECK-LABEL: @test_vabdq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VABDQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vabds.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VABDQ_V3_I:%.*]] = bitcast <8 x i16> [[VABDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VABDQ_V2_I]]
int16x8_t test_vabdq_s16(int16x8_t a, int16x8_t b) {
  return vabdq_s16(a, b);
}

// CHECK-LABEL: @test_vabdq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VABDQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vabds.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VABDQ_V3_I:%.*]] = bitcast <4 x i32> [[VABDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VABDQ_V2_I]]
int32x4_t test_vabdq_s32(int32x4_t a, int32x4_t b) {
  return vabdq_s32(a, b);
}

// CHECK-LABEL: @test_vabdq_u8(
// CHECK:   [[VABDQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vabdu.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VABDQ_V_I]]
uint8x16_t test_vabdq_u8(uint8x16_t a, uint8x16_t b) {
  return vabdq_u8(a, b);
}

// CHECK-LABEL: @test_vabdq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VABDQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vabdu.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VABDQ_V3_I:%.*]] = bitcast <8 x i16> [[VABDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VABDQ_V2_I]]
uint16x8_t test_vabdq_u16(uint16x8_t a, uint16x8_t b) {
  return vabdq_u16(a, b);
}

// CHECK-LABEL: @test_vabdq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VABDQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vabdu.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VABDQ_V3_I:%.*]] = bitcast <4 x i32> [[VABDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VABDQ_V2_I]]
uint32x4_t test_vabdq_u32(uint32x4_t a, uint32x4_t b) {
  return vabdq_u32(a, b);
}

// CHECK-LABEL: @test_vabdq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VABDQ_V2_I:%.*]] = call <4 x float> @llvm.arm.neon.vabds.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   [[VABDQ_V3_I:%.*]] = bitcast <4 x float> [[VABDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x float> [[VABDQ_V2_I]]
float32x4_t test_vabdq_f32(float32x4_t a, float32x4_t b) {
  return vabdq_f32(a, b);
}

// CHECK-LABEL: @test_vabdl_s8(
// CHECK:   [[VABD_V_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabds.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   [[VMOVL_I_I:%.*]] = zext <8 x i8> [[VABD_V_I_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[VMOVL_I_I]]
int16x8_t test_vabdl_s8(int8x8_t a, int8x8_t b) {
  return vabdl_s8(a, b);
}

// CHECK-LABEL: @test_vabdl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabds.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <4 x i16> [[VABD_V2_I_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[VMOVL_I_I]]
int32x4_t test_vabdl_s16(int16x4_t a, int16x4_t b) {
  return vabdl_s16(a, b);
}

// CHECK-LABEL: @test_vabdl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabds.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <2 x i32> [[VABD_V2_I_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[VMOVL_I_I]]
int64x2_t test_vabdl_s32(int32x2_t a, int32x2_t b) {
  return vabdl_s32(a, b);
}

// CHECK-LABEL: @test_vabdl_u8(
// CHECK:   [[VABD_V_I_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabdu.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   [[VMOVL_I_I:%.*]] = zext <8 x i8> [[VABD_V_I_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[VMOVL_I_I]]
uint16x8_t test_vabdl_u8(uint8x8_t a, uint8x8_t b) {
  return vabdl_u8(a, b);
}

// CHECK-LABEL: @test_vabdl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabdu.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <4 x i16> [[VABD_V2_I_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[VMOVL_I_I]]
uint32x4_t test_vabdl_u16(uint16x4_t a, uint16x4_t b) {
  return vabdl_u16(a, b);
}

// CHECK-LABEL: @test_vabdl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VABD_V2_I_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabdu.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VABD_V3_I_I:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> [[VABD_V2_I_I]] to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <2 x i32> [[VABD_V2_I_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[VMOVL_I_I]]
uint64x2_t test_vabdl_u32(uint32x2_t a, uint32x2_t b) {
  return vabdl_u32(a, b);
}

// CHECK-LABEL: @test_vabs_s8(
// CHECK:   [[VABS_I:%.*]] = call <8 x i8> @llvm.arm.neon.vabs.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VABS_I]]
int8x8_t test_vabs_s8(int8x8_t a) {
  return vabs_s8(a);
}

// CHECK-LABEL: @test_vabs_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <4 x i16> @llvm.arm.neon.vabs.v4i16(<4 x i16> %a)
// CHECK:   ret <4 x i16> [[VABS1_I]]
int16x4_t test_vabs_s16(int16x4_t a) {
  return vabs_s16(a);
}

// CHECK-LABEL: @test_vabs_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <2 x i32> @llvm.arm.neon.vabs.v2i32(<2 x i32> %a)
// CHECK:   ret <2 x i32> [[VABS1_I]]
int32x2_t test_vabs_s32(int32x2_t a) {
  return vabs_s32(a);
}

// CHECK-LABEL: @test_vabs_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <2 x float> @llvm.fabs.v2f32(<2 x float> %a)
// CHECK:   ret <2 x float> [[VABS1_I]]
float32x2_t test_vabs_f32(float32x2_t a) {
  return vabs_f32(a);
}

// CHECK-LABEL: @test_vabsq_s8(
// CHECK:   [[VABS_I:%.*]] = call <16 x i8> @llvm.arm.neon.vabs.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VABS_I]]
int8x16_t test_vabsq_s8(int8x16_t a) {
  return vabsq_s8(a);
}

// CHECK-LABEL: @test_vabsq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <8 x i16> @llvm.arm.neon.vabs.v8i16(<8 x i16> %a)
// CHECK:   ret <8 x i16> [[VABS1_I]]
int16x8_t test_vabsq_s16(int16x8_t a) {
  return vabsq_s16(a);
}

// CHECK-LABEL: @test_vabsq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <4 x i32> @llvm.arm.neon.vabs.v4i32(<4 x i32> %a)
// CHECK:   ret <4 x i32> [[VABS1_I]]
int32x4_t test_vabsq_s32(int32x4_t a) {
  return vabsq_s32(a);
}

// CHECK-LABEL: @test_vabsq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VABS1_I:%.*]] = call <4 x float> @llvm.fabs.v4f32(<4 x float> %a)
// CHECK:   ret <4 x float> [[VABS1_I]]
float32x4_t test_vabsq_f32(float32x4_t a) {
  return vabsq_f32(a);
}

// CHECK-LABEL: @test_vadd_s8(
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[ADD_I]]
int8x8_t test_vadd_s8(int8x8_t a, int8x8_t b) {
  return vadd_s8(a, b);
}

// CHECK-LABEL: @test_vadd_s16(
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[ADD_I]]
int16x4_t test_vadd_s16(int16x4_t a, int16x4_t b) {
  return vadd_s16(a, b);
}

// CHECK-LABEL: @test_vadd_s32(
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[ADD_I]]
int32x2_t test_vadd_s32(int32x2_t a, int32x2_t b) {
  return vadd_s32(a, b);
}

// CHECK-LABEL: @test_vadd_s64(
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[ADD_I]]
int64x1_t test_vadd_s64(int64x1_t a, int64x1_t b) {
  return vadd_s64(a, b);
}

// CHECK-LABEL: @test_vadd_f32(
// CHECK:   [[ADD_I:%.*]] = fadd <2 x float> %a, %b
// CHECK:   ret <2 x float> [[ADD_I]]
float32x2_t test_vadd_f32(float32x2_t a, float32x2_t b) {
  return vadd_f32(a, b);
}

// CHECK-LABEL: @test_vadd_u8(
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[ADD_I]]
uint8x8_t test_vadd_u8(uint8x8_t a, uint8x8_t b) {
  return vadd_u8(a, b);
}

// CHECK-LABEL: @test_vadd_u16(
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[ADD_I]]
uint16x4_t test_vadd_u16(uint16x4_t a, uint16x4_t b) {
  return vadd_u16(a, b);
}

// CHECK-LABEL: @test_vadd_u32(
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[ADD_I]]
uint32x2_t test_vadd_u32(uint32x2_t a, uint32x2_t b) {
  return vadd_u32(a, b);
}

// CHECK-LABEL: @test_vadd_u64(
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[ADD_I]]
uint64x1_t test_vadd_u64(uint64x1_t a, uint64x1_t b) {
  return vadd_u64(a, b);
}

// CHECK-LABEL: @test_vaddq_s8(
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[ADD_I]]
int8x16_t test_vaddq_s8(int8x16_t a, int8x16_t b) {
  return vaddq_s8(a, b);
}

// CHECK-LABEL: @test_vaddq_s16(
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vaddq_s16(int16x8_t a, int16x8_t b) {
  return vaddq_s16(a, b);
}

// CHECK-LABEL: @test_vaddq_s32(
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vaddq_s32(int32x4_t a, int32x4_t b) {
  return vaddq_s32(a, b);
}

// CHECK-LABEL: @test_vaddq_s64(
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vaddq_s64(int64x2_t a, int64x2_t b) {
  return vaddq_s64(a, b);
}

// CHECK-LABEL: @test_vaddq_f32(
// CHECK:   [[ADD_I:%.*]] = fadd <4 x float> %a, %b
// CHECK:   ret <4 x float> [[ADD_I]]
float32x4_t test_vaddq_f32(float32x4_t a, float32x4_t b) {
  return vaddq_f32(a, b);
}

// CHECK-LABEL: @test_vaddq_u8(
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[ADD_I]]
uint8x16_t test_vaddq_u8(uint8x16_t a, uint8x16_t b) {
  return vaddq_u8(a, b);
}

// CHECK-LABEL: @test_vaddq_u16(
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vaddq_u16(uint16x8_t a, uint16x8_t b) {
  return vaddq_u16(a, b);
}

// CHECK-LABEL: @test_vaddq_u32(
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vaddq_u32(uint32x4_t a, uint32x4_t b) {
  return vaddq_u32(a, b);
}

// CHECK-LABEL: @test_vaddq_u64(
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vaddq_u64(uint64x2_t a, uint64x2_t b) {
  return vaddq_u64(a, b);
}

// CHECK-LABEL: @test_vaddhn_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <8 x i16> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <8 x i16> [[VADDHN_I]], <i16 8, i16 8, i16 8, i16 8, i16 8, i16 8, i16 8, i16 8>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <8 x i16> [[VADDHN1_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VADDHN2_I]]
int8x8_t test_vaddhn_s16(int16x8_t a, int16x8_t b) {
  return vaddhn_s16(a, b);
}

// CHECK-LABEL: @test_vaddhn_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <4 x i32> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <4 x i32> [[VADDHN_I]], <i32 16, i32 16, i32 16, i32 16>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <4 x i32> [[VADDHN1_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VADDHN2_I]]
int16x4_t test_vaddhn_s32(int32x4_t a, int32x4_t b) {
  return vaddhn_s32(a, b);
}

// CHECK-LABEL: @test_vaddhn_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <2 x i64> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <2 x i64> [[VADDHN_I]], <i64 32, i64 32>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <2 x i64> [[VADDHN1_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VADDHN2_I]]
int32x2_t test_vaddhn_s64(int64x2_t a, int64x2_t b) {
  return vaddhn_s64(a, b);
}

// CHECK-LABEL: @test_vaddhn_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <8 x i16> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <8 x i16> [[VADDHN_I]], <i16 8, i16 8, i16 8, i16 8, i16 8, i16 8, i16 8, i16 8>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <8 x i16> [[VADDHN1_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VADDHN2_I]]
uint8x8_t test_vaddhn_u16(uint16x8_t a, uint16x8_t b) {
  return vaddhn_u16(a, b);
}

// CHECK-LABEL: @test_vaddhn_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <4 x i32> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <4 x i32> [[VADDHN_I]], <i32 16, i32 16, i32 16, i32 16>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <4 x i32> [[VADDHN1_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VADDHN2_I]]
uint16x4_t test_vaddhn_u32(uint32x4_t a, uint32x4_t b) {
  return vaddhn_u32(a, b);
}

// CHECK-LABEL: @test_vaddhn_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VADDHN_I:%.*]] = add <2 x i64> %a, %b
// CHECK:   [[VADDHN1_I:%.*]] = lshr <2 x i64> [[VADDHN_I]], <i64 32, i64 32>
// CHECK:   [[VADDHN2_I:%.*]] = trunc <2 x i64> [[VADDHN1_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VADDHN2_I]]
uint32x2_t test_vaddhn_u64(uint64x2_t a, uint64x2_t b) {
  return vaddhn_u64(a, b);
}

// CHECK-LABEL: @test_vaddl_s8(
// CHECK:   [[VMOVL_I_I:%.*]] = sext <8 x i8> %a to <8 x i16>
// CHECK:   [[VMOVL_I4_I:%.*]] = sext <8 x i8> %b to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vaddl_s8(int8x8_t a, int8x8_t b) {
  return vaddl_s8(a, b);
}

// CHECK-LABEL: @test_vaddl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = sext <4 x i16> %a to <4 x i32>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMOVL_I4_I:%.*]] = sext <4 x i16> %b to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vaddl_s16(int16x4_t a, int16x4_t b) {
  return vaddl_s16(a, b);
}

// CHECK-LABEL: @test_vaddl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = sext <2 x i32> %a to <2 x i64>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMOVL_I4_I:%.*]] = sext <2 x i32> %b to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vaddl_s32(int32x2_t a, int32x2_t b) {
  return vaddl_s32(a, b);
}

// CHECK-LABEL: @test_vaddl_u8(
// CHECK:   [[VMOVL_I_I:%.*]] = zext <8 x i8> %a to <8 x i16>
// CHECK:   [[VMOVL_I4_I:%.*]] = zext <8 x i8> %b to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vaddl_u8(uint8x8_t a, uint8x8_t b) {
  return vaddl_u8(a, b);
}

// CHECK-LABEL: @test_vaddl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <4 x i16> %a to <4 x i32>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMOVL_I4_I:%.*]] = zext <4 x i16> %b to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vaddl_u16(uint16x4_t a, uint16x4_t b) {
  return vaddl_u16(a, b);
}

// CHECK-LABEL: @test_vaddl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <2 x i32> %a to <2 x i64>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMOVL_I4_I:%.*]] = zext <2 x i32> %b to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> [[VMOVL_I_I]], [[VMOVL_I4_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vaddl_u32(uint32x2_t a, uint32x2_t b) {
  return vaddl_u32(a, b);
}

// CHECK-LABEL: @test_vaddw_s8(
// CHECK:   [[VMOVL_I_I:%.*]] = sext <8 x i8> %b to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VMOVL_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vaddw_s8(int16x8_t a, int8x8_t b) {
  return vaddw_s8(a, b);
}

// CHECK-LABEL: @test_vaddw_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = sext <4 x i16> %b to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VMOVL_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vaddw_s16(int32x4_t a, int16x4_t b) {
  return vaddw_s16(a, b);
}

// CHECK-LABEL: @test_vaddw_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = sext <2 x i32> %b to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, [[VMOVL_I_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vaddw_s32(int64x2_t a, int32x2_t b) {
  return vaddw_s32(a, b);
}

// CHECK-LABEL: @test_vaddw_u8(
// CHECK:   [[VMOVL_I_I:%.*]] = zext <8 x i8> %b to <8 x i16>
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %a, [[VMOVL_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vaddw_u8(uint16x8_t a, uint8x8_t b) {
  return vaddw_u8(a, b);
}

// CHECK-LABEL: @test_vaddw_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <4 x i16> %b to <4 x i32>
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %a, [[VMOVL_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vaddw_u16(uint32x4_t a, uint16x4_t b) {
  return vaddw_u16(a, b);
}

// CHECK-LABEL: @test_vaddw_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMOVL_I_I:%.*]] = zext <2 x i32> %b to <2 x i64>
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %a, [[VMOVL_I_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vaddw_u32(uint64x2_t a, uint32x2_t b) {
  return vaddw_u32(a, b);
}

// CHECK-LABEL: @test_vand_s8(
// CHECK:   [[AND_I:%.*]] = and <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[AND_I]]
int8x8_t test_vand_s8(int8x8_t a, int8x8_t b) {
  return vand_s8(a, b);
}

// CHECK-LABEL: @test_vand_s16(
// CHECK:   [[AND_I:%.*]] = and <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[AND_I]]
int16x4_t test_vand_s16(int16x4_t a, int16x4_t b) {
  return vand_s16(a, b);
}

// CHECK-LABEL: @test_vand_s32(
// CHECK:   [[AND_I:%.*]] = and <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[AND_I]]
int32x2_t test_vand_s32(int32x2_t a, int32x2_t b) {
  return vand_s32(a, b);
}

// CHECK-LABEL: @test_vand_s64(
// CHECK:   [[AND_I:%.*]] = and <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[AND_I]]
int64x1_t test_vand_s64(int64x1_t a, int64x1_t b) {
  return vand_s64(a, b);
}

// CHECK-LABEL: @test_vand_u8(
// CHECK:   [[AND_I:%.*]] = and <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[AND_I]]
uint8x8_t test_vand_u8(uint8x8_t a, uint8x8_t b) {
  return vand_u8(a, b);
}

// CHECK-LABEL: @test_vand_u16(
// CHECK:   [[AND_I:%.*]] = and <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[AND_I]]
uint16x4_t test_vand_u16(uint16x4_t a, uint16x4_t b) {
  return vand_u16(a, b);
}

// CHECK-LABEL: @test_vand_u32(
// CHECK:   [[AND_I:%.*]] = and <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[AND_I]]
uint32x2_t test_vand_u32(uint32x2_t a, uint32x2_t b) {
  return vand_u32(a, b);
}

// CHECK-LABEL: @test_vand_u64(
// CHECK:   [[AND_I:%.*]] = and <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[AND_I]]
uint64x1_t test_vand_u64(uint64x1_t a, uint64x1_t b) {
  return vand_u64(a, b);
}

// CHECK-LABEL: @test_vandq_s8(
// CHECK:   [[AND_I:%.*]] = and <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[AND_I]]
int8x16_t test_vandq_s8(int8x16_t a, int8x16_t b) {
  return vandq_s8(a, b);
}

// CHECK-LABEL: @test_vandq_s16(
// CHECK:   [[AND_I:%.*]] = and <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[AND_I]]
int16x8_t test_vandq_s16(int16x8_t a, int16x8_t b) {
  return vandq_s16(a, b);
}

// CHECK-LABEL: @test_vandq_s32(
// CHECK:   [[AND_I:%.*]] = and <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[AND_I]]
int32x4_t test_vandq_s32(int32x4_t a, int32x4_t b) {
  return vandq_s32(a, b);
}

// CHECK-LABEL: @test_vandq_s64(
// CHECK:   [[AND_I:%.*]] = and <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[AND_I]]
int64x2_t test_vandq_s64(int64x2_t a, int64x2_t b) {
  return vandq_s64(a, b);
}

// CHECK-LABEL: @test_vandq_u8(
// CHECK:   [[AND_I:%.*]] = and <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[AND_I]]
uint8x16_t test_vandq_u8(uint8x16_t a, uint8x16_t b) {
  return vandq_u8(a, b);
}

// CHECK-LABEL: @test_vandq_u16(
// CHECK:   [[AND_I:%.*]] = and <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[AND_I]]
uint16x8_t test_vandq_u16(uint16x8_t a, uint16x8_t b) {
  return vandq_u16(a, b);
}

// CHECK-LABEL: @test_vandq_u32(
// CHECK:   [[AND_I:%.*]] = and <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[AND_I]]
uint32x4_t test_vandq_u32(uint32x4_t a, uint32x4_t b) {
  return vandq_u32(a, b);
}

// CHECK-LABEL: @test_vandq_u64(
// CHECK:   [[AND_I:%.*]] = and <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[AND_I]]
uint64x2_t test_vandq_u64(uint64x2_t a, uint64x2_t b) {
  return vandq_u64(a, b);
}

// CHECK-LABEL: @test_vbic_s8(
// CHECK:   [[NEG_I:%.*]] = xor <8 x i8> %b, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[AND_I:%.*]] = and <8 x i8> %a, [[NEG_I]]
// CHECK:   ret <8 x i8> [[AND_I]]
int8x8_t test_vbic_s8(int8x8_t a, int8x8_t b) {
  return vbic_s8(a, b);
}

// CHECK-LABEL: @test_vbic_s16(
// CHECK:   [[NEG_I:%.*]] = xor <4 x i16> %b, <i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[AND_I:%.*]] = and <4 x i16> %a, [[NEG_I]]
// CHECK:   ret <4 x i16> [[AND_I]]
int16x4_t test_vbic_s16(int16x4_t a, int16x4_t b) {
  return vbic_s16(a, b);
}

// CHECK-LABEL: @test_vbic_s32(
// CHECK:   [[NEG_I:%.*]] = xor <2 x i32> %b, <i32 -1, i32 -1>
// CHECK:   [[AND_I:%.*]] = and <2 x i32> %a, [[NEG_I]]
// CHECK:   ret <2 x i32> [[AND_I]]
int32x2_t test_vbic_s32(int32x2_t a, int32x2_t b) {
  return vbic_s32(a, b);
}

// CHECK-LABEL: @test_vbic_s64(
// CHECK:   [[NEG_I:%.*]] = xor <1 x i64> %b, <i64 -1>
// CHECK:   [[AND_I:%.*]] = and <1 x i64> %a, [[NEG_I]]
// CHECK:   ret <1 x i64> [[AND_I]]
int64x1_t test_vbic_s64(int64x1_t a, int64x1_t b) {
  return vbic_s64(a, b);
}

// CHECK-LABEL: @test_vbic_u8(
// CHECK:   [[NEG_I:%.*]] = xor <8 x i8> %b, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[AND_I:%.*]] = and <8 x i8> %a, [[NEG_I]]
// CHECK:   ret <8 x i8> [[AND_I]]
uint8x8_t test_vbic_u8(uint8x8_t a, uint8x8_t b) {
  return vbic_u8(a, b);
}

// CHECK-LABEL: @test_vbic_u16(
// CHECK:   [[NEG_I:%.*]] = xor <4 x i16> %b, <i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[AND_I:%.*]] = and <4 x i16> %a, [[NEG_I]]
// CHECK:   ret <4 x i16> [[AND_I]]
uint16x4_t test_vbic_u16(uint16x4_t a, uint16x4_t b) {
  return vbic_u16(a, b);
}

// CHECK-LABEL: @test_vbic_u32(
// CHECK:   [[NEG_I:%.*]] = xor <2 x i32> %b, <i32 -1, i32 -1>
// CHECK:   [[AND_I:%.*]] = and <2 x i32> %a, [[NEG_I]]
// CHECK:   ret <2 x i32> [[AND_I]]
uint32x2_t test_vbic_u32(uint32x2_t a, uint32x2_t b) {
  return vbic_u32(a, b);
}

// CHECK-LABEL: @test_vbic_u64(
// CHECK:   [[NEG_I:%.*]] = xor <1 x i64> %b, <i64 -1>
// CHECK:   [[AND_I:%.*]] = and <1 x i64> %a, [[NEG_I]]
// CHECK:   ret <1 x i64> [[AND_I]]
uint64x1_t test_vbic_u64(uint64x1_t a, uint64x1_t b) {
  return vbic_u64(a, b);
}

// CHECK-LABEL: @test_vbicq_s8(
// CHECK:   [[NEG_I:%.*]] = xor <16 x i8> %b, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[AND_I:%.*]] = and <16 x i8> %a, [[NEG_I]]
// CHECK:   ret <16 x i8> [[AND_I]]
int8x16_t test_vbicq_s8(int8x16_t a, int8x16_t b) {
  return vbicq_s8(a, b);
}

// CHECK-LABEL: @test_vbicq_s16(
// CHECK:   [[NEG_I:%.*]] = xor <8 x i16> %b, <i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[AND_I:%.*]] = and <8 x i16> %a, [[NEG_I]]
// CHECK:   ret <8 x i16> [[AND_I]]
int16x8_t test_vbicq_s16(int16x8_t a, int16x8_t b) {
  return vbicq_s16(a, b);
}

// CHECK-LABEL: @test_vbicq_s32(
// CHECK:   [[NEG_I:%.*]] = xor <4 x i32> %b, <i32 -1, i32 -1, i32 -1, i32 -1>
// CHECK:   [[AND_I:%.*]] = and <4 x i32> %a, [[NEG_I]]
// CHECK:   ret <4 x i32> [[AND_I]]
int32x4_t test_vbicq_s32(int32x4_t a, int32x4_t b) {
  return vbicq_s32(a, b);
}

// CHECK-LABEL: @test_vbicq_s64(
// CHECK:   [[NEG_I:%.*]] = xor <2 x i64> %b, <i64 -1, i64 -1>
// CHECK:   [[AND_I:%.*]] = and <2 x i64> %a, [[NEG_I]]
// CHECK:   ret <2 x i64> [[AND_I]]
int64x2_t test_vbicq_s64(int64x2_t a, int64x2_t b) {
  return vbicq_s64(a, b);
}

// CHECK-LABEL: @test_vbicq_u8(
// CHECK:   [[NEG_I:%.*]] = xor <16 x i8> %b, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[AND_I:%.*]] = and <16 x i8> %a, [[NEG_I]]
// CHECK:   ret <16 x i8> [[AND_I]]
uint8x16_t test_vbicq_u8(uint8x16_t a, uint8x16_t b) {
  return vbicq_u8(a, b);
}

// CHECK-LABEL: @test_vbicq_u16(
// CHECK:   [[NEG_I:%.*]] = xor <8 x i16> %b, <i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[AND_I:%.*]] = and <8 x i16> %a, [[NEG_I]]
// CHECK:   ret <8 x i16> [[AND_I]]
uint16x8_t test_vbicq_u16(uint16x8_t a, uint16x8_t b) {
  return vbicq_u16(a, b);
}

// CHECK-LABEL: @test_vbicq_u32(
// CHECK:   [[NEG_I:%.*]] = xor <4 x i32> %b, <i32 -1, i32 -1, i32 -1, i32 -1>
// CHECK:   [[AND_I:%.*]] = and <4 x i32> %a, [[NEG_I]]
// CHECK:   ret <4 x i32> [[AND_I]]
uint32x4_t test_vbicq_u32(uint32x4_t a, uint32x4_t b) {
  return vbicq_u32(a, b);
}

// CHECK-LABEL: @test_vbicq_u64(
// CHECK:   [[NEG_I:%.*]] = xor <2 x i64> %b, <i64 -1, i64 -1>
// CHECK:   [[AND_I:%.*]] = and <2 x i64> %a, [[NEG_I]]
// CHECK:   ret <2 x i64> [[AND_I]]
uint64x2_t test_vbicq_u64(uint64x2_t a, uint64x2_t b) {
  return vbicq_u64(a, b);
}

// CHECK-LABEL: @test_vbsl_s8(
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> %a, <8 x i8> %b, <8 x i8> %c)
// CHECK:   ret <8 x i8> [[VBSL_V_I]]
int8x8_t test_vbsl_s8(uint8x8_t a, int8x8_t b, int8x8_t c) {
  return vbsl_s8(a, b, c);
}

// CHECK-LABEL: @test_vbsl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[TMP3]]
int16x4_t test_vbsl_s16(uint16x4_t a, int16x4_t b, int16x4_t c) {
  return vbsl_s16(a, b, c);
}

// CHECK-LABEL: @test_vbsl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[TMP3]]
int32x2_t test_vbsl_s32(uint32x2_t a, int32x2_t b, int32x2_t c) {
  return vbsl_s32(a, b, c);
}

// CHECK-LABEL: @test_vbsl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <1 x i64> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[TMP3]]
int64x1_t test_vbsl_s64(uint64x1_t a, int64x1_t b, int64x1_t c) {
  return vbsl_s64(a, b, c);
}

// CHECK-LABEL: @test_vbsl_u8(
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> %a, <8 x i8> %b, <8 x i8> %c)
// CHECK:   ret <8 x i8> [[VBSL_V_I]]
uint8x8_t test_vbsl_u8(uint8x8_t a, uint8x8_t b, uint8x8_t c) {
  return vbsl_u8(a, b, c);
}

// CHECK-LABEL: @test_vbsl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[TMP3]]
uint16x4_t test_vbsl_u16(uint16x4_t a, uint16x4_t b, uint16x4_t c) {
  return vbsl_u16(a, b, c);
}

// CHECK-LABEL: @test_vbsl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[TMP3]]
uint32x2_t test_vbsl_u32(uint32x2_t a, uint32x2_t b, uint32x2_t c) {
  return vbsl_u32(a, b, c);
}

// CHECK-LABEL: @test_vbsl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <1 x i64> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[TMP3]]
uint64x1_t test_vbsl_u64(uint64x1_t a, uint64x1_t b, uint64x1_t c) {
  return vbsl_u64(a, b, c);
}

// CHECK-LABEL: @test_vbsl_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <2 x float>
// CHECK:   ret <2 x float> [[TMP3]]
float32x2_t test_vbsl_f32(uint32x2_t a, float32x2_t b, float32x2_t c) {
  return vbsl_f32(a, b, c);
}

// CHECK-LABEL: @test_vbsl_p8(
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> %a, <8 x i8> %b, <8 x i8> %c)
// CHECK:   ret <8 x i8> [[VBSL_V_I]]
poly8x8_t test_vbsl_p8(uint8x8_t a, poly8x8_t b, poly8x8_t c) {
  return vbsl_p8(a, b, c);
}

// CHECK-LABEL: @test_vbsl_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %c to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP0]], <8 x i8> [[TMP1]], <8 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[TMP3]]
poly16x4_t test_vbsl_p16(uint16x4_t a, poly16x4_t b, poly16x4_t c) {
  return vbsl_p16(a, b, c);
}

// CHECK-LABEL: @test_vbslq_s8(
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> %a, <16 x i8> %b, <16 x i8> %c)
// CHECK:   ret <16 x i8> [[VBSLQ_V_I]]
int8x16_t test_vbslq_s8(uint8x16_t a, int8x16_t b, int8x16_t c) {
  return vbslq_s8(a, b, c);
}

// CHECK-LABEL: @test_vbslq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[TMP3]]
int16x8_t test_vbslq_s16(uint16x8_t a, int16x8_t b, int16x8_t c) {
  return vbslq_s16(a, b, c);
}

// CHECK-LABEL: @test_vbslq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i32> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[TMP3]]
int32x4_t test_vbslq_s32(uint32x4_t a, int32x4_t b, int32x4_t c) {
  return vbslq_s32(a, b, c);
}

// CHECK-LABEL: @test_vbslq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i64> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[TMP3]]
int64x2_t test_vbslq_s64(uint64x2_t a, int64x2_t b, int64x2_t c) {
  return vbslq_s64(a, b, c);
}

// CHECK-LABEL: @test_vbslq_u8(
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> %a, <16 x i8> %b, <16 x i8> %c)
// CHECK:   ret <16 x i8> [[VBSLQ_V_I]]
uint8x16_t test_vbslq_u8(uint8x16_t a, uint8x16_t b, uint8x16_t c) {
  return vbslq_u8(a, b, c);
}

// CHECK-LABEL: @test_vbslq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[TMP3]]
uint16x8_t test_vbslq_u16(uint16x8_t a, uint16x8_t b, uint16x8_t c) {
  return vbslq_u16(a, b, c);
}

// CHECK-LABEL: @test_vbslq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i32> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[TMP3]]
uint32x4_t test_vbslq_u32(uint32x4_t a, uint32x4_t b, uint32x4_t c) {
  return vbslq_u32(a, b, c);
}

// CHECK-LABEL: @test_vbslq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i64> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[TMP3]]
uint64x2_t test_vbslq_u64(uint64x2_t a, uint64x2_t b, uint64x2_t c) {
  return vbslq_u64(a, b, c);
}

// CHECK-LABEL: @test_vbslq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <4 x float>
// CHECK:   ret <4 x float> [[TMP3]]
float32x4_t test_vbslq_f32(uint32x4_t a, float32x4_t b, float32x4_t c) {
  return vbslq_f32(a, b, c);
}

// CHECK-LABEL: @test_vbslq_p8(
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> %a, <16 x i8> %b, <16 x i8> %c)
// CHECK:   ret <16 x i8> [[VBSLQ_V_I]]
poly8x16_t test_vbslq_p8(uint8x16_t a, poly8x16_t b, poly8x16_t c) {
  return vbslq_p8(a, b, c);
}

// CHECK-LABEL: @test_vbslq_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %c to <16 x i8>
// CHECK:   [[VBSLQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vbsl.v16i8(<16 x i8> [[TMP0]], <16 x i8> [[TMP1]], <16 x i8> [[TMP2]])
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[VBSLQ_V_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[TMP3]]
poly16x8_t test_vbslq_p16(uint16x8_t a, poly16x8_t b, poly16x8_t c) {
  return vbslq_p16(a, b, c);
}

// CHECK-LABEL: @test_vcage_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VCAGE_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vacge.v2i32.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x i32> [[VCAGE_V2_I]]
uint32x2_t test_vcage_f32(float32x2_t a, float32x2_t b) {
  return vcage_f32(a, b);
}

// CHECK-LABEL: @test_vcageq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VCAGEQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vacge.v4i32.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x i32> [[VCAGEQ_V2_I]]
uint32x4_t test_vcageq_f32(float32x4_t a, float32x4_t b) {
  return vcageq_f32(a, b);
}

// CHECK-LABEL: @test_vcagt_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VCAGT_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vacgt.v2i32.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x i32> [[VCAGT_V2_I]]
uint32x2_t test_vcagt_f32(float32x2_t a, float32x2_t b) {
  return vcagt_f32(a, b);
}

// CHECK-LABEL: @test_vcagtq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VCAGTQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vacgt.v4i32.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x i32> [[VCAGTQ_V2_I]]
uint32x4_t test_vcagtq_f32(float32x4_t a, float32x4_t b) {
  return vcagtq_f32(a, b);
}

// CHECK-LABEL: @test_vcale_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VCALE_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vacge.v2i32.v2f32(<2 x float> %b, <2 x float> %a)
// CHECK:   ret <2 x i32> [[VCALE_V2_I]]
uint32x2_t test_vcale_f32(float32x2_t a, float32x2_t b) {
  return vcale_f32(a, b);
}

// CHECK-LABEL: @test_vcaleq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VCALEQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vacge.v4i32.v4f32(<4 x float> %b, <4 x float> %a)
// CHECK:   ret <4 x i32> [[VCALEQ_V2_I]]
uint32x4_t test_vcaleq_f32(float32x4_t a, float32x4_t b) {
  return vcaleq_f32(a, b);
}

// CHECK-LABEL: @test_vcalt_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VCALT_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vacgt.v2i32.v2f32(<2 x float> %b, <2 x float> %a)
// CHECK:   ret <2 x i32> [[VCALT_V2_I]]
uint32x2_t test_vcalt_f32(float32x2_t a, float32x2_t b) {
  return vcalt_f32(a, b);
}

// CHECK-LABEL: @test_vcaltq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VCALTQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vacgt.v4i32.v4f32(<4 x float> %b, <4 x float> %a)
// CHECK:   ret <4 x i32> [[VCALTQ_V2_I]]
uint32x4_t test_vcaltq_f32(float32x4_t a, float32x4_t b) {
  return vcaltq_f32(a, b);
}

// CHECK-LABEL: @test_vceq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_s8(int8x8_t a, int8x8_t b) {
  return vceq_s8(a, b);
}

// CHECK-LABEL: @test_vceq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vceq_s16(int16x4_t a, int16x4_t b) {
  return vceq_s16(a, b);
}

// CHECK-LABEL: @test_vceq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_s32(int32x2_t a, int32x2_t b) {
  return vceq_s32(a, b);
}

// CHECK-LABEL: @test_vceq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <2 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_f32(float32x2_t a, float32x2_t b) {
  return vceq_f32(a, b);
}

// CHECK-LABEL: @test_vceq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_u8(uint8x8_t a, uint8x8_t b) {
  return vceq_u8(a, b);
}

// CHECK-LABEL: @test_vceq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vceq_u16(uint16x4_t a, uint16x4_t b) {
  return vceq_u16(a, b);
}

// CHECK-LABEL: @test_vceq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_u32(uint32x2_t a, uint32x2_t b) {
  return vceq_u32(a, b);
}

// CHECK-LABEL: @test_vceq_p8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_p8(poly8x8_t a, poly8x8_t b) {
  return vceq_p8(a, b);
}

// CHECK-LABEL: @test_vceqq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_s8(int8x16_t a, int8x16_t b) {
  return vceqq_s8(a, b);
}

// CHECK-LABEL: @test_vceqq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vceqq_s16(int16x8_t a, int16x8_t b) {
  return vceqq_s16(a, b);
}

// CHECK-LABEL: @test_vceqq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_s32(int32x4_t a, int32x4_t b) {
  return vceqq_s32(a, b);
}

// CHECK-LABEL: @test_vceqq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <4 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_f32(float32x4_t a, float32x4_t b) {
  return vceqq_f32(a, b);
}

// CHECK-LABEL: @test_vceqq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_u8(uint8x16_t a, uint8x16_t b) {
  return vceqq_u8(a, b);
}

// CHECK-LABEL: @test_vceqq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vceqq_u16(uint16x8_t a, uint16x8_t b) {
  return vceqq_u16(a, b);
}

// CHECK-LABEL: @test_vceqq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_u32(uint32x4_t a, uint32x4_t b) {
  return vceqq_u32(a, b);
}

// CHECK-LABEL: @test_vceqq_p8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_p8(poly8x16_t a, poly8x16_t b) {
  return vceqq_p8(a, b);
}

// CHECK-LABEL: @test_vcge_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sge <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcge_s8(int8x8_t a, int8x8_t b) {
  return vcge_s8(a, b);
}

// CHECK-LABEL: @test_vcge_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sge <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcge_s16(int16x4_t a, int16x4_t b) {
  return vcge_s16(a, b);
}

// CHECK-LABEL: @test_vcge_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sge <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_s32(int32x2_t a, int32x2_t b) {
  return vcge_s32(a, b);
}

// CHECK-LABEL: @test_vcge_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <2 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_f32(float32x2_t a, float32x2_t b) {
  return vcge_f32(a, b);
}

// CHECK-LABEL: @test_vcge_u8(
// CHECK:   [[CMP_I:%.*]] = icmp uge <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcge_u8(uint8x8_t a, uint8x8_t b) {
  return vcge_u8(a, b);
}

// CHECK-LABEL: @test_vcge_u16(
// CHECK:   [[CMP_I:%.*]] = icmp uge <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcge_u16(uint16x4_t a, uint16x4_t b) {
  return vcge_u16(a, b);
}

// CHECK-LABEL: @test_vcge_u32(
// CHECK:   [[CMP_I:%.*]] = icmp uge <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_u32(uint32x2_t a, uint32x2_t b) {
  return vcge_u32(a, b);
}

// CHECK-LABEL: @test_vcgeq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sge <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgeq_s8(int8x16_t a, int8x16_t b) {
  return vcgeq_s8(a, b);
}

// CHECK-LABEL: @test_vcgeq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sge <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgeq_s16(int16x8_t a, int16x8_t b) {
  return vcgeq_s16(a, b);
}

// CHECK-LABEL: @test_vcgeq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sge <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_s32(int32x4_t a, int32x4_t b) {
  return vcgeq_s32(a, b);
}

// CHECK-LABEL: @test_vcgeq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <4 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_f32(float32x4_t a, float32x4_t b) {
  return vcgeq_f32(a, b);
}

// CHECK-LABEL: @test_vcgeq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp uge <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgeq_u8(uint8x16_t a, uint8x16_t b) {
  return vcgeq_u8(a, b);
}

// CHECK-LABEL: @test_vcgeq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp uge <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgeq_u16(uint16x8_t a, uint16x8_t b) {
  return vcgeq_u16(a, b);
}

// CHECK-LABEL: @test_vcgeq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp uge <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_u32(uint32x4_t a, uint32x4_t b) {
  return vcgeq_u32(a, b);
}

// CHECK-LABEL: @test_vcgt_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcgt_s8(int8x8_t a, int8x8_t b) {
  return vcgt_s8(a, b);
}

// CHECK-LABEL: @test_vcgt_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcgt_s16(int16x4_t a, int16x4_t b) {
  return vcgt_s16(a, b);
}

// CHECK-LABEL: @test_vcgt_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_s32(int32x2_t a, int32x2_t b) {
  return vcgt_s32(a, b);
}

// CHECK-LABEL: @test_vcgt_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <2 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_f32(float32x2_t a, float32x2_t b) {
  return vcgt_f32(a, b);
}

// CHECK-LABEL: @test_vcgt_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcgt_u8(uint8x8_t a, uint8x8_t b) {
  return vcgt_u8(a, b);
}

// CHECK-LABEL: @test_vcgt_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcgt_u16(uint16x4_t a, uint16x4_t b) {
  return vcgt_u16(a, b);
}

// CHECK-LABEL: @test_vcgt_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_u32(uint32x2_t a, uint32x2_t b) {
  return vcgt_u32(a, b);
}

// CHECK-LABEL: @test_vcgtq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgtq_s8(int8x16_t a, int8x16_t b) {
  return vcgtq_s8(a, b);
}

// CHECK-LABEL: @test_vcgtq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgtq_s16(int16x8_t a, int16x8_t b) {
  return vcgtq_s16(a, b);
}

// CHECK-LABEL: @test_vcgtq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_s32(int32x4_t a, int32x4_t b) {
  return vcgtq_s32(a, b);
}

// CHECK-LABEL: @test_vcgtq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <4 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_f32(float32x4_t a, float32x4_t b) {
  return vcgtq_f32(a, b);
}

// CHECK-LABEL: @test_vcgtq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgtq_u8(uint8x16_t a, uint8x16_t b) {
  return vcgtq_u8(a, b);
}

// CHECK-LABEL: @test_vcgtq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgtq_u16(uint16x8_t a, uint16x8_t b) {
  return vcgtq_u16(a, b);
}

// CHECK-LABEL: @test_vcgtq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_u32(uint32x4_t a, uint32x4_t b) {
  return vcgtq_u32(a, b);
}

// CHECK-LABEL: @test_vcle_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sle <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcle_s8(int8x8_t a, int8x8_t b) {
  return vcle_s8(a, b);
}

// CHECK-LABEL: @test_vcle_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sle <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcle_s16(int16x4_t a, int16x4_t b) {
  return vcle_s16(a, b);
}

// CHECK-LABEL: @test_vcle_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sle <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_s32(int32x2_t a, int32x2_t b) {
  return vcle_s32(a, b);
}

// CHECK-LABEL: @test_vcle_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <2 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_f32(float32x2_t a, float32x2_t b) {
  return vcle_f32(a, b);
}

// CHECK-LABEL: @test_vcle_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ule <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcle_u8(uint8x8_t a, uint8x8_t b) {
  return vcle_u8(a, b);
}

// CHECK-LABEL: @test_vcle_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ule <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcle_u16(uint16x4_t a, uint16x4_t b) {
  return vcle_u16(a, b);
}

// CHECK-LABEL: @test_vcle_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ule <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_u32(uint32x2_t a, uint32x2_t b) {
  return vcle_u32(a, b);
}

// CHECK-LABEL: @test_vcleq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sle <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcleq_s8(int8x16_t a, int8x16_t b) {
  return vcleq_s8(a, b);
}

// CHECK-LABEL: @test_vcleq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sle <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcleq_s16(int16x8_t a, int16x8_t b) {
  return vcleq_s16(a, b);
}

// CHECK-LABEL: @test_vcleq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sle <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_s32(int32x4_t a, int32x4_t b) {
  return vcleq_s32(a, b);
}

// CHECK-LABEL: @test_vcleq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <4 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_f32(float32x4_t a, float32x4_t b) {
  return vcleq_f32(a, b);
}

// CHECK-LABEL: @test_vcleq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ule <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcleq_u8(uint8x16_t a, uint8x16_t b) {
  return vcleq_u8(a, b);
}

// CHECK-LABEL: @test_vcleq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ule <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcleq_u16(uint16x8_t a, uint16x8_t b) {
  return vcleq_u16(a, b);
}

// CHECK-LABEL: @test_vcleq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ule <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_u32(uint32x4_t a, uint32x4_t b) {
  return vcleq_u32(a, b);
}

// CHECK-LABEL: @test_vcls_s8(
// CHECK:   [[VCLS_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vcls.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VCLS_V_I]]
int8x8_t test_vcls_s8(int8x8_t a) {
  return vcls_s8(a);
}

// CHECK-LABEL: @test_vcls_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VCLS_V1_I:%.*]] = call <4 x i16> @llvm.arm.neon.vcls.v4i16(<4 x i16> %a)
// CHECK:   [[VCLS_V2_I:%.*]] = bitcast <4 x i16> [[VCLS_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLS_V1_I]]
int16x4_t test_vcls_s16(int16x4_t a) {
  return vcls_s16(a);
}

// CHECK-LABEL: @test_vcls_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCLS_V1_I:%.*]] = call <2 x i32> @llvm.arm.neon.vcls.v2i32(<2 x i32> %a)
// CHECK:   [[VCLS_V2_I:%.*]] = bitcast <2 x i32> [[VCLS_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLS_V1_I]]
int32x2_t test_vcls_s32(int32x2_t a) {
  return vcls_s32(a);
}

// CHECK-LABEL: @test_vcls_u8(
// CHECK:   [[VCLS_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vcls.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VCLS_V_I]]
int8x8_t test_vcls_u8(uint8x8_t a) {
  return vcls_u8(a);
}

// CHECK-LABEL: @test_vcls_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VCLS_V1_I:%.*]] = call <4 x i16> @llvm.arm.neon.vcls.v4i16(<4 x i16> %a)
// CHECK:   [[VCLS_V2_I:%.*]] = bitcast <4 x i16> [[VCLS_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLS_V1_I]]
int16x4_t test_vcls_u16(uint16x4_t a) {
  return vcls_u16(a);
}

// CHECK-LABEL: @test_vcls_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCLS_V1_I:%.*]] = call <2 x i32> @llvm.arm.neon.vcls.v2i32(<2 x i32> %a)
// CHECK:   [[VCLS_V2_I:%.*]] = bitcast <2 x i32> [[VCLS_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLS_V1_I]]
int32x2_t test_vcls_u32(uint32x2_t a) {
  return vcls_u32(a);
}

// CHECK-LABEL: @test_vclsq_s8(
// CHECK:   [[VCLSQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vcls.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VCLSQ_V_I]]
int8x16_t test_vclsq_s8(int8x16_t a) {
  return vclsq_s8(a);
}

// CHECK-LABEL: @test_vclsq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VCLSQ_V1_I:%.*]] = call <8 x i16> @llvm.arm.neon.vcls.v8i16(<8 x i16> %a)
// CHECK:   [[VCLSQ_V2_I:%.*]] = bitcast <8 x i16> [[VCLSQ_V1_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VCLSQ_V1_I]]
int16x8_t test_vclsq_s16(int16x8_t a) {
  return vclsq_s16(a);
}

// CHECK-LABEL: @test_vclsq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCLSQ_V1_I:%.*]] = call <4 x i32> @llvm.arm.neon.vcls.v4i32(<4 x i32> %a)
// CHECK:   [[VCLSQ_V2_I:%.*]] = bitcast <4 x i32> [[VCLSQ_V1_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VCLSQ_V1_I]]
int32x4_t test_vclsq_s32(int32x4_t a) {
  return vclsq_s32(a);
}

// CHECK-LABEL: @test_vclsq_u8(
// CHECK:   [[VCLSQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vcls.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VCLSQ_V_I]]
int8x16_t test_vclsq_u8(uint8x16_t a) {
  return vclsq_u8(a);
}

// CHECK-LABEL: @test_vclsq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VCLSQ_V1_I:%.*]] = call <8 x i16> @llvm.arm.neon.vcls.v8i16(<8 x i16> %a)
// CHECK:   [[VCLSQ_V2_I:%.*]] = bitcast <8 x i16> [[VCLSQ_V1_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VCLSQ_V1_I]]
int16x8_t test_vclsq_u16(uint16x8_t a) {
  return vclsq_u16(a);
}

// CHECK-LABEL: @test_vclsq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCLSQ_V1_I:%.*]] = call <4 x i32> @llvm.arm.neon.vcls.v4i32(<4 x i32> %a)
// CHECK:   [[VCLSQ_V2_I:%.*]] = bitcast <4 x i32> [[VCLSQ_V1_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VCLSQ_V1_I]]
int32x4_t test_vclsq_u32(uint32x4_t a) {
  return vclsq_u32(a);
}

// CHECK-LABEL: @test_vclt_s8(
// CHECK:   [[CMP_I:%.*]] = icmp slt <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vclt_s8(int8x8_t a, int8x8_t b) {
  return vclt_s8(a, b);
}

// CHECK-LABEL: @test_vclt_s16(
// CHECK:   [[CMP_I:%.*]] = icmp slt <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vclt_s16(int16x4_t a, int16x4_t b) {
  return vclt_s16(a, b);
}

// CHECK-LABEL: @test_vclt_s32(
// CHECK:   [[CMP_I:%.*]] = icmp slt <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_s32(int32x2_t a, int32x2_t b) {
  return vclt_s32(a, b);
}

// CHECK-LABEL: @test_vclt_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <2 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_f32(float32x2_t a, float32x2_t b) {
  return vclt_f32(a, b);
}

// CHECK-LABEL: @test_vclt_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ult <8 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vclt_u8(uint8x8_t a, uint8x8_t b) {
  return vclt_u8(a, b);
}

// CHECK-LABEL: @test_vclt_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ult <4 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vclt_u16(uint16x4_t a, uint16x4_t b) {
  return vclt_u16(a, b);
}

// CHECK-LABEL: @test_vclt_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ult <2 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_u32(uint32x2_t a, uint32x2_t b) {
  return vclt_u32(a, b);
}

// CHECK-LABEL: @test_vcltq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp slt <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcltq_s8(int8x16_t a, int8x16_t b) {
  return vcltq_s8(a, b);
}

// CHECK-LABEL: @test_vcltq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp slt <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcltq_s16(int16x8_t a, int16x8_t b) {
  return vcltq_s16(a, b);
}

// CHECK-LABEL: @test_vcltq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp slt <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_s32(int32x4_t a, int32x4_t b) {
  return vcltq_s32(a, b);
}

// CHECK-LABEL: @test_vcltq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <4 x float> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_f32(float32x4_t a, float32x4_t b) {
  return vcltq_f32(a, b);
}

// CHECK-LABEL: @test_vcltq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ult <16 x i8> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcltq_u8(uint8x16_t a, uint8x16_t b) {
  return vcltq_u8(a, b);
}

// CHECK-LABEL: @test_vcltq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ult <8 x i16> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcltq_u16(uint16x8_t a, uint16x8_t b) {
  return vcltq_u16(a, b);
}

// CHECK-LABEL: @test_vcltq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ult <4 x i32> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_u32(uint32x4_t a, uint32x4_t b) {
  return vcltq_u32(a, b);
}

// CHECK-LABEL: @test_vclz_s8(
// CHECK:   [[VCLZ_V_I:%.*]] = call <8 x i8> @llvm.ctlz.v8i8(<8 x i8> %a, i1 false)
// CHECK:   ret <8 x i8> [[VCLZ_V_I]]
int8x8_t test_vclz_s8(int8x8_t a) {
  return vclz_s8(a);
}

// CHECK-LABEL: @test_vclz_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <4 x i16> @llvm.ctlz.v4i16(<4 x i16> %a, i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <4 x i16> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLZ_V1_I]]
int16x4_t test_vclz_s16(int16x4_t a) {
  return vclz_s16(a);
}

// CHECK-LABEL: @test_vclz_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <2 x i32> @llvm.ctlz.v2i32(<2 x i32> %a, i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <2 x i32> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLZ_V1_I]]
int32x2_t test_vclz_s32(int32x2_t a) {
  return vclz_s32(a);
}

// CHECK-LABEL: @test_vclz_u8(
// CHECK:   [[VCLZ_V_I:%.*]] = call <8 x i8> @llvm.ctlz.v8i8(<8 x i8> %a, i1 false)
// CHECK:   ret <8 x i8> [[VCLZ_V_I]]
uint8x8_t test_vclz_u8(uint8x8_t a) {
  return vclz_u8(a);
}

// CHECK-LABEL: @test_vclz_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <4 x i16> @llvm.ctlz.v4i16(<4 x i16> %a, i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <4 x i16> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLZ_V1_I]]
uint16x4_t test_vclz_u16(uint16x4_t a) {
  return vclz_u16(a);
}

// CHECK-LABEL: @test_vclz_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <2 x i32> @llvm.ctlz.v2i32(<2 x i32> %a, i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <2 x i32> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLZ_V1_I]]
uint32x2_t test_vclz_u32(uint32x2_t a) {
  return vclz_u32(a);
}

// CHECK-LABEL: @test_vclzq_s8(
// CHECK:   [[VCLZQ_V_I:%.*]] = call <16 x i8> @llvm.ctlz.v16i8(<16 x i8> %a, i1 false)
// CHECK:   ret <16 x i8> [[VCLZQ_V_I]]
int8x16_t test_vclzq_s8(int8x16_t a) {
  return vclzq_s8(a);
}

// CHECK-LABEL: @test_vclzq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VCLZQ_V1_I:%.*]] = call <8 x i16> @llvm.ctlz.v8i16(<8 x i16> %a, i1 false)
// CHECK:   [[VCLZQ_V2_I:%.*]] = bitcast <8 x i16> [[VCLZQ_V1_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VCLZQ_V1_I]]
int16x8_t test_vclzq_s16(int16x8_t a) {
  return vclzq_s16(a);
}

// CHECK-LABEL: @test_vclzq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCLZQ_V1_I:%.*]] = call <4 x i32> @llvm.ctlz.v4i32(<4 x i32> %a, i1 false)
// CHECK:   [[VCLZQ_V2_I:%.*]] = bitcast <4 x i32> [[VCLZQ_V1_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VCLZQ_V1_I]]
int32x4_t test_vclzq_s32(int32x4_t a) {
  return vclzq_s32(a);
}

// CHECK-LABEL: @test_vclzq_u8(
// CHECK:   [[VCLZQ_V_I:%.*]] = call <16 x i8> @llvm.ctlz.v16i8(<16 x i8> %a, i1 false)
// CHECK:   ret <16 x i8> [[VCLZQ_V_I]]
uint8x16_t test_vclzq_u8(uint8x16_t a) {
  return vclzq_u8(a);
}

// CHECK-LABEL: @test_vclzq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VCLZQ_V1_I:%.*]] = call <8 x i16> @llvm.ctlz.v8i16(<8 x i16> %a, i1 false)
// CHECK:   [[VCLZQ_V2_I:%.*]] = bitcast <8 x i16> [[VCLZQ_V1_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VCLZQ_V1_I]]
uint16x8_t test_vclzq_u16(uint16x8_t a) {
  return vclzq_u16(a);
}

// CHECK-LABEL: @test_vclzq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCLZQ_V1_I:%.*]] = call <4 x i32> @llvm.ctlz.v4i32(<4 x i32> %a, i1 false)
// CHECK:   [[VCLZQ_V2_I:%.*]] = bitcast <4 x i32> [[VCLZQ_V1_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VCLZQ_V1_I]]
uint32x4_t test_vclzq_u32(uint32x4_t a) {
  return vclzq_u32(a);
}

// CHECK-LABEL: @test_vcnt_u8(
// CHECK:   [[VCNT_V_I:%.*]] = call <8 x i8> @llvm.ctpop.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VCNT_V_I]]
uint8x8_t test_vcnt_u8(uint8x8_t a) {
  return vcnt_u8(a);
}

// CHECK-LABEL: @test_vcnt_s8(
// CHECK:   [[VCNT_V_I:%.*]] = call <8 x i8> @llvm.ctpop.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VCNT_V_I]]
int8x8_t test_vcnt_s8(int8x8_t a) {
  return vcnt_s8(a);
}

// CHECK-LABEL: @test_vcnt_p8(
// CHECK:   [[VCNT_V_I:%.*]] = call <8 x i8> @llvm.ctpop.v8i8(<8 x i8> %a)
// CHECK:   ret <8 x i8> [[VCNT_V_I]]
poly8x8_t test_vcnt_p8(poly8x8_t a) {
  return vcnt_p8(a);
}

// CHECK-LABEL: @test_vcntq_u8(
// CHECK:   [[VCNTQ_V_I:%.*]] = call <16 x i8> @llvm.ctpop.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VCNTQ_V_I]]
uint8x16_t test_vcntq_u8(uint8x16_t a) {
  return vcntq_u8(a);
}

// CHECK-LABEL: @test_vcntq_s8(
// CHECK:   [[VCNTQ_V_I:%.*]] = call <16 x i8> @llvm.ctpop.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VCNTQ_V_I]]
int8x16_t test_vcntq_s8(int8x16_t a) {
  return vcntq_s8(a);
}

// CHECK-LABEL: @test_vcntq_p8(
// CHECK:   [[VCNTQ_V_I:%.*]] = call <16 x i8> @llvm.ctpop.v16i8(<16 x i8> %a)
// CHECK:   ret <16 x i8> [[VCNTQ_V_I]]
poly8x16_t test_vcntq_p8(poly8x16_t a) {
  return vcntq_p8(a);
}

// CHECK-LABEL: @test_vcombine_s8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vcombine_s8(int8x8_t a, int8x8_t b) {
  return vcombine_s8(a, b);
}

// CHECK-LABEL: @test_vcombine_s16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> %b, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vcombine_s16(int16x4_t a, int16x4_t b) {
  return vcombine_s16(a, b);
}

// CHECK-LABEL: @test_vcombine_s32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> %b, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vcombine_s32(int32x2_t a, int32x2_t b) {
  return vcombine_s32(a, b);
}

// CHECK-LABEL: @test_vcombine_s64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <1 x i64> %a, <1 x i64> %b, <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i64> [[SHUFFLE_I]]
int64x2_t test_vcombine_s64(int64x1_t a, int64x1_t b) {
  return vcombine_s64(a, b);
}

// CHECK-LABEL: @test_vcombine_f16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x half> %a, <4 x half> %b, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x half> [[SHUFFLE_I]]
float16x8_t test_vcombine_f16(float16x4_t a, float16x4_t b) {
  return vcombine_f16(a, b);
}

// CHECK-LABEL: @test_vcombine_f32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x float> %a, <2 x float> %b, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x float> [[SHUFFLE_I]]
float32x4_t test_vcombine_f32(float32x2_t a, float32x2_t b) {
  return vcombine_f32(a, b);
}

// CHECK-LABEL: @test_vcombine_u8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
uint8x16_t test_vcombine_u8(uint8x8_t a, uint8x8_t b) {
  return vcombine_u8(a, b);
}

// CHECK-LABEL: @test_vcombine_u16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> %b, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
uint16x8_t test_vcombine_u16(uint16x4_t a, uint16x4_t b) {
  return vcombine_u16(a, b);
}

// CHECK-LABEL: @test_vcombine_u32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> %b, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
uint32x4_t test_vcombine_u32(uint32x2_t a, uint32x2_t b) {
  return vcombine_u32(a, b);
}

// CHECK-LABEL: @test_vcombine_u64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <1 x i64> %a, <1 x i64> %b, <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i64> [[SHUFFLE_I]]
uint64x2_t test_vcombine_u64(uint64x1_t a, uint64x1_t b) {
  return vcombine_u64(a, b);
}

// CHECK-LABEL: @test_vcombine_p8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
poly8x16_t test_vcombine_p8(poly8x8_t a, poly8x8_t b) {
  return vcombine_p8(a, b);
}

// CHECK-LABEL: @test_vcombine_p16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> %b, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
poly16x8_t test_vcombine_p16(poly16x4_t a, poly16x4_t b) {
  return vcombine_p16(a, b);
}

// CHECK-LABEL: @test_vcreate_s8(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <8 x i8>
// CHECK:   [[VCLZ_V_I:%.*]] = call <8 x i8> @llvm.ctlz.v8i8(<8 x i8> [[TMP0]], i1 false)
// CHECK:   ret <8 x i8> [[VCLZ_V_I]]
int8x8_t test_vcreate_s8(uint64_t a) {
  return vclz_s8(vcreate_s8(a));
}

// CHECK-LABEL: @test_vcreate_imm
// CHECK: [[RES:%.*]] = bitcast i64 0 to <4 x i16>
// CHECK: ret <4 x i16> [[RES]]
int16x4_t test_vcreate_imm(void) {
  return vcreate_s16(0);
}

// CHECK-LABEL: @test_vcreate_s16(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <4 x i16>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> [[TMP0]] to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <4 x i16> @llvm.ctlz.v4i16(<4 x i16> [[TMP0]], i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <4 x i16> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLZ_V1_I]]
int16x4_t test_vcreate_s16(uint64_t a) {
  return vclz_s16(vcreate_s16(a));
}

// CHECK-LABEL: @test_vcreate_s32(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <2 x i32>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> [[TMP0]] to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <2 x i32> @llvm.ctlz.v2i32(<2 x i32> [[TMP0]], i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <2 x i32> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLZ_V1_I]]
int32x2_t test_vcreate_s32(uint64_t a) {
  return vclz_s32(vcreate_s32(a));
}

// CHECK-LABEL: @test_vcreate_f16(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <4 x half>
// CHECK:   ret <4 x half> [[TMP0]]
float16x4_t test_vcreate_f16(uint64_t a) {
  return vcreate_f16(a);
}

// CHECK-LABEL: @test_vcreate_f32(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <2 x float>
// CHECK:   ret <2 x float> [[TMP0]]
float32x2_t test_vcreate_f32(uint64_t a) {
  return vcreate_f32(a);
}

// CHECK-LABEL: @test_vcreate_u8(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <8 x i8>
// CHECK:   [[VCLZ_V_I:%.*]] = call <8 x i8> @llvm.ctlz.v8i8(<8 x i8> [[TMP0]], i1 false)
// CHECK:   ret <8 x i8> [[VCLZ_V_I]]
int8x8_t test_vcreate_u8(uint64_t a) {
  return vclz_s8((int8x8_t)vcreate_u8(a));
}

// CHECK-LABEL: @test_vcreate_u16(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <4 x i16>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> [[TMP0]] to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <4 x i16> @llvm.ctlz.v4i16(<4 x i16> [[TMP0]], i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <4 x i16> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VCLZ_V1_I]]
int16x4_t test_vcreate_u16(uint64_t a) {
  return vclz_s16((int16x4_t)vcreate_u16(a));
}

// CHECK-LABEL: @test_vcreate_u32(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <2 x i32>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> [[TMP0]] to <8 x i8>
// CHECK:   [[VCLZ_V1_I:%.*]] = call <2 x i32> @llvm.ctlz.v2i32(<2 x i32> [[TMP0]], i1 false)
// CHECK:   [[VCLZ_V2_I:%.*]] = bitcast <2 x i32> [[VCLZ_V1_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VCLZ_V1_I]]
int32x2_t test_vcreate_u32(uint64_t a) {
  return vclz_s32((int32x2_t)vcreate_u32(a));
}

// CHECK-LABEL: @test_vcreate_u64(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <1 x i64>
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> [[TMP0]], [[TMP0]]
// CHECK:   ret <1 x i64> [[ADD_I]]
uint64x1_t test_vcreate_u64(uint64_t a) {
  uint64x1_t tmp = vcreate_u64(a);
  return vadd_u64(tmp, tmp);
}

// CHECK-LABEL: @test_vcreate_p8(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <8 x i8>
// CHECK:   [[VCNT_V_I:%.*]] = call <8 x i8> @llvm.ctpop.v8i8(<8 x i8> [[TMP0]])
// CHECK:   ret <8 x i8> [[VCNT_V_I]]
poly8x8_t test_vcreate_p8(uint64_t a) {
  return vcnt_p8(vcreate_p8(a));
}

// CHECK-LABEL: @test_vcreate_p16(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <4 x i16>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> [[TMP0]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> [[TMP0]] to <8 x i8>
// CHECK:   [[TMP3:%.*]] = bitcast <4 x i16> [[TMP0]] to <8 x i8>
// CHECK:   [[VBSL_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vbsl.v8i8(<8 x i8> [[TMP1]], <8 x i8> [[TMP2]], <8 x i8> [[TMP3]])
// CHECK:   [[TMP4:%.*]] = bitcast <8 x i8> [[VBSL_V_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[TMP4]]
poly16x4_t test_vcreate_p16(uint64_t a) {
  poly16x4_t tmp = vcreate_p16(a);
  return vbsl_p16((uint16x4_t)tmp, tmp, tmp);
}

// CHECK-LABEL: @test_vcreate_s64(
// CHECK:   [[TMP0:%.*]] = bitcast i64 %a to <1 x i64>
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> [[TMP0]], [[TMP0]]
// CHECK:   ret <1 x i64> [[ADD_I]]
int64x1_t test_vcreate_s64(uint64_t a) {
  int64x1_t tmp = vcreate_s64(a);
  return vadd_s64(tmp, tmp);
}

// CHECK-LABEL: @test_vcvt_f16_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VCVT_F16_F321_I:%.*]] = call <4 x i16> @llvm.arm.neon.vcvtfp2hf(<4 x float> %a)
// CHECK:   [[VCVT_F16_F322_I:%.*]] = bitcast <4 x i16> [[VCVT_F16_F321_I]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[VCVT_F16_F322_I]] to <4 x half>
// CHECK:   ret <4 x half> [[TMP1]]
float16x4_t test_vcvt_f16_f32(float32x4_t a) {
  return vcvt_f16_f32(a);
}

// CHECK-LABEL: @test_vcvt_f32_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCVT_I:%.*]] = sitofp <2 x i32> %a to <2 x float>
// CHECK:   ret <2 x float> [[VCVT_I]]
float32x2_t test_vcvt_f32_s32(int32x2_t a) {
  return vcvt_f32_s32(a);
}

// CHECK-LABEL: @test_vcvt_f32_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCVT_I:%.*]] = uitofp <2 x i32> %a to <2 x float>
// CHECK:   ret <2 x float> [[VCVT_I]]
float32x2_t test_vcvt_f32_u32(uint32x2_t a) {
  return vcvt_f32_u32(a);
}

// CHECK-LABEL: @test_vcvtq_f32_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCVT_I:%.*]] = sitofp <4 x i32> %a to <4 x float>
// CHECK:   ret <4 x float> [[VCVT_I]]
float32x4_t test_vcvtq_f32_s32(int32x4_t a) {
  return vcvtq_f32_s32(a);
}

// CHECK-LABEL: @test_vcvtq_f32_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCVT_I:%.*]] = uitofp <4 x i32> %a to <4 x float>
// CHECK:   ret <4 x float> [[VCVT_I]]
float32x4_t test_vcvtq_f32_u32(uint32x4_t a) {
  return vcvtq_f32_u32(a);
}

// CHECK-LABEL: @test_vcvt_f32_f16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x half> %a to <8 x i8>
// CHECK:   [[VCVT_F32_F16_I:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VCVT_F32_F161_I:%.*]] = call <4 x float> @llvm.arm.neon.vcvthf2fp(<4 x i16> [[VCVT_F32_F16_I]])
// CHECK:   [[VCVT_F32_F162_I:%.*]] = bitcast <4 x float> [[VCVT_F32_F161_I]] to <16 x i8>
// CHECK:   ret <4 x float> [[VCVT_F32_F161_I]]
float32x4_t test_vcvt_f32_f16(float16x4_t a) {
  return vcvt_f32_f16(a);
}

// CHECK-LABEL: @test_vcvt_n_f32_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VCVT_N1:%.*]] = call <2 x float> @llvm.arm.neon.vcvtfxs2fp.v2f32.v2i32(<2 x i32> [[VCVT_N]], i32 1)
// CHECK:   ret <2 x float> [[VCVT_N1]]
float32x2_t test_vcvt_n_f32_s32(int32x2_t a) {
  return vcvt_n_f32_s32(a, 1);
}

// CHECK-LABEL: @test_vcvt_n_f32_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VCVT_N1:%.*]] = call <2 x float> @llvm.arm.neon.vcvtfxu2fp.v2f32.v2i32(<2 x i32> [[VCVT_N]], i32 1)
// CHECK:   ret <2 x float> [[VCVT_N1]]
float32x2_t test_vcvt_n_f32_u32(uint32x2_t a) {
  return vcvt_n_f32_u32(a, 1);
}

// CHECK-LABEL: @test_vcvtq_n_f32_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VCVT_N1:%.*]] = call <4 x float> @llvm.arm.neon.vcvtfxs2fp.v4f32.v4i32(<4 x i32> [[VCVT_N]], i32 3)
// CHECK:   ret <4 x float> [[VCVT_N1]]
float32x4_t test_vcvtq_n_f32_s32(int32x4_t a) {
  return vcvtq_n_f32_s32(a, 3);
}

// CHECK-LABEL: @test_vcvtq_n_f32_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VCVT_N1:%.*]] = call <4 x float> @llvm.arm.neon.vcvtfxu2fp.v4f32.v4i32(<4 x i32> [[VCVT_N]], i32 3)
// CHECK:   ret <4 x float> [[VCVT_N1]]
float32x4_t test_vcvtq_n_f32_u32(uint32x4_t a) {
  return vcvtq_n_f32_u32(a, 3);
}

// CHECK-LABEL: @test_vcvt_n_s32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x float>
// CHECK:   [[VCVT_N1:%.*]] = call <2 x i32> @llvm.arm.neon.vcvtfp2fxs.v2i32.v2f32(<2 x float> [[VCVT_N]], i32 1)
// CHECK:   ret <2 x i32> [[VCVT_N1]]
int32x2_t test_vcvt_n_s32_f32(float32x2_t a) {
  return vcvt_n_s32_f32(a, 1);
}

// CHECK-LABEL: @test_vcvtq_n_s32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x float>
// CHECK:   [[VCVT_N1:%.*]] = call <4 x i32> @llvm.arm.neon.vcvtfp2fxs.v4i32.v4f32(<4 x float> [[VCVT_N]], i32 3)
// CHECK:   ret <4 x i32> [[VCVT_N1]]
int32x4_t test_vcvtq_n_s32_f32(float32x4_t a) {
  return vcvtq_n_s32_f32(a, 3);
}

// CHECK-LABEL: @test_vcvt_n_u32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x float>
// CHECK:   [[VCVT_N1:%.*]] = call <2 x i32> @llvm.arm.neon.vcvtfp2fxu.v2i32.v2f32(<2 x float> [[VCVT_N]], i32 1)
// CHECK:   ret <2 x i32> [[VCVT_N1]]
uint32x2_t test_vcvt_n_u32_f32(float32x2_t a) {
  return vcvt_n_u32_f32(a, 1);
}

// CHECK-LABEL: @test_vcvtq_n_u32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VCVT_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x float>
// CHECK:   [[VCVT_N1:%.*]] = call <4 x i32> @llvm.arm.neon.vcvtfp2fxu.v4i32.v4f32(<4 x float> [[VCVT_N]], i32 3)
// CHECK:   ret <4 x i32> [[VCVT_N1]]
uint32x4_t test_vcvtq_n_u32_f32(float32x4_t a) {
  return vcvtq_n_u32_f32(a, 3);
}

// CHECK-LABEL: @test_vcvt_s32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[VCVT_I:%.*]] = fptosi <2 x float> %a to <2 x i32>
// CHECK:   ret <2 x i32> [[VCVT_I]]
int32x2_t test_vcvt_s32_f32(float32x2_t a) {
  return vcvt_s32_f32(a);
}

// CHECK-LABEL: @test_vcvtq_s32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VCVT_I:%.*]] = fptosi <4 x float> %a to <4 x i32>
// CHECK:   ret <4 x i32> [[VCVT_I]]
int32x4_t test_vcvtq_s32_f32(float32x4_t a) {
  return vcvtq_s32_f32(a);
}

// CHECK-LABEL: @test_vcvt_u32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[VCVT_I:%.*]] = fptoui <2 x float> %a to <2 x i32>
// CHECK:   ret <2 x i32> [[VCVT_I]]
uint32x2_t test_vcvt_u32_f32(float32x2_t a) {
  return vcvt_u32_f32(a);
}

// CHECK-LABEL: @test_vcvtq_u32_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[VCVT_I:%.*]] = fptoui <4 x float> %a to <4 x i32>
// CHECK:   ret <4 x i32> [[VCVT_I]]
uint32x4_t test_vcvtq_u32_f32(float32x4_t a) {
  return vcvtq_u32_f32(a);
}

// CHECK-LABEL: @test_vdup_lane_u8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <8 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE]]
uint8x8_t test_vdup_lane_u8(uint8x8_t a) {
  return vdup_lane_u8(a, 7);
}

// CHECK-LABEL: @test_vdup_lane_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <4 x i32> <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i16> [[LANE]]
uint16x4_t test_vdup_lane_u16(uint16x4_t a) {
  return vdup_lane_u16(a, 3);
}

// CHECK-LABEL: @test_vdup_lane_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP1]], <2 x i32> [[TMP1]], <2 x i32> <i32 1, i32 1>
// CHECK:   ret <2 x i32> [[LANE]]
uint32x2_t test_vdup_lane_u32(uint32x2_t a) {
  return vdup_lane_u32(a, 1);
}

// CHECK-LABEL: @test_vdup_lane_s8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <8 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE]]
int8x8_t test_vdup_lane_s8(int8x8_t a) {
  return vdup_lane_s8(a, 7);
}

// CHECK-LABEL: @test_vdup_lane_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <4 x i32> <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i16> [[LANE]]
int16x4_t test_vdup_lane_s16(int16x4_t a) {
  return vdup_lane_s16(a, 3);
}

// CHECK-LABEL: @test_vdup_lane_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP1]], <2 x i32> [[TMP1]], <2 x i32> <i32 1, i32 1>
// CHECK:   ret <2 x i32> [[LANE]]
int32x2_t test_vdup_lane_s32(int32x2_t a) {
  return vdup_lane_s32(a, 1);
}

// CHECK-LABEL: @test_vdup_lane_p8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <8 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE]]
poly8x8_t test_vdup_lane_p8(poly8x8_t a) {
  return vdup_lane_p8(a, 7);
}

// CHECK-LABEL: @test_vdup_lane_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <4 x i32> <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i16> [[LANE]]
poly16x4_t test_vdup_lane_p16(poly16x4_t a) {
  return vdup_lane_p16(a, 3);
}

// CHECK-LABEL: @test_vdup_lane_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x float>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x float> [[TMP1]], <2 x float> [[TMP1]], <2 x i32> <i32 1, i32 1>
// CHECK:   ret <2 x float> [[LANE]]
float32x2_t test_vdup_lane_f32(float32x2_t a) {
  return vdup_lane_f32(a, 1);
}

// CHECK-LABEL: @test_vdupq_lane_u8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <16 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <16 x i8> [[SHUFFLE]]
uint8x16_t test_vdupq_lane_u8(uint8x8_t a) {
  return vdupq_lane_u8(a, 7);
}

// CHECK-LABEL: @test_vdupq_lane_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <8 x i32> <i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <8 x i16> [[LANE]]
uint16x8_t test_vdupq_lane_u16(uint16x4_t a) {
  return vdupq_lane_u16(a, 3);
}

// CHECK-LABEL: @test_vdupq_lane_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP1]], <2 x i32> [[TMP1]], <4 x i32> <i32 1, i32 1, i32 1, i32 1>
// CHECK:   ret <4 x i32> [[LANE]]
uint32x4_t test_vdupq_lane_u32(uint32x2_t a) {
  return vdupq_lane_u32(a, 1);
}

// CHECK-LABEL: @test_vdupq_lane_s8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <16 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <16 x i8> [[SHUFFLE]]
int8x16_t test_vdupq_lane_s8(int8x8_t a) {
  return vdupq_lane_s8(a, 7);
}

// CHECK-LABEL: @test_vdupq_lane_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <8 x i32> <i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <8 x i16> [[LANE]]
int16x8_t test_vdupq_lane_s16(int16x4_t a) {
  return vdupq_lane_s16(a, 3);
}

// CHECK-LABEL: @test_vdupq_lane_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP1]], <2 x i32> [[TMP1]], <4 x i32> <i32 1, i32 1, i32 1, i32 1>
// CHECK:   ret <4 x i32> [[LANE]]
int32x4_t test_vdupq_lane_s32(int32x2_t a) {
  return vdupq_lane_s32(a, 1);
}

// CHECK-LABEL: @test_vdupq_lane_p8(
// CHECK:   [[SHUFFLE:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %a, <16 x i32> <i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7, i32 7>
// CHECK:   ret <16 x i8> [[SHUFFLE]]
poly8x16_t test_vdupq_lane_p8(poly8x8_t a) {
  return vdupq_lane_p8(a, 7);
}

// CHECK-LABEL: @test_vdupq_lane_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP1]], <4 x i16> [[TMP1]], <8 x i32> <i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <8 x i16> [[LANE]]
poly16x8_t test_vdupq_lane_p16(poly16x4_t a) {
  return vdupq_lane_p16(a, 3);
}

// CHECK-LABEL: @test_vdupq_lane_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x float>
// CHECK:   [[LANE:%.*]] = shufflevector <2 x float> [[TMP1]], <2 x float> [[TMP1]], <4 x i32> <i32 1, i32 1, i32 1, i32 1>
// CHECK:   ret <4 x float> [[LANE]]
float32x4_t test_vdupq_lane_f32(float32x2_t a) {
  return vdupq_lane_f32(a, 1);
}

// CHECK-LABEL: @test_vdup_lane_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[LANE]]
int64x1_t test_vdup_lane_s64(int64x1_t a) {
  return vdup_lane_s64(a, 0);
}

// CHECK-LABEL: @test_vdup_lane_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[LANE]]
uint64x1_t test_vdup_lane_u64(uint64x1_t a) {
  return vdup_lane_u64(a, 0);
}

// CHECK-LABEL: @test_vdupq_lane_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i64> [[LANE]]
int64x2_t test_vdupq_lane_s64(int64x1_t a) {
  return vdupq_lane_s64(a, 0);
}

// CHECK-LABEL: @test_vdupq_lane_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> [[A:%.*]] to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP1]], <1 x i64> [[TMP1]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i64> [[LANE]]
uint64x2_t test_vdupq_lane_u64(uint64x1_t a) {
  return vdupq_lane_u64(a, 0);
}

// CHECK-LABEL: @test_vdup_n_u8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   ret <8 x i8> [[VECINIT7_I]]
uint8x8_t test_vdup_n_u8(uint8_t a) {
  return vdup_n_u8(a);
}

// CHECK-LABEL: @test_vdup_n_u16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   ret <4 x i16> [[VECINIT3_I]]
uint16x4_t test_vdup_n_u16(uint16_t a) {
  return vdup_n_u16(a);
}

// CHECK-LABEL: @test_vdup_n_u32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <2 x i32> undef, i32 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <2 x i32> [[VECINIT_I]], i32 %a, i32 1
// CHECK:   ret <2 x i32> [[VECINIT1_I]]
uint32x2_t test_vdup_n_u32(uint32_t a) {
  return vdup_n_u32(a);
}

// CHECK-LABEL: @test_vdup_n_s8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   ret <8 x i8> [[VECINIT7_I]]
int8x8_t test_vdup_n_s8(int8_t a) {
  return vdup_n_s8(a);
}

// CHECK-LABEL: @test_vdup_n_s16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   ret <4 x i16> [[VECINIT3_I]]
int16x4_t test_vdup_n_s16(int16_t a) {
  return vdup_n_s16(a);
}

// CHECK-LABEL: @test_vdup_n_s32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <2 x i32> undef, i32 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <2 x i32> [[VECINIT_I]], i32 %a, i32 1
// CHECK:   ret <2 x i32> [[VECINIT1_I]]
int32x2_t test_vdup_n_s32(int32_t a) {
  return vdup_n_s32(a);
}

// CHECK-LABEL: @test_vdup_n_p8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   ret <8 x i8> [[VECINIT7_I]]
poly8x8_t test_vdup_n_p8(poly8_t a) {
  return vdup_n_p8(a);
}

// CHECK-LABEL: @test_vdup_n_p16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   ret <4 x i16> [[VECINIT3_I]]
poly16x4_t test_vdup_n_p16(poly16_t a) {
  return vdup_n_p16(a);
}

// CHECK-LABEL: @test_vdup_n_f16(
// CHECK:   [[TMP0:%.*]] = load half, ptr %a, align 2
// CHECK:   [[VECINIT:%.*]] = insertelement <4 x half> undef, half [[TMP0]], i32 0
// CHECK:   [[VECINIT1:%.*]] = insertelement <4 x half> [[VECINIT]], half [[TMP0]], i32 1
// CHECK:   [[VECINIT2:%.*]] = insertelement <4 x half> [[VECINIT1]], half [[TMP0]], i32 2
// CHECK:   [[VECINIT3:%.*]] = insertelement <4 x half> [[VECINIT2]], half [[TMP0]], i32 3
// CHECK:   ret <4 x half> [[VECINIT3]]
float16x4_t test_vdup_n_f16(float16_t *a) {
  return vdup_n_f16(*a);
}

// CHECK-LABEL: @test_vdup_n_f32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <2 x float> undef, float %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <2 x float> [[VECINIT_I]], float %a, i32 1
// CHECK:   ret <2 x float> [[VECINIT1_I]]
float32x2_t test_vdup_n_f32(float32_t a) {
  return vdup_n_f32(a);
}

// CHECK-LABEL: @test_vdupq_n_u8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <16 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <16 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <16 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <16 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <16 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <16 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <16 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <16 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   [[VECINIT8_I:%.*]] = insertelement <16 x i8> [[VECINIT7_I]], i8 %a, i32 8
// CHECK:   [[VECINIT9_I:%.*]] = insertelement <16 x i8> [[VECINIT8_I]], i8 %a, i32 9
// CHECK:   [[VECINIT10_I:%.*]] = insertelement <16 x i8> [[VECINIT9_I]], i8 %a, i32 10
// CHECK:   [[VECINIT11_I:%.*]] = insertelement <16 x i8> [[VECINIT10_I]], i8 %a, i32 11
// CHECK:   [[VECINIT12_I:%.*]] = insertelement <16 x i8> [[VECINIT11_I]], i8 %a, i32 12
// CHECK:   [[VECINIT13_I:%.*]] = insertelement <16 x i8> [[VECINIT12_I]], i8 %a, i32 13
// CHECK:   [[VECINIT14_I:%.*]] = insertelement <16 x i8> [[VECINIT13_I]], i8 %a, i32 14
// CHECK:   [[VECINIT15_I:%.*]] = insertelement <16 x i8> [[VECINIT14_I]], i8 %a, i32 15
// CHECK:   ret <16 x i8> [[VECINIT15_I]]
uint8x16_t test_vdupq_n_u8(uint8_t a) {
  return vdupq_n_u8(a);
}

// CHECK-LABEL: @test_vdupq_n_u16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i16> [[VECINIT3_I]], i16 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i16> [[VECINIT4_I]], i16 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i16> [[VECINIT5_I]], i16 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i16> [[VECINIT6_I]], i16 %a, i32 7
// CHECK:   ret <8 x i16> [[VECINIT7_I]]
uint16x8_t test_vdupq_n_u16(uint16_t a) {
  return vdupq_n_u16(a);
}

// CHECK-LABEL: @test_vdupq_n_u32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x i32> undef, i32 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x i32> [[VECINIT_I]], i32 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x i32> [[VECINIT1_I]], i32 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x i32> [[VECINIT2_I]], i32 %a, i32 3
// CHECK:   ret <4 x i32> [[VECINIT3_I]]
uint32x4_t test_vdupq_n_u32(uint32_t a) {
  return vdupq_n_u32(a);
}

// CHECK-LABEL: @test_vdupq_n_s8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <16 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <16 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <16 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <16 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <16 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <16 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <16 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <16 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   [[VECINIT8_I:%.*]] = insertelement <16 x i8> [[VECINIT7_I]], i8 %a, i32 8
// CHECK:   [[VECINIT9_I:%.*]] = insertelement <16 x i8> [[VECINIT8_I]], i8 %a, i32 9
// CHECK:   [[VECINIT10_I:%.*]] = insertelement <16 x i8> [[VECINIT9_I]], i8 %a, i32 10
// CHECK:   [[VECINIT11_I:%.*]] = insertelement <16 x i8> [[VECINIT10_I]], i8 %a, i32 11
// CHECK:   [[VECINIT12_I:%.*]] = insertelement <16 x i8> [[VECINIT11_I]], i8 %a, i32 12
// CHECK:   [[VECINIT13_I:%.*]] = insertelement <16 x i8> [[VECINIT12_I]], i8 %a, i32 13
// CHECK:   [[VECINIT14_I:%.*]] = insertelement <16 x i8> [[VECINIT13_I]], i8 %a, i32 14
// CHECK:   [[VECINIT15_I:%.*]] = insertelement <16 x i8> [[VECINIT14_I]], i8 %a, i32 15
// CHECK:   ret <16 x i8> [[VECINIT15_I]]
int8x16_t test_vdupq_n_s8(int8_t a) {
  return vdupq_n_s8(a);
}

// CHECK-LABEL: @test_vdupq_n_s16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i16> [[VECINIT3_I]], i16 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i16> [[VECINIT4_I]], i16 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i16> [[VECINIT5_I]], i16 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i16> [[VECINIT6_I]], i16 %a, i32 7
// CHECK:   ret <8 x i16> [[VECINIT7_I]]
int16x8_t test_vdupq_n_s16(int16_t a) {
  return vdupq_n_s16(a);
}

// CHECK-LABEL: @test_vdupq_n_s32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x i32> undef, i32 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x i32> [[VECINIT_I]], i32 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x i32> [[VECINIT1_I]], i32 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x i32> [[VECINIT2_I]], i32 %a, i32 3
// CHECK:   ret <4 x i32> [[VECINIT3_I]]
int32x4_t test_vdupq_n_s32(int32_t a) {
  return vdupq_n_s32(a);
}

// CHECK-LABEL: @test_vdupq_n_p8(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <16 x i8> undef, i8 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <16 x i8> [[VECINIT_I]], i8 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <16 x i8> [[VECINIT1_I]], i8 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <16 x i8> [[VECINIT2_I]], i8 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <16 x i8> [[VECINIT3_I]], i8 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <16 x i8> [[VECINIT4_I]], i8 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <16 x i8> [[VECINIT5_I]], i8 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <16 x i8> [[VECINIT6_I]], i8 %a, i32 7
// CHECK:   [[VECINIT8_I:%.*]] = insertelement <16 x i8> [[VECINIT7_I]], i8 %a, i32 8
// CHECK:   [[VECINIT9_I:%.*]] = insertelement <16 x i8> [[VECINIT8_I]], i8 %a, i32 9
// CHECK:   [[VECINIT10_I:%.*]] = insertelement <16 x i8> [[VECINIT9_I]], i8 %a, i32 10
// CHECK:   [[VECINIT11_I:%.*]] = insertelement <16 x i8> [[VECINIT10_I]], i8 %a, i32 11
// CHECK:   [[VECINIT12_I:%.*]] = insertelement <16 x i8> [[VECINIT11_I]], i8 %a, i32 12
// CHECK:   [[VECINIT13_I:%.*]] = insertelement <16 x i8> [[VECINIT12_I]], i8 %a, i32 13
// CHECK:   [[VECINIT14_I:%.*]] = insertelement <16 x i8> [[VECINIT13_I]], i8 %a, i32 14
// CHECK:   [[VECINIT15_I:%.*]] = insertelement <16 x i8> [[VECINIT14_I]], i8 %a, i32 15
// CHECK:   ret <16 x i8> [[VECINIT15_I]]
poly8x16_t test_vdupq_n_p8(poly8_t a) {
  return vdupq_n_p8(a);
}

// CHECK-LABEL: @test_vdupq_n_p16(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <8 x i16> undef, i16 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <8 x i16> [[VECINIT_I]], i16 %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <8 x i16> [[VECINIT1_I]], i16 %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <8 x i16> [[VECINIT2_I]], i16 %a, i32 3
// CHECK:   [[VECINIT4_I:%.*]] = insertelement <8 x i16> [[VECINIT3_I]], i16 %a, i32 4
// CHECK:   [[VECINIT5_I:%.*]] = insertelement <8 x i16> [[VECINIT4_I]], i16 %a, i32 5
// CHECK:   [[VECINIT6_I:%.*]] = insertelement <8 x i16> [[VECINIT5_I]], i16 %a, i32 6
// CHECK:   [[VECINIT7_I:%.*]] = insertelement <8 x i16> [[VECINIT6_I]], i16 %a, i32 7
// CHECK:   ret <8 x i16> [[VECINIT7_I]]
poly16x8_t test_vdupq_n_p16(poly16_t a) {
  return vdupq_n_p16(a);
}

// CHECK-LABEL: @test_vdupq_n_f16(
// CHECK:   [[TMP0:%.*]] = load half, ptr %a, align 2
// CHECK:   [[VECINIT:%.*]] = insertelement <8 x half> undef, half [[TMP0]], i32 0
// CHECK:   [[VECINIT1:%.*]] = insertelement <8 x half> [[VECINIT]], half [[TMP0]], i32 1
// CHECK:   [[VECINIT2:%.*]] = insertelement <8 x half> [[VECINIT1]], half [[TMP0]], i32 2
// CHECK:   [[VECINIT3:%.*]] = insertelement <8 x half> [[VECINIT2]], half [[TMP0]], i32 3
// CHECK:   [[VECINIT4:%.*]] = insertelement <8 x half> [[VECINIT3]], half [[TMP0]], i32 4
// CHECK:   [[VECINIT5:%.*]] = insertelement <8 x half> [[VECINIT4]], half [[TMP0]], i32 5
// CHECK:   [[VECINIT6:%.*]] = insertelement <8 x half> [[VECINIT5]], half [[TMP0]], i32 6
// CHECK:   [[VECINIT7:%.*]] = insertelement <8 x half> [[VECINIT6]], half [[TMP0]], i32 7
// CHECK:   ret <8 x half> [[VECINIT7]]
float16x8_t test_vdupq_n_f16(float16_t *a) {
  return vdupq_n_f16(*a);
}

// CHECK-LABEL: @test_vdupq_n_f32(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <4 x float> undef, float %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <4 x float> [[VECINIT_I]], float %a, i32 1
// CHECK:   [[VECINIT2_I:%.*]] = insertelement <4 x float> [[VECINIT1_I]], float %a, i32 2
// CHECK:   [[VECINIT3_I:%.*]] = insertelement <4 x float> [[VECINIT2_I]], float %a, i32 3
// CHECK:   ret <4 x float> [[VECINIT3_I]]
float32x4_t test_vdupq_n_f32(float32_t a) {
  return vdupq_n_f32(a);
}

// CHECK-LABEL: @test_vdup_n_s64(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <1 x i64> undef, i64 %a, i32 0
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> [[VECINIT_I]], [[VECINIT_I]]
// CHECK:   ret <1 x i64> [[ADD_I]]
int64x1_t test_vdup_n_s64(int64_t a) {
  int64x1_t tmp = vdup_n_s64(a);
  return vadd_s64(tmp, tmp);
}

// CHECK-LABEL: @test_vdup_n_u64(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <1 x i64> undef, i64 %a, i32 0
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> [[VECINIT_I]], [[VECINIT_I]]
// CHECK:   ret <1 x i64> [[ADD_I]]
int64x1_t test_vdup_n_u64(uint64_t a) {
  int64x1_t tmp = (int64x1_t)vdup_n_u64(a);
  return vadd_s64(tmp, tmp);
}

// CHECK-LABEL: @test_vdupq_n_s64(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <2 x i64> undef, i64 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <2 x i64> [[VECINIT_I]], i64 %a, i32 1
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> [[VECINIT1_I]], [[VECINIT1_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vdupq_n_s64(int64_t a) {
  int64x2_t tmp = vdupq_n_s64(a);
  return vaddq_s64(tmp, tmp);
}

// CHECK-LABEL: @test_vdupq_n_u64(
// CHECK:   [[VECINIT_I:%.*]] = insertelement <2 x i64> undef, i64 %a, i32 0
// CHECK:   [[VECINIT1_I:%.*]] = insertelement <2 x i64> [[VECINIT_I]], i64 %a, i32 1
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> [[VECINIT1_I]], [[VECINIT1_I]]
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vdupq_n_u64(uint64_t a) {
  uint64x2_t tmp = vdupq_n_u64(a);
  return vaddq_u64(tmp, tmp);
}

// CHECK-LABEL: @test_veor_s8(
// CHECK:   [[XOR_I:%.*]] = xor <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[XOR_I]]
int8x8_t test_veor_s8(int8x8_t a, int8x8_t b) {
  return veor_s8(a, b);
}

// CHECK-LABEL: @test_veor_s16(
// CHECK:   [[XOR_I:%.*]] = xor <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[XOR_I]]
int16x4_t test_veor_s16(int16x4_t a, int16x4_t b) {
  return veor_s16(a, b);
}

// CHECK-LABEL: @test_veor_s32(
// CHECK:   [[XOR_I:%.*]] = xor <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[XOR_I]]
int32x2_t test_veor_s32(int32x2_t a, int32x2_t b) {
  return veor_s32(a, b);
}

// CHECK-LABEL: @test_veor_s64(
// CHECK:   [[XOR_I:%.*]] = xor <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[XOR_I]]
int64x1_t test_veor_s64(int64x1_t a, int64x1_t b) {
  return veor_s64(a, b);
}

// CHECK-LABEL: @test_veor_u8(
// CHECK:   [[XOR_I:%.*]] = xor <8 x i8> %a, %b
// CHECK:   ret <8 x i8> [[XOR_I]]
uint8x8_t test_veor_u8(uint8x8_t a, uint8x8_t b) {
  return veor_u8(a, b);
}

// CHECK-LABEL: @test_veor_u16(
// CHECK:   [[XOR_I:%.*]] = xor <4 x i16> %a, %b
// CHECK:   ret <4 x i16> [[XOR_I]]
uint16x4_t test_veor_u16(uint16x4_t a, uint16x4_t b) {
  return veor_u16(a, b);
}

// CHECK-LABEL: @test_veor_u32(
// CHECK:   [[XOR_I:%.*]] = xor <2 x i32> %a, %b
// CHECK:   ret <2 x i32> [[XOR_I]]
uint32x2_t test_veor_u32(uint32x2_t a, uint32x2_t b) {
  return veor_u32(a, b);
}

// CHECK-LABEL: @test_veor_u64(
// CHECK:   [[XOR_I:%.*]] = xor <1 x i64> %a, %b
// CHECK:   ret <1 x i64> [[XOR_I]]
uint64x1_t test_veor_u64(uint64x1_t a, uint64x1_t b) {
  return veor_u64(a, b);
}

// CHECK-LABEL: @test_veorq_s8(
// CHECK:   [[XOR_I:%.*]] = xor <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[XOR_I]]
int8x16_t test_veorq_s8(int8x16_t a, int8x16_t b) {
  return veorq_s8(a, b);
}

// CHECK-LABEL: @test_veorq_s16(
// CHECK:   [[XOR_I:%.*]] = xor <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[XOR_I]]
int16x8_t test_veorq_s16(int16x8_t a, int16x8_t b) {
  return veorq_s16(a, b);
}

// CHECK-LABEL: @test_veorq_s32(
// CHECK:   [[XOR_I:%.*]] = xor <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[XOR_I]]
int32x4_t test_veorq_s32(int32x4_t a, int32x4_t b) {
  return veorq_s32(a, b);
}

// CHECK-LABEL: @test_veorq_s64(
// CHECK:   [[XOR_I:%.*]] = xor <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[XOR_I]]
int64x2_t test_veorq_s64(int64x2_t a, int64x2_t b) {
  return veorq_s64(a, b);
}

// CHECK-LABEL: @test_veorq_u8(
// CHECK:   [[XOR_I:%.*]] = xor <16 x i8> %a, %b
// CHECK:   ret <16 x i8> [[XOR_I]]
uint8x16_t test_veorq_u8(uint8x16_t a, uint8x16_t b) {
  return veorq_u8(a, b);
}

// CHECK-LABEL: @test_veorq_u16(
// CHECK:   [[XOR_I:%.*]] = xor <8 x i16> %a, %b
// CHECK:   ret <8 x i16> [[XOR_I]]
uint16x8_t test_veorq_u16(uint16x8_t a, uint16x8_t b) {
  return veorq_u16(a, b);
}

// CHECK-LABEL: @test_veorq_u32(
// CHECK:   [[XOR_I:%.*]] = xor <4 x i32> %a, %b
// CHECK:   ret <4 x i32> [[XOR_I]]
uint32x4_t test_veorq_u32(uint32x4_t a, uint32x4_t b) {
  return veorq_u32(a, b);
}

// CHECK-LABEL: @test_veorq_u64(
// CHECK:   [[XOR_I:%.*]] = xor <2 x i64> %a, %b
// CHECK:   ret <2 x i64> [[XOR_I]]
uint64x2_t test_veorq_u64(uint64x2_t a, uint64x2_t b) {
  return veorq_u64(a, b);
}

// CHECK-LABEL: @test_vext_s8(
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i8> [[VEXT]]
int8x8_t test_vext_s8(int8x8_t a, int8x8_t b) {
  return vext_s8(a, b, 7);
}

// CHECK-LABEL: @test_vext_u8(
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i8> [[VEXT]]
uint8x8_t test_vext_u8(uint8x8_t a, uint8x8_t b) {
  return vext_u8(a, b, 7);
}

// CHECK-LABEL: @test_vext_p8(
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i8> %a, <8 x i8> %b, <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i8> [[VEXT]]
poly8x8_t test_vext_p8(poly8x8_t a, poly8x8_t b) {
  return vext_p8(a, b, 7);
}

// CHECK-LABEL: @test_vext_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x i16> [[TMP2]], <4 x i16> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x i16> [[VEXT]]
int16x4_t test_vext_s16(int16x4_t a, int16x4_t b) {
  return vext_s16(a, b, 3);
}

// CHECK-LABEL: @test_vext_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x i16> [[TMP2]], <4 x i16> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x i16> [[VEXT]]
uint16x4_t test_vext_u16(uint16x4_t a, uint16x4_t b) {
  return vext_u16(a, b, 3);
}

// CHECK-LABEL: @test_vext_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x i16> [[TMP2]], <4 x i16> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x i16> [[VEXT]]
poly16x4_t test_vext_p16(poly16x4_t a, poly16x4_t b) {
  return vext_p16(a, b, 3);
}

// CHECK-LABEL: @test_vext_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VEXT:%.*]] = shufflevector <2 x i32> [[TMP2]], <2 x i32> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK:   ret <2 x i32> [[VEXT]]
int32x2_t test_vext_s32(int32x2_t a, int32x2_t b) {
  return vext_s32(a, b, 1);
}

// CHECK-LABEL: @test_vext_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VEXT:%.*]] = shufflevector <2 x i32> [[TMP2]], <2 x i32> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK:   ret <2 x i32> [[VEXT]]
uint32x2_t test_vext_u32(uint32x2_t a, uint32x2_t b) {
  return vext_u32(a, b, 1);
}

// CHECK-LABEL: @test_vext_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[VEXT:%.*]] = shufflevector <1 x i64> [[TMP2]], <1 x i64> [[TMP3]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[VEXT]]
int64x1_t test_vext_s64(int64x1_t a, int64x1_t b) {
  return vext_s64(a, b, 0);
}

// CHECK-LABEL: @test_vext_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[VEXT:%.*]] = shufflevector <1 x i64> [[TMP2]], <1 x i64> [[TMP3]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[VEXT]]
uint64x1_t test_vext_u64(uint64x1_t a, uint64x1_t b) {
  return vext_u64(a, b, 0);
}

// CHECK-LABEL: @test_vext_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x float>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x float>
// CHECK:   [[VEXT:%.*]] = shufflevector <2 x float> [[TMP2]], <2 x float> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK:   ret <2 x float> [[VEXT]]
float32x2_t test_vext_f32(float32x2_t a, float32x2_t b) {
  return vext_f32(a, b, 1);
}

// CHECK-LABEL: @test_vextq_s8(
// CHECK:   [[VEXT:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %b, <16 x i32> <i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30>
// CHECK:   ret <16 x i8> [[VEXT]]
int8x16_t test_vextq_s8(int8x16_t a, int8x16_t b) {
  return vextq_s8(a, b, 15);
}

// CHECK-LABEL: @test_vextq_u8(
// CHECK:   [[VEXT:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %b, <16 x i32> <i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30>
// CHECK:   ret <16 x i8> [[VEXT]]
uint8x16_t test_vextq_u8(uint8x16_t a, uint8x16_t b) {
  return vextq_u8(a, b, 15);
}

// CHECK-LABEL: @test_vextq_p8(
// CHECK:   [[VEXT:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %b, <16 x i32> <i32 15, i32 16, i32 17, i32 18, i32 19, i32 20, i32 21, i32 22, i32 23, i32 24, i32 25, i32 26, i32 27, i32 28, i32 29, i32 30>
// CHECK:   ret <16 x i8> [[VEXT]]
poly8x16_t test_vextq_p8(poly8x16_t a, poly8x16_t b) {
  return vextq_p8(a, b, 15);
}

// CHECK-LABEL: @test_vextq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i16> [[TMP2]], <8 x i16> [[TMP3]], <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i16> [[VEXT]]
int16x8_t test_vextq_s16(int16x8_t a, int16x8_t b) {
  return vextq_s16(a, b, 7);
}

// CHECK-LABEL: @test_vextq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i16> [[TMP2]], <8 x i16> [[TMP3]], <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i16> [[VEXT]]
uint16x8_t test_vextq_u16(uint16x8_t a, uint16x8_t b) {
  return vextq_u16(a, b, 7);
}

// CHECK-LABEL: @test_vextq_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VEXT:%.*]] = shufflevector <8 x i16> [[TMP2]], <8 x i16> [[TMP3]], <8 x i32> <i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14>
// CHECK:   ret <8 x i16> [[VEXT]]
poly16x8_t test_vextq_p16(poly16x8_t a, poly16x8_t b) {
  return vextq_p16(a, b, 7);
}

// CHECK-LABEL: @test_vextq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x i32> [[TMP2]], <4 x i32> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x i32> [[VEXT]]
int32x4_t test_vextq_s32(int32x4_t a, int32x4_t b) {
  return vextq_s32(a, b, 3);
}

// CHECK-LABEL: @test_vextq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x i32> [[TMP2]], <4 x i32> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x i32> [[VEXT]]
uint32x4_t test_vextq_u32(uint32x4_t a, uint32x4_t b) {
  return vextq_u32(a, b, 3);
}

// CHECK-LABEL: @test_vextq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VEXT:%.*]] = shufflevector <2 x i64> [[TMP2]], <2 x i64> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK:   ret <2 x i64> [[VEXT]]
int64x2_t test_vextq_s64(int64x2_t a, int64x2_t b) {
  return vextq_s64(a, b, 1);
}

// CHECK-LABEL: @test_vextq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VEXT:%.*]] = shufflevector <2 x i64> [[TMP2]], <2 x i64> [[TMP3]], <2 x i32> <i32 1, i32 2>
// CHECK:   ret <2 x i64> [[VEXT]]
uint64x2_t test_vextq_u64(uint64x2_t a, uint64x2_t b) {
  return vextq_u64(a, b, 1);
}

// CHECK-LABEL: @test_vextq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x float>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x float>
// CHECK:   [[VEXT:%.*]] = shufflevector <4 x float> [[TMP2]], <4 x float> [[TMP3]], <4 x i32> <i32 3, i32 4, i32 5, i32 6>
// CHECK:   ret <4 x float> [[VEXT]]
float32x4_t test_vextq_f32(float32x4_t a, float32x4_t b) {
  return vextq_f32(a, b, 3);
}

// CHECK-LABEL: @test_vfma_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %c to <8 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x float> @llvm.fma.v2f32(<2 x float> %b, <2 x float> %c, <2 x float> %a)
// CHECK:   ret <2 x float> [[TMP3]]
float32x2_t test_vfma_f32(float32x2_t a, float32x2_t b, float32x2_t c) {
  return vfma_f32(a, b, c);
}

// CHECK-LABEL: @test_vfmaq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %c to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <4 x float> @llvm.fma.v4f32(<4 x float> %b, <4 x float> %c, <4 x float> %a)
// CHECK:   ret <4 x float> [[TMP3]]
float32x4_t test_vfmaq_f32(float32x4_t a, float32x4_t b, float32x4_t c) {
  return vfmaq_f32(a, b, c);
}

// CHECK-LABEL: @test_vfms_f32(
// CHECK:   [[SUB_I:%.*]] = fneg <2 x float> %b
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> [[SUB_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %c to <8 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x float> @llvm.fma.v2f32(<2 x float> [[SUB_I]], <2 x float> %c, <2 x float> %a)
// CHECK:   ret <2 x float> [[TMP3]]
float32x2_t test_vfms_f32(float32x2_t a, float32x2_t b, float32x2_t c) {
  return vfms_f32(a, b, c);
}

// CHECK-LABEL: @test_vfmsq_f32(
// CHECK:   [[SUB_I:%.*]] = fneg <4 x float> %b
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> [[SUB_I]] to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %c to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <4 x float> @llvm.fma.v4f32(<4 x float> [[SUB_I]], <4 x float> %c, <4 x float> %a)
// CHECK:   ret <4 x float> [[TMP3]]
float32x4_t test_vfmsq_f32(float32x4_t a, float32x4_t b, float32x4_t c) {
  return vfmsq_f32(a, b, c);
}

// CHECK-LABEL: @test_vget_high_s8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
int8x8_t test_vget_high_s8(int8x16_t a) {
  return vget_high_s8(a);
}

// CHECK-LABEL: @test_vget_high_s16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
int16x4_t test_vget_high_s16(int16x8_t a) {
  return vget_high_s16(a);
}

// CHECK-LABEL: @test_vget_high_s32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i32> %a, <4 x i32> %a, <2 x i32> <i32 2, i32 3>
// CHECK:   ret <2 x i32> [[SHUFFLE_I]]
int32x2_t test_vget_high_s32(int32x4_t a) {
  return vget_high_s32(a);
}

// CHECK-LABEL: @test_vget_high_s64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> %a, <2 x i64> %a, <1 x i32> <i32 1>
// CHECK:   ret <1 x i64> [[SHUFFLE_I]]
int64x1_t test_vget_high_s64(int64x2_t a) {
  return vget_high_s64(a);
}

// CHECK-LABEL: @test_vget_high_f16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x half> %a, <8 x half> %a, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <4 x half> [[SHUFFLE_I]]
float16x4_t test_vget_high_f16(float16x8_t a) {
  return vget_high_f16(a);
}

// CHECK-LABEL: @test_vget_high_f32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x float> %a, <4 x float> %a, <2 x i32> <i32 2, i32 3>
// CHECK:   ret <2 x float> [[SHUFFLE_I]]
float32x2_t test_vget_high_f32(float32x4_t a) {
  return vget_high_f32(a);
}

// CHECK-LABEL: @test_vget_high_u8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
uint8x8_t test_vget_high_u8(uint8x16_t a) {
  return vget_high_u8(a);
}

// CHECK-LABEL: @test_vget_high_u16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
uint16x4_t test_vget_high_u16(uint16x8_t a) {
  return vget_high_u16(a);
}

// CHECK-LABEL: @test_vget_high_u32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i32> %a, <4 x i32> %a, <2 x i32> <i32 2, i32 3>
// CHECK:   ret <2 x i32> [[SHUFFLE_I]]
uint32x2_t test_vget_high_u32(uint32x4_t a) {
  return vget_high_u32(a);
}

// CHECK-LABEL: @test_vget_high_u64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> %a, <2 x i64> %a, <1 x i32> <i32 1>
// CHECK:   ret <1 x i64> [[SHUFFLE_I]]
uint64x1_t test_vget_high_u64(uint64x2_t a) {
  return vget_high_u64(a);
}

// CHECK-LABEL: @test_vget_high_p8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
poly8x8_t test_vget_high_p8(poly8x16_t a) {
  return vget_high_p8(a);
}

// CHECK-LABEL: @test_vget_high_p16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
poly16x4_t test_vget_high_p16(poly16x8_t a) {
  return vget_high_p16(a);
}

// CHECK-LABEL: @test_vget_lane_u8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i8> %a, i32 7
// CHECK:   ret i8 [[VGET_LANE]]
uint8_t test_vget_lane_u8(uint8x8_t a) {
  return vget_lane_u8(a, 7);
}

// CHECK-LABEL: @test_vget_lane_u16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i16> %a, i32 3
// CHECK:   ret i16 [[VGET_LANE]]
uint16_t test_vget_lane_u16(uint16x4_t a) {
  return vget_lane_u16(a, 3);
}

// CHECK-LABEL: @test_vget_lane_u32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <2 x i32> %a, i32 1
// CHECK:   ret i32 [[VGET_LANE]]
uint32_t test_vget_lane_u32(uint32x2_t a) {
  return vget_lane_u32(a, 1);
}

// CHECK-LABEL: @test_vget_lane_s8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i8> %a, i32 7
// CHECK:   ret i8 [[VGET_LANE]]
int8_t test_vget_lane_s8(int8x8_t a) {
  return vget_lane_s8(a, 7);
}

// CHECK-LABEL: @test_vget_lane_s16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i16> %a, i32 3
// CHECK:   ret i16 [[VGET_LANE]]
int16_t test_vget_lane_s16(int16x4_t a) {
  return vget_lane_s16(a, 3);
}

// CHECK-LABEL: @test_vget_lane_s32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <2 x i32> %a, i32 1
// CHECK:   ret i32 [[VGET_LANE]]
int32_t test_vget_lane_s32(int32x2_t a) {
  return vget_lane_s32(a, 1);
}

// CHECK-LABEL: @test_vget_lane_p8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i8> %a, i32 7
// CHECK:   ret i8 [[VGET_LANE]]
poly8_t test_vget_lane_p8(poly8x8_t a) {
  return vget_lane_p8(a, 7);
}

// CHECK-LABEL: @test_vget_lane_p16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i16> %a, i32 3
// CHECK:   ret i16 [[VGET_LANE]]
poly16_t test_vget_lane_p16(poly16x4_t a) {
  return vget_lane_p16(a, 3);
}

// CHECK-LABEL: @test_vget_lane_f32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <2 x float> %a, i32 1
// CHECK:   ret float [[VGET_LANE]]
float32_t test_vget_lane_f32(float32x2_t a) {
  return vget_lane_f32(a, 1);
}

// CHECK-LABEL: @test_vget_lane_f16(
// CHECK:   [[__REINT_242:%.*]] = alloca <4 x half>, align 8
// CHECK:   [[__REINT1_242:%.*]] = alloca i16, align 2
// CHECK:   store <4 x half> %a, ptr [[__REINT_242]], align 8
// CHECK:   [[TMP1:%.*]] = load <4 x i16>, ptr [[__REINT_242]], align 8
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i16> [[TMP1]], i32 1
// CHECK:   store i16 [[VGET_LANE]], ptr [[__REINT1_242]], align 2
// CHECK:   [[TMP5:%.*]] = load half, ptr [[__REINT1_242]], align 2
// CHECK:   [[CONV:%.*]] = fpext half [[TMP5]] to float
// CHECK:   ret float [[CONV]]
float32_t test_vget_lane_f16(float16x4_t a) {
  return vget_lane_f16(a, 1);
}

// CHECK-LABEL: @test_vgetq_lane_u8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <16 x i8> %a, i32 15
// CHECK:   ret i8 [[VGET_LANE]]
uint8_t test_vgetq_lane_u8(uint8x16_t a) {
  return vgetq_lane_u8(a, 15);
}

// CHECK-LABEL: @test_vgetq_lane_u16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i16> %a, i32 7
// CHECK:   ret i16 [[VGET_LANE]]
uint16_t test_vgetq_lane_u16(uint16x8_t a) {
  return vgetq_lane_u16(a, 7);
}

// CHECK-LABEL: @test_vgetq_lane_u32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i32> %a, i32 3
// CHECK:   ret i32 [[VGET_LANE]]
uint32_t test_vgetq_lane_u32(uint32x4_t a) {
  return vgetq_lane_u32(a, 3);
}

// CHECK-LABEL: @test_vgetq_lane_s8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <16 x i8> %a, i32 15
// CHECK:   ret i8 [[VGET_LANE]]
int8_t test_vgetq_lane_s8(int8x16_t a) {
  return vgetq_lane_s8(a, 15);
}

// CHECK-LABEL: @test_vgetq_lane_s16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i16> %a, i32 7
// CHECK:   ret i16 [[VGET_LANE]]
int16_t test_vgetq_lane_s16(int16x8_t a) {
  return vgetq_lane_s16(a, 7);
}

// CHECK-LABEL: @test_vgetq_lane_s32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x i32> %a, i32 3
// CHECK:   ret i32 [[VGET_LANE]]
int32_t test_vgetq_lane_s32(int32x4_t a) {
  return vgetq_lane_s32(a, 3);
}

// CHECK-LABEL: @test_vgetq_lane_p8(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <16 x i8> %a, i32 15
// CHECK:   ret i8 [[VGET_LANE]]
poly8_t test_vgetq_lane_p8(poly8x16_t a) {
  return vgetq_lane_p8(a, 15);
}

// CHECK-LABEL: @test_vgetq_lane_p16(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i16> %a, i32 7
// CHECK:   ret i16 [[VGET_LANE]]
poly16_t test_vgetq_lane_p16(poly16x8_t a) {
  return vgetq_lane_p16(a, 7);
}

// CHECK-LABEL: @test_vgetq_lane_f32(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <4 x float> %a, i32 3
// CHECK:   ret float [[VGET_LANE]]
float32_t test_vgetq_lane_f32(float32x4_t a) {
  return vgetq_lane_f32(a, 3);
}

// CHECK-LABEL: @test_vgetq_lane_f16(
// CHECK:   [[__REINT_244:%.*]] = alloca <8 x half>, align 16
// CHECK:   [[__REINT1_244:%.*]] = alloca i16, align 2
// CHECK:   store <8 x half> %a, ptr [[__REINT_244]], align 16
// CHECK:   [[TMP1:%.*]] = load <8 x i16>, ptr [[__REINT_244]], align 16
// CHECK:   [[VGET_LANE:%.*]] = extractelement <8 x i16> [[TMP1]], i32 3
// CHECK:   store i16 [[VGET_LANE]], ptr [[__REINT1_244]], align 2
// CHECK:   [[TMP5:%.*]] = load half, ptr [[__REINT1_244]], align 2
// CHECK:   [[CONV:%.*]] = fpext half [[TMP5]] to float
// CHECK:   ret float [[CONV]]
float32_t test_vgetq_lane_f16(float16x8_t a) {
  return vgetq_lane_f16(a, 3);
}

// CHECK-LABEL: @test_vget_lane_s64(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <1 x i64> %a, i32 0
// CHECK:   ret i64 [[VGET_LANE]]
int64_t test_vget_lane_s64(int64x1_t a) {
  return vget_lane_s64(a, 0);
}

// CHECK-LABEL: @test_vget_lane_u64(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <1 x i64> %a, i32 0
// CHECK:   ret i64 [[VGET_LANE]]
uint64_t test_vget_lane_u64(uint64x1_t a) {
  return vget_lane_u64(a, 0);
}

// CHECK-LABEL: @test_vgetq_lane_s64(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <2 x i64> %a, i32 1
// CHECK:   ret i64 [[VGET_LANE]]
int64_t test_vgetq_lane_s64(int64x2_t a) {
  return vgetq_lane_s64(a, 1);
}

// CHECK-LABEL: @test_vgetq_lane_u64(
// CHECK:   [[VGET_LANE:%.*]] = extractelement <2 x i64> %a, i32 1
// CHECK:   ret i64 [[VGET_LANE]]
uint64_t test_vgetq_lane_u64(uint64x2_t a) {
  return vgetq_lane_u64(a, 1);
}

// CHECK-LABEL: @test_vget_low_s8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
int8x8_t test_vget_low_s8(int8x16_t a) {
  return vget_low_s8(a);
}

// CHECK-LABEL: @test_vget_low_s16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
int16x4_t test_vget_low_s16(int16x8_t a) {
  return vget_low_s16(a);
}

// CHECK-LABEL: @test_vget_low_s32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i32> %a, <4 x i32> %a, <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i32> [[SHUFFLE_I]]
int32x2_t test_vget_low_s32(int32x4_t a) {
  return vget_low_s32(a);
}

// CHECK-LABEL: @test_vget_low_s64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> %a, <2 x i64> %a, <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[SHUFFLE_I]]
int64x1_t test_vget_low_s64(int64x2_t a) {
  return vget_low_s64(a);
}

// CHECK-LABEL: @test_vget_low_f16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x half> %a, <8 x half> %a, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x half> [[SHUFFLE_I]]
float16x4_t test_vget_low_f16(float16x8_t a) {
  return vget_low_f16(a);
}

// CHECK-LABEL: @test_vget_low_f32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x float> %a, <4 x float> %a, <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x float> [[SHUFFLE_I]]
float32x2_t test_vget_low_f32(float32x4_t a) {
  return vget_low_f32(a);
}

// CHECK-LABEL: @test_vget_low_u8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
uint8x8_t test_vget_low_u8(uint8x16_t a) {
  return vget_low_u8(a);
}

// CHECK-LABEL: @test_vget_low_u16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
uint16x4_t test_vget_low_u16(uint16x8_t a) {
  return vget_low_u16(a);
}

// CHECK-LABEL: @test_vget_low_u32(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i32> %a, <4 x i32> %a, <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i32> [[SHUFFLE_I]]
uint32x2_t test_vget_low_u32(uint32x4_t a) {
  return vget_low_u32(a);
}

// CHECK-LABEL: @test_vget_low_u64(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i64> %a, <2 x i64> %a, <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[SHUFFLE_I]]
uint64x1_t test_vget_low_u64(uint64x2_t a) {
  return vget_low_u64(a);
}

// CHECK-LABEL: @test_vget_low_p8(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <16 x i8> %a, <16 x i8> %a, <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i8> [[SHUFFLE_I]]
poly8x8_t test_vget_low_p8(poly8x16_t a) {
  return vget_low_p8(a);
}

// CHECK-LABEL: @test_vget_low_p16(
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i16> %a, <8 x i16> %a, <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i16> [[SHUFFLE_I]]
poly16x4_t test_vget_low_p16(poly16x8_t a) {
  return vget_low_p16(a);
}

// CHECK-LABEL: @test_vhadd_s8(
// CHECK:   [[VHADD_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vhadds.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VHADD_V_I]]
int8x8_t test_vhadd_s8(int8x8_t a, int8x8_t b) {
  return vhadd_s8(a, b);
}

// CHECK-LABEL: @test_vhadd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vhadds.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <4 x i16> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHADD_V2_I]]
int16x4_t test_vhadd_s16(int16x4_t a, int16x4_t b) {
  return vhadd_s16(a, b);
}

// CHECK-LABEL: @test_vhadd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vhadds.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <2 x i32> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHADD_V2_I]]
int32x2_t test_vhadd_s32(int32x2_t a, int32x2_t b) {
  return vhadd_s32(a, b);
}

// CHECK-LABEL: @test_vhadd_u8(
// CHECK:   [[VHADD_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vhaddu.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VHADD_V_I]]
uint8x8_t test_vhadd_u8(uint8x8_t a, uint8x8_t b) {
  return vhadd_u8(a, b);
}

// CHECK-LABEL: @test_vhadd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vhaddu.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <4 x i16> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHADD_V2_I]]
uint16x4_t test_vhadd_u16(uint16x4_t a, uint16x4_t b) {
  return vhadd_u16(a, b);
}

// CHECK-LABEL: @test_vhadd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vhaddu.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <2 x i32> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHADD_V2_I]]
uint32x2_t test_vhadd_u32(uint32x2_t a, uint32x2_t b) {
  return vhadd_u32(a, b);
}

// CHECK-LABEL: @test_vhaddq_s8(
// CHECK:   [[VHADDQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vhadds.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VHADDQ_V_I]]
int8x16_t test_vhaddq_s8(int8x16_t a, int8x16_t b) {
  return vhaddq_s8(a, b);
}

// CHECK-LABEL: @test_vhaddq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vhadds.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHADDQ_V2_I]]
int16x8_t test_vhaddq_s16(int16x8_t a, int16x8_t b) {
  return vhaddq_s16(a, b);
}

// CHECK-LABEL: @test_vhaddq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vhadds.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHADDQ_V2_I]]
int32x4_t test_vhaddq_s32(int32x4_t a, int32x4_t b) {
  return vhaddq_s32(a, b);
}

// CHECK-LABEL: @test_vhaddq_u8(
// CHECK:   [[VHADDQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vhaddu.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VHADDQ_V_I]]
uint8x16_t test_vhaddq_u8(uint8x16_t a, uint8x16_t b) {
  return vhaddq_u8(a, b);
}

// CHECK-LABEL: @test_vhaddq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vhaddu.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHADDQ_V2_I]]
uint16x8_t test_vhaddq_u16(uint16x8_t a, uint16x8_t b) {
  return vhaddq_u16(a, b);
}

// CHECK-LABEL: @test_vhaddq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vhaddu.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHADDQ_V2_I]]
uint32x4_t test_vhaddq_u32(uint32x4_t a, uint32x4_t b) {
  return vhaddq_u32(a, b);
}

// CHECK-LABEL: @test_vhsub_s8(
// CHECK:   [[VHSUB_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vhsubs.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VHSUB_V_I]]
int8x8_t test_vhsub_s8(int8x8_t a, int8x8_t b) {
  return vhsub_s8(a, b);
}

// CHECK-LABEL: @test_vhsub_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vhsubs.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <4 x i16> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHSUB_V2_I]]
int16x4_t test_vhsub_s16(int16x4_t a, int16x4_t b) {
  return vhsub_s16(a, b);
}

// CHECK-LABEL: @test_vhsub_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vhsubs.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <2 x i32> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHSUB_V2_I]]
int32x2_t test_vhsub_s32(int32x2_t a, int32x2_t b) {
  return vhsub_s32(a, b);
}

// CHECK-LABEL: @test_vhsub_u8(
// CHECK:   [[VHSUB_V_I:%.*]] = call <8 x i8> @llvm.arm.neon.vhsubu.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VHSUB_V_I]]
uint8x8_t test_vhsub_u8(uint8x8_t a, uint8x8_t b) {
  return vhsub_u8(a, b);
}

// CHECK-LABEL: @test_vhsub_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <4 x i16> @llvm.arm.neon.vhsubu.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <4 x i16> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHSUB_V2_I]]
uint16x4_t test_vhsub_u16(uint16x4_t a, uint16x4_t b) {
  return vhsub_u16(a, b);
}

// CHECK-LABEL: @test_vhsub_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <2 x i32> @llvm.arm.neon.vhsubu.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <2 x i32> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHSUB_V2_I]]
uint32x2_t test_vhsub_u32(uint32x2_t a, uint32x2_t b) {
  return vhsub_u32(a, b);
}

// CHECK-LABEL: @test_vhsubq_s8(
// CHECK:   [[VHSUBQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vhsubs.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VHSUBQ_V_I]]
int8x16_t test_vhsubq_s8(int8x16_t a, int8x16_t b) {
  return vhsubq_s8(a, b);
}

// CHECK-LABEL: @test_vhsubq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vhsubs.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHSUBQ_V2_I]]
int16x8_t test_vhsubq_s16(int16x8_t a, int16x8_t b) {
  return vhsubq_s16(a, b);
}

// CHECK-LABEL: @test_vhsubq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vhsubs.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHSUBQ_V2_I]]
int32x4_t test_vhsubq_s32(int32x4_t a, int32x4_t b) {
  return vhsubq_s32(a, b);
}

// CHECK-LABEL: @test_vhsubq_u8(
// CHECK:   [[VHSUBQ_V_I:%.*]] = call <16 x i8> @llvm.arm.neon.vhsubu.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VHSUBQ_V_I]]
uint8x16_t test_vhsubq_u8(uint8x16_t a, uint8x16_t b) {
  return vhsubq_u8(a, b);
}

// CHECK-LABEL: @test_vhsubq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.arm.neon.vhsubu.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHSUBQ_V2_I]]
uint16x8_t test_vhsubq_u16(uint16x8_t a, uint16x8_t b) {
  return vhsubq_u16(a, b);
}

// CHECK-LABEL: @test_vhsubq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.arm.neon.vhsubu.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHSUBQ_V2_I]]
uint32x4_t test_vhsubq_u32(uint32x4_t a, uint32x4_t b) {
  return vhsubq_u32(a, b);
}

// CHECK-LABEL: @test_vld1q_u8(
// CHECK:   [[VLD1:%.*]] = call <16 x i8> @llvm.arm.neon.vld1.v16i8.p0(ptr %a, i32 1)
// CHECK:   ret <16 x i8> [[VLD1]]
uint8x16_t test_vld1q_u8(uint8_t const * a) {
  return vld1q_u8(a);
}

// CHECK-LABEL: @test_vld1q_u16(
// CHECK:   [[VLD1:%.*]] = call <8 x i16> @llvm.arm.neon.vld1.v8i16.p0(ptr %a, i32 2)
// CHECK:   ret <8 x i16> [[VLD1]]
uint16x8_t test_vld1q_u16(uint16_t const * a) {
  return vld1q_u16(a);
}

// CHECK-LABEL: @test_vld1q_u32(
// CHECK:   [[VLD1:%.*]] = call <4 x i32> @llvm.arm.neon.vld1.v4i32.p0(ptr %a, i32 4)
// CHECK:   ret <4 x i32> [[VLD1]]
uint32x4_t test_vld1q_u32(uint32_t const * a) {
  return vld1q_u32(a);
}

// CHECK-LABEL: @test_vld1q_u64(
// CHECK:   [[VLD1:%.*]] = call <2 x i64> @llvm.arm.neon.vld1.v2i64.p0(ptr %a, i32 4)
// CHECK:   ret <2 x i64> [[VLD1]]
uint64x2_t test_vld1q_u64(uint64_t const * a) {
  return vld1q_u64(a);
}

// CHECK-LABEL: @test_vld1q_s8(
// CHECK:   [[VLD1:%.*]] = call <16 x i8> @llvm.arm.neon.vld1.v16i8.p0(ptr %a, i32 1)
// CHECK:   ret <16 x i8> [[VLD1]]
int8x16_t test_vld1q_s8(int8_t const * a) {
  return vld1q_s8(a);
}

// CHECK-LABEL: @test_vld1q_s16(
// CHECK:   [[VLD1:%.*]] = call <8 x i16> @llvm.arm.neon.vld1.v8i16.p0(ptr %a, i32 2)
// CHECK:   ret <8 x i16> [[VLD1]]
int16x8_t test_vld1q_s16(int16_t const * a) {
  return vld1q_s16(a);
}

// CHECK-LABEL: @test_vld1q_s32(
// CHECK:   [[VLD1:%.*]] = call <4 x i32> @llvm.arm.neon.vld1.v4i32.p0(ptr %a, i32 4)
// CHECK:   ret <4 x i32> [[VLD1]]
int32x4_t test_vld1q_s32(int32_t const * a) {
  return vld1q_s32(a);
}

// CHECK-LABEL: @test_vld1q_s64(
// CHECK:   [[VLD1:%.*]] = call <2 x i64> @llvm.arm.neon.vld1.v2i64.p0(ptr %a, i32 4)
// CHECK:   ret <2 x i64> [[VLD1]]
int64x2_t test_vld1q_s64(int64_t const * a) {
  return vld1q_s64(a);
}

// CHECK-LABEL: @test_vld1q_f16(
// CHECK:   [[VLD1:%.*]] = call <8 x half> @llvm.arm.neon.vld1.v8f16.p0(ptr %a, i32 2)
// CHECK:   ret <8 x half> [[VLD1]]
float16x8_t test_vld1q_f16(float16_t const * a) {
  return vld1q_f16(a);
}

// CHECK-LABEL: @test_vld1q_f32(
// CHECK:   [[VLD1:%.*]] = call <4 x float> @llvm.arm.neon.vld1.v4f32.p0(ptr %a, i32 4)
// CHECK:   ret <4 x float> [[VLD1]]
float32x4_t test_vld1q_f32(float32_t const * a) {
  return vld1q_f32(a);
}

// CHECK-LABEL: @test_vld1q_p8(
// CHECK:   [[VLD1:%.*]] = call <16 x i8> @llvm.arm.neon.vld1.v16i8.p0(ptr %a, i32 1)
// CHECK:   ret <16 x i8> [[VLD1]]
poly8x16_t test_vld1q_p8(poly8_t const * a) {
  return vld1q_p8(a);
}

// CHECK-LABEL: @test_vld1q_p16(
// CHECK:   [[VLD1:%.*]] = call <8 x i16> @llvm.arm.neon.vld1.v8i16.p0(ptr %a, i32 2)
// CHECK:   ret <8 x i16> [[VLD1]]
poly16x8_t test_vld1q_p16(poly16_t const * a) {
  return vld1q_p16(a);
}

// CHECK-LABEL: @test_vld1_u8(
// CHECK:   [[VLD1:%.*]] = call <8 x i8> @llvm.arm.neon.vld1.v8i8.p0(ptr %a, i32 1)
// CHECK:   ret <8 x i8> [[VLD1]]
uint8x8_t test_vld1_u8(uint8_t const * a) {
  return vld1_u8(a);
}

// CHECK-LABEL: @test_vld1_u16(
// CHECK:   [[VLD1:%.*]] = call <4 x i16> @llvm.arm.neon.vld1.v4i16.p0(ptr %a, i32 2)
// CHECK:   ret <4 x i16> [[VLD1]]
uint16x4_t test_vld1_u16(uint16_t const * a) {
  return vld1_u16(a);
}

// CHECK-LABEL: @test_vld1_u32(
// CHECK:   [[VLD1:%.*]] = call <2 x i32> @llvm.arm.neon.vld1.v2i32.p0(ptr %a, i32 4)
// CHECK:   ret <2 x i32> [[VLD1]]
uint32x2_t test_vld1_u32(uint32_t const * a) {
  return vld1_u32(a);
}

// CHECK-LABEL: @test_vld1_u64(
// CHECK:   [[VLD1:%.*]] = call <1 x i64> @llvm.arm.neon.vld1.v1i64.p0(ptr %a, i32 4)
// CHECK:   ret <1 x i64> [[VLD1]]
uint64x1_t test_vld1_u64(uint64_t const * a) {
  return vld1_u64(a);
}

// CHECK-LABEL: @test_vld1_s8(
// CHECK:   [[VLD1:%.*]] = call <8 x i8> @llvm.arm.neon.vld1.v8i8.p0(ptr %a, i32 1)
// CHECK:   ret <8 x i8> [[VLD1]]
int8x8_t test_vld1_s8(int8_t const * a) {
  return vld1_s8(a);
}

// CHECK-LABEL: @test_vld1_s16(
// CHECK:   [[VLD1:%.*]] = call <4 x i16> @llvm.arm.neon.vld1.v4i16.p0(ptr %a, i32 2)
// CHECK:   ret <4 x i16> [[VLD1]]
int16x4_t test_vld1_s16(int16_t const * a) {
  return vld1_s16(a);
}

// CHECK-LABEL: @test_vld1_s32(
// CHECK:   [[VLD1:%.*]] = call <2 x i32> @llvm.arm.neon.vld1.v2i32.p0(ptr %a, i32 4)
// CHECK:   ret <2 x i32> [[VLD1]]
int32x2_t test_vld1_s32(int32_t const * a) {
  return vld1_s32(a);
}

// CHECK-LABEL: @test_vld1_s64(
// CHECK:   [[VLD1:%.*]] = call <1 x i64> @llvm.arm.neon.vld1.v1i64.p0(ptr %a, i32 4)
// CHECK:   ret <1 x i64> [[VLD1]]
int64x1_t test_vld1_s64(int64_t const * a) {
  return vld1_s64(a);
}

// CHECK-LABEL: @test_vld1_f16(
// CHECK:   [[VLD1:%.*]] = call <4 x half> @llvm.arm.neon.vld1.v4f16.p0(ptr %a, i32 2)
// CHECK:   ret <4 x half> [[VLD1]]
float16x4_t test_vld1_f16(float16_t const * a) {
  return vld1_f16(a);
}

// CHECK-LABEL: @test_vld1_f32(
// CHECK:   [[VLD1:%.*]] = call <2 x float> @llvm.arm.neon.vld1.v2f32.p0(ptr %a, i32 4)
// CHECK:   ret <2 x float> [[VLD1]]
float32x2_t test_vld1_f32(float32_t const * a) {
  return vld1_f32(a);
}

// CHECK-LABEL: @test_vld1_p8(
// CHECK:   [[VLD1:%.*]] = call <8 x i8> @llvm.arm.neon.vld1.v8i8.p0(ptr %a, i32 1)
// CHECK:   ret <8 x i8> [[VLD1]]
poly8x8_t test_vld1_p8(poly8_t const * a) {
  return vld1_p8(a);
}

// CHECK-LABEL: @test_vld1_p16(
// CHECK:   [[VLD1:%.*]] = call <4 x i16> @llvm.arm.neon.vld1.v4i16.p0(ptr %a, i32 2)
// CHECK:   ret <4 x i16> [[VLD1]]
poly16x4_t test_vld1_p16(poly16_t const * a) {
  return vld1_p16(a);
}

// CHECK-LABEL: @test_vld1q_dup_u8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <16 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <16 x i8> [[TMP1]], <16 x i8> [[TMP1]], <16 x i32> zeroinitializer
// CHECK:   ret <16 x i8> [[LANE]]
uint8x16_t test_vld1q_dup_u8(uint8_t const * a) {
  return vld1q_dup_u8(a);
}

// CHECK-LABEL: @test_vld1q_dup_u16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <8 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i16> [[TMP3]], <8 x i16> [[TMP3]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i16> [[LANE]]
uint16x8_t test_vld1q_dup_u16(uint16_t const * a) {
  return vld1q_dup_u16(a);
}

// CHECK-LABEL: @test_vld1q_dup_u32(
// CHECK:   [[TMP2:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <4 x i32> poison, i32 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i32> [[TMP3]], <4 x i32> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x i32> [[LANE]]
uint32x4_t test_vld1q_dup_u32(uint32_t const * a) {
  return vld1q_dup_u32(a);
}

// CHECK-LABEL: @test_vld1q_dup_u64(
// CHECK:   [[TMP2:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <2 x i64> poison, i64 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i64> [[TMP3]], <2 x i64> [[TMP3]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i64> [[LANE]]
uint64x2_t test_vld1q_dup_u64(uint64_t const * a) {
  return vld1q_dup_u64(a);
}

// CHECK-LABEL: @test_vld1q_dup_s8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <16 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <16 x i8> [[TMP1]], <16 x i8> [[TMP1]], <16 x i32> zeroinitializer
// CHECK:   ret <16 x i8> [[LANE]]
int8x16_t test_vld1q_dup_s8(int8_t const * a) {
  return vld1q_dup_s8(a);
}

// CHECK-LABEL: @test_vld1q_dup_s16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <8 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i16> [[TMP3]], <8 x i16> [[TMP3]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i16> [[LANE]]
int16x8_t test_vld1q_dup_s16(int16_t const * a) {
  return vld1q_dup_s16(a);
}

// CHECK-LABEL: @test_vld1q_dup_s32(
// CHECK:   [[TMP2:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <4 x i32> poison, i32 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i32> [[TMP3]], <4 x i32> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x i32> [[LANE]]
int32x4_t test_vld1q_dup_s32(int32_t const * a) {
  return vld1q_dup_s32(a);
}

// CHECK-LABEL: @test_vld1q_dup_s64(
// CHECK:   [[TMP2:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <2 x i64> poison, i64 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i64> [[TMP3]], <2 x i64> [[TMP3]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i64> [[LANE]]
int64x2_t test_vld1q_dup_s64(int64_t const * a) {
  return vld1q_dup_s64(a);
}

// CHECK-LABEL: @test_vld1q_dup_f16(
// CHECK:   [[TMP2:%.*]] = load half, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <8 x half> poison, half [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x half> [[TMP3]], <8 x half> [[TMP3]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x half> [[LANE]]
float16x8_t test_vld1q_dup_f16(float16_t const * a) {
  return vld1q_dup_f16(a);
}

// CHECK-LABEL: @test_vld1q_dup_f32(
// CHECK:   [[TMP2:%.*]] = load float, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <4 x float> poison, float [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x float> [[TMP3]], <4 x float> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x float> [[LANE]]
float32x4_t test_vld1q_dup_f32(float32_t const * a) {
  return vld1q_dup_f32(a);
}

// CHECK-LABEL: @test_vld1q_dup_p8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <16 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <16 x i8> [[TMP1]], <16 x i8> [[TMP1]], <16 x i32> zeroinitializer
// CHECK:   ret <16 x i8> [[LANE]]
poly8x16_t test_vld1q_dup_p8(poly8_t const * a) {
  return vld1q_dup_p8(a);
}

// CHECK-LABEL: @test_vld1q_dup_p16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <8 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i16> [[TMP3]], <8 x i16> [[TMP3]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i16> [[LANE]]
poly16x8_t test_vld1q_dup_p16(poly16_t const * a) {
  return vld1q_dup_p16(a);
}

// CHECK-LABEL: @test_vld1_dup_u8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <8 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i8> [[TMP1]], <8 x i8> [[TMP1]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i8> [[LANE]]
uint8x8_t test_vld1_dup_u8(uint8_t const * a) {
  return vld1_dup_u8(a);
}

// CHECK-LABEL: @test_vld1_dup_u16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <4 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP3]], <4 x i16> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x i16> [[LANE]]
uint16x4_t test_vld1_dup_u16(uint16_t const * a) {
  return vld1_dup_u16(a);
}

// CHECK-LABEL: @test_vld1_dup_u32(
// CHECK:   [[TMP2:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <2 x i32> poison, i32 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP3]], <2 x i32> [[TMP3]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i32> [[LANE]]
uint32x2_t test_vld1_dup_u32(uint32_t const * a) {
  return vld1_dup_u32(a);
}

// CHECK-LABEL: @test_vld1_dup_u64(
// CHECK:   [[TMP2:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <1 x i64> poison, i64 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP3]], <1 x i64> [[TMP3]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[LANE]]
uint64x1_t test_vld1_dup_u64(uint64_t const * a) {
  return vld1_dup_u64(a);
}

// CHECK-LABEL: @test_vld1_dup_s8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <8 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i8> [[TMP1]], <8 x i8> [[TMP1]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i8> [[LANE]]
int8x8_t test_vld1_dup_s8(int8_t const * a) {
  return vld1_dup_s8(a);
}

// CHECK-LABEL: @test_vld1_dup_s16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <4 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP3]], <4 x i16> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x i16> [[LANE]]
int16x4_t test_vld1_dup_s16(int16_t const * a) {
  return vld1_dup_s16(a);
}

// CHECK-LABEL: @test_vld1_dup_s32(
// CHECK:   [[TMP2:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <2 x i32> poison, i32 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <2 x i32> [[TMP3]], <2 x i32> [[TMP3]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x i32> [[LANE]]
int32x2_t test_vld1_dup_s32(int32_t const * a) {
  return vld1_dup_s32(a);
}

// CHECK-LABEL: @test_vld1_dup_s64(
// CHECK:   [[TMP2:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <1 x i64> poison, i64 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <1 x i64> [[TMP3]], <1 x i64> [[TMP3]], <1 x i32> zeroinitializer
// CHECK:   ret <1 x i64> [[LANE]]
int64x1_t test_vld1_dup_s64(int64_t const * a) {
  return vld1_dup_s64(a);
}

// CHECK-LABEL: @test_vld1_dup_f16(
// CHECK:   [[TMP2:%.*]] = load half, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <4 x half> poison, half [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x half> [[TMP3]], <4 x half> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x half> [[LANE]]
float16x4_t test_vld1_dup_f16(float16_t const * a) {
  return vld1_dup_f16(a);
}

// CHECK-LABEL: @test_vld1_dup_f32(
// CHECK:   [[TMP2:%.*]] = load float, ptr %a, align 4
// CHECK:   [[TMP3:%.*]] = insertelement <2 x float> poison, float [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <2 x float> [[TMP3]], <2 x float> [[TMP3]], <2 x i32> zeroinitializer
// CHECK:   ret <2 x float> [[LANE]]
float32x2_t test_vld1_dup_f32(float32_t const * a) {
  return vld1_dup_f32(a);
}

// CHECK-LABEL: @test_vld1_dup_p8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[TMP1:%.*]] = insertelement <8 x i8> poison, i8 [[TMP0]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <8 x i8> [[TMP1]], <8 x i8> [[TMP1]], <8 x i32> zeroinitializer
// CHECK:   ret <8 x i8> [[LANE]]
poly8x8_t test_vld1_dup_p8(poly8_t const * a) {
  return vld1_dup_p8(a);
}

// CHECK-LABEL: @test_vld1_dup_p16(
// CHECK:   [[TMP2:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[TMP3:%.*]] = insertelement <4 x i16> poison, i16 [[TMP2]], i32 0
// CHECK:   [[LANE:%.*]] = shufflevector <4 x i16> [[TMP3]], <4 x i16> [[TMP3]], <4 x i32> zeroinitializer
// CHECK:   ret <4 x i16> [[LANE]]
poly16x4_t test_vld1_dup_p16(poly16_t const * a) {
  return vld1_dup_p16(a);
}

// CHECK-LABEL: @test_vld1q_lane_u8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <16 x i8> %b, i8 [[TMP0]], i32 15
// CHECK:   ret <16 x i8> [[VLD1_LANE]]
uint8x16_t test_vld1q_lane_u8(uint8_t const * a, uint8x16_t b) {
  return vld1q_lane_u8(a, b, 15);
}

// CHECK-LABEL: @test_vld1q_lane_u16(
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i16> [[TMP2]], i16 [[TMP4]], i32 7
// CHECK:   ret <8 x i16> [[VLD1_LANE]]
uint16x8_t test_vld1q_lane_u16(uint16_t const * a, uint16x8_t b) {
  return vld1q_lane_u16(a, b, 7);
}

// CHECK-LABEL: @test_vld1q_lane_u32(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[TMP4:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x i32> [[TMP2]], i32 [[TMP4]], i32 3
// CHECK:   ret <4 x i32> [[VLD1_LANE]]
uint32x4_t test_vld1q_lane_u32(uint32_t const * a, uint32x4_t b) {
  return vld1q_lane_u32(a, b, 3);
}

// CHECK-LABEL: @test_vld1q_lane_u64(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = shufflevector <2 x i64> [[TMP2]], <2 x i64> [[TMP2]], <1 x i32> zeroinitializer
// CHECK:   [[TMP4:%.*]] = call <1 x i64> @llvm.arm.neon.vld1.v1i64.p0(ptr %a, i32 4)
// CHECK:   [[VLD1Q_LANE:%.*]] = shufflevector <1 x i64> [[TMP3]], <1 x i64> [[TMP4]], <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i64> [[VLD1Q_LANE]]
uint64x2_t test_vld1q_lane_u64(uint64_t const * a, uint64x2_t b) {
  return vld1q_lane_u64(a, b, 1);
}

// CHECK-LABEL: @test_vld1q_lane_s8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <16 x i8> %b, i8 [[TMP0]], i32 15
// CHECK:   ret <16 x i8> [[VLD1_LANE]]
int8x16_t test_vld1q_lane_s8(int8_t const * a, int8x16_t b) {
  return vld1q_lane_s8(a, b, 15);
}

// CHECK-LABEL: @test_vld1q_lane_s16(
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i16> [[TMP2]], i16 [[TMP4]], i32 7
// CHECK:   ret <8 x i16> [[VLD1_LANE]]
int16x8_t test_vld1q_lane_s16(int16_t const * a, int16x8_t b) {
  return vld1q_lane_s16(a, b, 7);
}

// CHECK-LABEL: @test_vld1q_lane_s32(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[TMP4:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x i32> [[TMP2]], i32 [[TMP4]], i32 3
// CHECK:   ret <4 x i32> [[VLD1_LANE]]
int32x4_t test_vld1q_lane_s32(int32_t const * a, int32x4_t b) {
  return vld1q_lane_s32(a, b, 3);
}

// CHECK-LABEL: @test_vld1q_lane_s64(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = shufflevector <2 x i64> [[TMP2]], <2 x i64> [[TMP2]], <1 x i32> zeroinitializer
// CHECK:   [[TMP4:%.*]] = call <1 x i64> @llvm.arm.neon.vld1.v1i64.p0(ptr %a, i32 4)
// CHECK:   [[VLD1Q_LANE:%.*]] = shufflevector <1 x i64> [[TMP3]], <1 x i64> [[TMP4]], <2 x i32> <i32 0, i32 1>
// CHECK:   ret <2 x i64> [[VLD1Q_LANE]]
int64x2_t test_vld1q_lane_s64(int64_t const * a, int64x2_t b) {
  return vld1q_lane_s64(a, b, 1);
}

// CHECK-LABEL: @test_vld1q_lane_f16(
// CHECK:   [[TMP1:%.*]] = bitcast <8 x half> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x half>
// CHECK:   [[TMP4:%.*]] = load half, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x half> [[TMP2]], half [[TMP4]], i32 7
// CHECK:   ret <8 x half> [[VLD1_LANE]]
float16x8_t test_vld1q_lane_f16(float16_t const * a, float16x8_t b) {
  return vld1q_lane_f16(a, b, 7);
}

// CHECK-LABEL: @test_vld1q_lane_f32(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x float>
// CHECK:   [[TMP4:%.*]] = load float, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x float> [[TMP2]], float [[TMP4]], i32 3
// CHECK:   ret <4 x float> [[VLD1_LANE]]
float32x4_t test_vld1q_lane_f32(float32_t const * a, float32x4_t b) {
  return vld1q_lane_f32(a, b, 3);
}

// CHECK-LABEL: @test_vld1q_lane_p8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <16 x i8> %b, i8 [[TMP0]], i32 15
// CHECK:   ret <16 x i8> [[VLD1_LANE]]
poly8x16_t test_vld1q_lane_p8(poly8_t const * a, poly8x16_t b) {
  return vld1q_lane_p8(a, b, 15);
}

// CHECK-LABEL: @test_vld1q_lane_p16(
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i16> [[TMP2]], i16 [[TMP4]], i32 7
// CHECK:   ret <8 x i16> [[VLD1_LANE]]
poly16x8_t test_vld1q_lane_p16(poly16_t const * a, poly16x8_t b) {
  return vld1q_lane_p16(a, b, 7);
}

// CHECK-LABEL: @test_vld1_lane_u8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i8> %b, i8 [[TMP0]], i32 7
// CHECK:   ret <8 x i8> [[VLD1_LANE]]
uint8x8_t test_vld1_lane_u8(uint8_t const * a, uint8x8_t b) {
  return vld1_lane_u8(a, b, 7);
}

// CHECK-LABEL: @test_vld1_lane_u16(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x i16> [[TMP2]], i16 [[TMP4]], i32 3
// CHECK:   ret <4 x i16> [[VLD1_LANE]]
uint16x4_t test_vld1_lane_u16(uint16_t const * a, uint16x4_t b) {
  return vld1_lane_u16(a, b, 3);
}

// CHECK-LABEL: @test_vld1_lane_u32(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[TMP4:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <2 x i32> [[TMP2]], i32 [[TMP4]], i32 1
// CHECK:   ret <2 x i32> [[VLD1_LANE]]
uint32x2_t test_vld1_lane_u32(uint32_t const * a, uint32x2_t b) {
  return vld1_lane_u32(a, b, 1);
}

// CHECK-LABEL: @test_vld1_lane_u64(
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[TMP4:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <1 x i64> [[TMP2]], i64 [[TMP4]], i32 0
// CHECK:   ret <1 x i64> [[VLD1_LANE]]
uint64x1_t test_vld1_lane_u64(uint64_t const * a, uint64x1_t b) {
  return vld1_lane_u64(a, b, 0);
}

// CHECK-LABEL: @test_vld1_lane_s8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i8> %b, i8 [[TMP0]], i32 7
// CHECK:   ret <8 x i8> [[VLD1_LANE]]
int8x8_t test_vld1_lane_s8(int8_t const * a, int8x8_t b) {
  return vld1_lane_s8(a, b, 7);
}

// CHECK-LABEL: @test_vld1_lane_s16(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x i16> [[TMP2]], i16 [[TMP4]], i32 3
// CHECK:   ret <4 x i16> [[VLD1_LANE]]
int16x4_t test_vld1_lane_s16(int16_t const * a, int16x4_t b) {
  return vld1_lane_s16(a, b, 3);
}

// CHECK-LABEL: @test_vld1_lane_s32(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[TMP4:%.*]] = load i32, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <2 x i32> [[TMP2]], i32 [[TMP4]], i32 1
// CHECK:   ret <2 x i32> [[VLD1_LANE]]
int32x2_t test_vld1_lane_s32(int32_t const * a, int32x2_t b) {
  return vld1_lane_s32(a, b, 1);
}

// CHECK-LABEL: @test_vld1_lane_s64(
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[TMP4:%.*]] = load i64, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <1 x i64> [[TMP2]], i64 [[TMP4]], i32 0
// CHECK:   ret <1 x i64> [[VLD1_LANE]]
int64x1_t test_vld1_lane_s64(int64_t const * a, int64x1_t b) {
  return vld1_lane_s64(a, b, 0);
}

// CHECK-LABEL: @test_vld1_lane_f16(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x half> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x half>
// CHECK:   [[TMP4:%.*]] = load half, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x half> [[TMP2]], half [[TMP4]], i32 3
// CHECK:   ret <4 x half> [[VLD1_LANE]]
float16x4_t test_vld1_lane_f16(float16_t const * a, float16x4_t b) {
  return vld1_lane_f16(a, b, 3);
}

// CHECK-LABEL: @test_vld1_lane_f32(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x float>
// CHECK:   [[TMP4:%.*]] = load float, ptr %a, align 4
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <2 x float> [[TMP2]], float [[TMP4]], i32 1
// CHECK:   ret <2 x float> [[VLD1_LANE]]
float32x2_t test_vld1_lane_f32(float32_t const * a, float32x2_t b) {
  return vld1_lane_f32(a, b, 1);
}

// CHECK-LABEL: @test_vld1_lane_p8(
// CHECK:   [[TMP0:%.*]] = load i8, ptr %a, align 1
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <8 x i8> %b, i8 [[TMP0]], i32 7
// CHECK:   ret <8 x i8> [[VLD1_LANE]]
poly8x8_t test_vld1_lane_p8(poly8_t const * a, poly8x8_t b) {
  return vld1_lane_p8(a, b, 7);
}

// CHECK-LABEL: @test_vld1_lane_p16(
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[TMP4:%.*]] = load i16, ptr %a, align 2
// CHECK:   [[VLD1_LANE:%.*]] = insertelement <4 x i16> [[TMP2]], i16 [[TMP4]], i32 3
// CHECK:   ret <4 x i16> [[VLD1_LANE]]
poly16x4_t test_vld1_lane_p16(poly16_t const * a, poly16x4_t b) {
  return vld1_lane_p16(a, b, 3);
}

// CHECK-LABEL: @test_vld2q_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x16x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <16 x i8>, <16 x i8>
uint8x16x2_t test_vld2q_u8(uint8_t const * a) {
  return vld2q_u8(a);
}

// CHECK-LABEL: @test_vld2q_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <8 x i16>, <8 x i16>
uint16x8x2_t test_vld2q_u16(uint16_t const * a) {
  return vld2q_u16(a);
}

// CHECK-LABEL: @test_vld2q_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <4 x i32>, <4 x i32>
uint32x4x2_t test_vld2q_u32(uint32_t const * a) {
  return vld2q_u32(a);
}

// CHECK-LABEL: @test_vld2q_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x16x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <16 x i8>, <16 x i8>
int8x16x2_t test_vld2q_s8(int8_t const * a) {
  return vld2q_s8(a);
}

// CHECK-LABEL: @test_vld2q_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <8 x i16>, <8 x i16>
int16x8x2_t test_vld2q_s16(int16_t const * a) {
  return vld2q_s16(a);
}

// CHECK-LABEL: @test_vld2q_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <4 x i32>, <4 x i32>
int32x4x2_t test_vld2q_s32(int32_t const * a) {
  return vld2q_s32(a);
}

// CHECK-LABEL: @test_vld2q_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x8x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <8 x half>, <8 x half>
float16x8x2_t test_vld2q_f16(float16_t const * a) {
  return vld2q_f16(a);
}

// CHECK-LABEL: @test_vld2q_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x4x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <4 x float>, <4 x float>
float32x4x2_t test_vld2q_f32(float32_t const * a) {
  return vld2q_f32(a);
}

// CHECK-LABEL: @test_vld2q_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x16x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <16 x i8>, <16 x i8>
poly8x16x2_t test_vld2q_p8(poly8_t const * a) {
  return vld2q_p8(a);
}

// CHECK-LABEL: @test_vld2q_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x8x2_t, align 16
// CHECK:   [[VLD2Q_V:%.*]] = call { <8 x i16>, <8 x i16>
poly16x8x2_t test_vld2q_p16(poly16_t const * a) {
  return vld2q_p16(a);
}

// CHECK-LABEL: @test_vld2_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x8x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <8 x i8>, <8 x i8>
uint8x8x2_t test_vld2_u8(uint8_t const * a) {
  return vld2_u8(a);
}

// CHECK-LABEL: @test_vld2_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x4x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <4 x i16>, <4 x i16>
uint16x4x2_t test_vld2_u16(uint16_t const * a) {
  return vld2_u16(a);
}

// CHECK-LABEL: @test_vld2_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x2x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <2 x i32>, <2 x i32>
uint32x2x2_t test_vld2_u32(uint32_t const * a) {
  return vld2_u32(a);
}

// CHECK-LABEL: @test_vld2_u64(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint64x1x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <1 x i64>, <1 x i64>
uint64x1x2_t test_vld2_u64(uint64_t const * a) {
  return vld2_u64(a);
}

// CHECK-LABEL: @test_vld2_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x8x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <8 x i8>, <8 x i8>
int8x8x2_t test_vld2_s8(int8_t const * a) {
  return vld2_s8(a);
}

// CHECK-LABEL: @test_vld2_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x4x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <4 x i16>, <4 x i16>
int16x4x2_t test_vld2_s16(int16_t const * a) {
  return vld2_s16(a);
}

// CHECK-LABEL: @test_vld2_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x2x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <2 x i32>, <2 x i32>
int32x2x2_t test_vld2_s32(int32_t const * a) {
  return vld2_s32(a);
}

// CHECK-LABEL: @test_vld2_s64(
// CHECK:   [[__RET:%.*]] = alloca %struct.int64x1x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <1 x i64>, <1 x i64>
int64x1x2_t test_vld2_s64(int64_t const * a) {
  return vld2_s64(a);
}

// CHECK-LABEL: @test_vld2_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x4x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <4 x half>, <4 x half>
float16x4x2_t test_vld2_f16(float16_t const * a) {
  return vld2_f16(a);
}

// CHECK-LABEL: @test_vld2_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x2x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <2 x float>, <2 x float>
float32x2x2_t test_vld2_f32(float32_t const * a) {
  return vld2_f32(a);
}

// CHECK-LABEL: @test_vld2_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x8x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <8 x i8>, <8 x i8>
poly8x8x2_t test_vld2_p8(poly8_t const * a) {
  return vld2_p8(a);
}

// CHECK-LABEL: @test_vld2_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x4x2_t, align 8
// CHECK:   [[VLD2_V:%.*]] = call { <4 x i16>, <4 x i16>
poly16x4x2_t test_vld2_p16(poly16_t const * a) {
  return vld2_p16(a);
}

// CHECK-LABEL: @test_vld2q_lane_u16(
// CHECK:   [[B:%.*]] = alloca %struct.uint16x8x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint16x8x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint16x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>
uint16x8x2_t test_vld2q_lane_u16(uint16_t const * a, uint16x8x2_t b) {
  return vld2q_lane_u16(a, b, 7);
}

// CHECK-LABEL: @test_vld2q_lane_u32(
// CHECK:   [[B:%.*]] = alloca %struct.uint32x4x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint32x4x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint32x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i32>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i32> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x i32>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x i32>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <4 x i32>, <4 x i32>
uint32x4x2_t test_vld2q_lane_u32(uint32_t const * a, uint32x4x2_t b) {
  return vld2q_lane_u32(a, b, 3);
}

// CHECK-LABEL: @test_vld2q_lane_s16(
// CHECK:   [[B:%.*]] = alloca %struct.int16x8x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int16x8x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int16x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>
int16x8x2_t test_vld2q_lane_s16(int16_t const * a, int16x8x2_t b) {
  return vld2q_lane_s16(a, b, 7);
}

// CHECK-LABEL: @test_vld2q_lane_s32(
// CHECK:   [[B:%.*]] = alloca %struct.int32x4x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int32x4x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int32x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i32>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i32> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x i32>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x i32>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <4 x i32>, <4 x i32>
int32x4x2_t test_vld2q_lane_s32(int32_t const * a, int32x4x2_t b) {
  return vld2q_lane_s32(a, b, 3);
}

// CHECK-LABEL: @test_vld2q_lane_f16(
// CHECK:   [[B:%.*]] = alloca %struct.float16x8x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x8x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.float16x8x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float16x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x half>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x half>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x half> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x half>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x half>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x half> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x half>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x half>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <8 x half>, <8 x half>
float16x8x2_t test_vld2q_lane_f16(float16_t const * a, float16x8x2_t b) {
  return vld2q_lane_f16(a, b, 7);
}

// CHECK-LABEL: @test_vld2q_lane_f32(
// CHECK:   [[B:%.*]] = alloca %struct.float32x4x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x4x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.float32x4x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float32x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x float>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x float>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x float> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float32x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x float>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x float>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x float> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x float>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x float>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <4 x float>, <4 x float>
float32x4x2_t test_vld2q_lane_f32(float32_t const * a, float32x4x2_t b) {
  return vld2q_lane_f32(a, b, 3);
}

// CHECK-LABEL: @test_vld2q_lane_p16(
// CHECK:   [[B:%.*]] = alloca %struct.poly16x8x2_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x8x2_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.poly16x8x2_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly16x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [4 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 32, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly16x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[VLD2Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>
poly16x8x2_t test_vld2q_lane_p16(poly16_t const * a, poly16x8x2_t b) {
  return vld2q_lane_p16(a, b, 7);
}

// CHECK-LABEL: @test_vld2_lane_u8(
// CHECK:   [[B:%.*]] = alloca %struct.uint8x8x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x8x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint8x8x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint8x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>
uint8x8x2_t test_vld2_lane_u8(uint8_t const * a, uint8x8x2_t b) {
  return vld2_lane_u8(a, b, 7);
}

// CHECK-LABEL: @test_vld2_lane_u16(
// CHECK:   [[B:%.*]] = alloca %struct.uint16x4x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x4x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint16x4x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint16x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>
uint16x4x2_t test_vld2_lane_u16(uint16_t const * a, uint16x4x2_t b) {
  return vld2_lane_u16(a, b, 3);
}

// CHECK-LABEL: @test_vld2_lane_u32(
// CHECK:   [[B:%.*]] = alloca %struct.uint32x2x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x2x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint32x2x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint32x2x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <2 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x i32>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x i32> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <2 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x i32>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x i32> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x i32>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x i32>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <2 x i32>, <2 x i32>
uint32x2x2_t test_vld2_lane_u32(uint32_t const * a, uint32x2x2_t b) {
  return vld2_lane_u32(a, b, 1);
}

// CHECK-LABEL: @test_vld2_lane_s8(
// CHECK:   [[B:%.*]] = alloca %struct.int8x8x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x8x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int8x8x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int8x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>
int8x8x2_t test_vld2_lane_s8(int8_t const * a, int8x8x2_t b) {
  return vld2_lane_s8(a, b, 7);
}

// CHECK-LABEL: @test_vld2_lane_s16(
// CHECK:   [[B:%.*]] = alloca %struct.int16x4x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x4x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int16x4x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int16x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>
int16x4x2_t test_vld2_lane_s16(int16_t const * a, int16x4x2_t b) {
  return vld2_lane_s16(a, b, 3);
}

// CHECK-LABEL: @test_vld2_lane_s32(
// CHECK:   [[B:%.*]] = alloca %struct.int32x2x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x2x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int32x2x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int32x2x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <2 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x i32>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x i32> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <2 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x i32>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x i32> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x i32>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x i32>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <2 x i32>, <2 x i32>
int32x2x2_t test_vld2_lane_s32(int32_t const * a, int32x2x2_t b) {
  return vld2_lane_s32(a, b, 1);
}

// CHECK-LABEL: @test_vld2_lane_f16(
// CHECK:   [[B:%.*]] = alloca %struct.float16x4x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x4x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.float16x4x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float16x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x half>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x half>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x half> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x half>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x half>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x half> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x half>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x half>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <4 x half>, <4 x half>
float16x4x2_t test_vld2_lane_f16(float16_t const * a, float16x4x2_t b) {
  return vld2_lane_f16(a, b, 3);
}

// CHECK-LABEL: @test_vld2_lane_f32(
// CHECK:   [[B:%.*]] = alloca %struct.float32x2x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x2x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.float32x2x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float32x2x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <2 x float>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x float>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x float> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float32x2x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <2 x float>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x float>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x float> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x float>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x float>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <2 x float>, <2 x float>
float32x2x2_t test_vld2_lane_f32(float32_t const * a, float32x2x2_t b) {
  return vld2_lane_f32(a, b, 1);
}

// CHECK-LABEL: @test_vld2_lane_p8(
// CHECK:   [[B:%.*]] = alloca %struct.poly8x8x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x8x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.poly8x8x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly8x8x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly8x8x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>
poly8x8x2_t test_vld2_lane_p8(poly8_t const * a, poly8x8x2_t b) {
  return vld2_lane_p8(a, b, 7);
}

// CHECK-LABEL: @test_vld2_lane_p16(
// CHECK:   [[B:%.*]] = alloca %struct.poly16x4x2_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x4x2_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.poly16x4x2_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly16x4x2_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [2 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 16, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly16x4x2_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [2 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[TMP9:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[VLD2_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>
poly16x4x2_t test_vld2_lane_p16(poly16_t const * a, poly16x4x2_t b) {
  return vld2_lane_p16(a, b, 3);
}

// CHECK-LABEL: @test_vld3q_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x16x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>
uint8x16x3_t test_vld3q_u8(uint8_t const * a) {
  return vld3q_u8(a);
}

// CHECK-LABEL: @test_vld3q_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
uint16x8x3_t test_vld3q_u16(uint16_t const * a) {
  return vld3q_u16(a);
}

// CHECK-LABEL: @test_vld3q_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>
uint32x4x3_t test_vld3q_u32(uint32_t const * a) {
  return vld3q_u32(a);
}

// CHECK-LABEL: @test_vld3q_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x16x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>
int8x16x3_t test_vld3q_s8(int8_t const * a) {
  return vld3q_s8(a);
}

// CHECK-LABEL: @test_vld3q_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
int16x8x3_t test_vld3q_s16(int16_t const * a) {
  return vld3q_s16(a);
}

// CHECK-LABEL: @test_vld3q_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>
int32x4x3_t test_vld3q_s32(int32_t const * a) {
  return vld3q_s32(a);
}

// CHECK-LABEL: @test_vld3q_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x8x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <8 x half>, <8 x half>, <8 x half>
float16x8x3_t test_vld3q_f16(float16_t const * a) {
  return vld3q_f16(a);
}

// CHECK-LABEL: @test_vld3q_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x4x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <4 x float>, <4 x float>, <4 x float>
float32x4x3_t test_vld3q_f32(float32_t const * a) {
  return vld3q_f32(a);
}

// CHECK-LABEL: @test_vld3q_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x16x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>
poly8x16x3_t test_vld3q_p8(poly8_t const * a) {
  return vld3q_p8(a);
}

// CHECK-LABEL: @test_vld3q_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x8x3_t, align 16
// CHECK:   [[VLD3Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
poly16x8x3_t test_vld3q_p16(poly16_t const * a) {
  return vld3q_p16(a);
}

// CHECK-LABEL: @test_vld3_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x8x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
uint8x8x3_t test_vld3_u8(uint8_t const * a) {
  return vld3_u8(a);
}

// CHECK-LABEL: @test_vld3_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x4x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
uint16x4x3_t test_vld3_u16(uint16_t const * a) {
  return vld3_u16(a);
}

// CHECK-LABEL: @test_vld3_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x2x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>
uint32x2x3_t test_vld3_u32(uint32_t const * a) {
  return vld3_u32(a);
}

// CHECK-LABEL: @test_vld3_u64(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint64x1x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64>
uint64x1x3_t test_vld3_u64(uint64_t const * a) {
  return vld3_u64(a);
}

// CHECK-LABEL: @test_vld3_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x8x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
int8x8x3_t test_vld3_s8(int8_t const * a) {
  return vld3_s8(a);
}

// CHECK-LABEL: @test_vld3_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x4x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
int16x4x3_t test_vld3_s16(int16_t const * a) {
  return vld3_s16(a);
}

// CHECK-LABEL: @test_vld3_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x2x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>
int32x2x3_t test_vld3_s32(int32_t const * a) {
  return vld3_s32(a);
}

// CHECK-LABEL: @test_vld3_s64(
// CHECK:   [[__RET:%.*]] = alloca %struct.int64x1x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64>
int64x1x3_t test_vld3_s64(int64_t const * a) {
  return vld3_s64(a);
}

// CHECK-LABEL: @test_vld3_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x4x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <4 x half>, <4 x half>, <4 x half>
float16x4x3_t test_vld3_f16(float16_t const * a) {
  return vld3_f16(a);
}

// CHECK-LABEL: @test_vld3_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x2x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <2 x float>, <2 x float>, <2 x float>
float32x2x3_t test_vld3_f32(float32_t const * a) {
  return vld3_f32(a);
}

// CHECK-LABEL: @test_vld3_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x8x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
poly8x8x3_t test_vld3_p8(poly8_t const * a) {
  return vld3_p8(a);
}

// CHECK-LABEL: @test_vld3_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x4x3_t, align 8
// CHECK:   [[VLD3_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
poly16x4x3_t test_vld3_p16(poly16_t const * a) {
  return vld3_p16(a);
}

// CHECK-LABEL: @test_vld3q_lane_u16(
// CHECK:   [[B:%.*]] = alloca %struct.uint16x8x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint16x8x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint16x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x i16>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i16> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x i16>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
uint16x8x3_t test_vld3q_lane_u16(uint16_t const * a, uint16x8x3_t b) {
  return vld3q_lane_u16(a, b, 7);
}

// CHECK-LABEL: @test_vld3q_lane_u32(
// CHECK:   [[B:%.*]] = alloca %struct.uint32x4x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint32x4x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint32x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i32>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i32> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i32>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i32> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x i32>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x i32>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <4 x i32>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>
uint32x4x3_t test_vld3q_lane_u32(uint32_t const * a, uint32x4x3_t b) {
  return vld3q_lane_u32(a, b, 3);
}

// CHECK-LABEL: @test_vld3q_lane_s16(
// CHECK:   [[B:%.*]] = alloca %struct.int16x8x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int16x8x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int16x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x i16>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i16> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x i16>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
int16x8x3_t test_vld3q_lane_s16(int16_t const * a, int16x8x3_t b) {
  return vld3q_lane_s16(a, b, 7);
}

// CHECK-LABEL: @test_vld3q_lane_s32(
// CHECK:   [[B:%.*]] = alloca %struct.int32x4x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int32x4x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int32x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i32>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i32> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x i32>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i32>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i32> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x i32>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x i32>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <4 x i32>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>
int32x4x3_t test_vld3q_lane_s32(int32_t const * a, int32x4x3_t b) {
  return vld3q_lane_s32(a, b, 3);
}

// CHECK-LABEL: @test_vld3q_lane_f16(
// CHECK:   [[B:%.*]] = alloca %struct.float16x8x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x8x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.float16x8x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float16x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x half>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x half>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x half> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x half>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x half>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x half> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.float16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x half>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x half>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x half> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x half>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x half>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x half>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <8 x half>, <8 x half>, <8 x half>
float16x8x3_t test_vld3q_lane_f16(float16_t const * a, float16x8x3_t b) {
  return vld3q_lane_f16(a, b, 7);
}

// CHECK-LABEL: @test_vld3q_lane_f32(
// CHECK:   [[B:%.*]] = alloca %struct.float32x4x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x4x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.float32x4x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float32x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x float>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x float>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x float> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x float>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x float>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x float> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.float32x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x float>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x float>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <4 x float> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x float>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x float>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <4 x float>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <4 x float>, <4 x float>, <4 x float>
float32x4x3_t test_vld3q_lane_f32(float32_t const * a, float32x4x3_t b) {
  return vld3q_lane_f32(a, b, 3);
}

// CHECK-LABEL: @test_vld3q_lane_p16(
// CHECK:   [[B:%.*]] = alloca %struct.poly16x8x3_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x8x3_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.poly16x8x3_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly16x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [6 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 48, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.poly16x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x i16>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i16> [[TMP9]] to <16 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x i16>
// CHECK:   [[VLD3Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>
poly16x8x3_t test_vld3q_lane_p16(poly16_t const * a, poly16x8x3_t b) {
  return vld3q_lane_p16(a, b, 7);
}

// CHECK-LABEL: @test_vld3_lane_u8(
// CHECK:   [[B:%.*]] = alloca %struct.uint8x8x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x8x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint8x8x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint8x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP6:%.*]] = load <8 x i8>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
uint8x8x3_t test_vld3_lane_u8(uint8_t const * a, uint8x8x3_t b) {
  return vld3_lane_u8(a, b, 7);
}

// CHECK-LABEL: @test_vld3_lane_u16(
// CHECK:   [[B:%.*]] = alloca %struct.uint16x4x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x4x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint16x4x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint16x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i16>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i16> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <4 x i16>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
uint16x4x3_t test_vld3_lane_u16(uint16_t const * a, uint16x4x3_t b) {
  return vld3_lane_u16(a, b, 3);
}

// CHECK-LABEL: @test_vld3_lane_u32(
// CHECK:   [[B:%.*]] = alloca %struct.uint32x2x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x2x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.uint32x2x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint32x2x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x i32>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x i32> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x i32>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x i32> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <2 x i32>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <2 x i32> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x i32>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x i32>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <2 x i32>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>
uint32x2x3_t test_vld3_lane_u32(uint32_t const * a, uint32x2x3_t b) {
  return vld3_lane_u32(a, b, 1);
}

// CHECK-LABEL: @test_vld3_lane_s8(
// CHECK:   [[B:%.*]] = alloca %struct.int8x8x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x8x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int8x8x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int8x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP6:%.*]] = load <8 x i8>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
int8x8x3_t test_vld3_lane_s8(int8_t const * a, int8x8x3_t b) {
  return vld3_lane_s8(a, b, 7);
}

// CHECK-LABEL: @test_vld3_lane_s16(
// CHECK:   [[B:%.*]] = alloca %struct.int16x4x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x4x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int16x4x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int16x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i16>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i16> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <4 x i16>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
int16x4x3_t test_vld3_lane_s16(int16_t const * a, int16x4x3_t b) {
  return vld3_lane_s16(a, b, 3);
}

// CHECK-LABEL: @test_vld3_lane_s32(
// CHECK:   [[B:%.*]] = alloca %struct.int32x2x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x2x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.int32x2x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int32x2x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x i32>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x i32> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x i32>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x i32> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <2 x i32>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <2 x i32>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <2 x i32> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x i32>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x i32>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <2 x i32>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>
int32x2x3_t test_vld3_lane_s32(int32_t const * a, int32x2x3_t b) {
  return vld3_lane_s32(a, b, 1);
}

// CHECK-LABEL: @test_vld3_lane_f16(
// CHECK:   [[B:%.*]] = alloca %struct.float16x4x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x4x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.float16x4x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float16x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x half>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x half>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x half> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x half>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x half>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x half> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.float16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x half>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x half>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <4 x half> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x half>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x half>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <4 x half>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <4 x half>, <4 x half>, <4 x half>
float16x4x3_t test_vld3_lane_f16(float16_t const * a, float16x4x3_t b) {
  return vld3_lane_f16(a, b, 3);
}

// CHECK-LABEL: @test_vld3_lane_f32(
// CHECK:   [[B:%.*]] = alloca %struct.float32x2x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x2x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.float32x2x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.float32x2x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.float32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <2 x float>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <2 x float>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <2 x float> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.float32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <2 x float>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <2 x float>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <2 x float> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.float32x2x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <2 x float>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <2 x float>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <2 x float> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <2 x float>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <2 x float>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <2 x float>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <2 x float>, <2 x float>, <2 x float>
float32x2x3_t test_vld3_lane_f32(float32_t const * a, float32x2x3_t b) {
  return vld3_lane_f32(a, b, 1);
}

// CHECK-LABEL: @test_vld3_lane_p8(
// CHECK:   [[B:%.*]] = alloca %struct.poly8x8x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x8x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.poly8x8x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly8x8x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP4:%.*]] = load <8 x i8>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP5:%.*]] = load <8 x i8>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.poly8x8x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <8 x i8>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP6:%.*]] = load <8 x i8>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>
poly8x8x3_t test_vld3_lane_p8(poly8_t const * a, poly8x8x3_t b) {
  return vld3_lane_p8(a, b, 7);
}

// CHECK-LABEL: @test_vld3_lane_p16(
// CHECK:   [[B:%.*]] = alloca %struct.poly16x4x3_t, align 8
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x4x3_t, align 8
// CHECK:   [[__S1:%.*]] = alloca %struct.poly16x4x3_t, align 8
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.poly16x4x3_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [3 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 8
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 8 [[__S1]], ptr align 8 [[B]], i32 24, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.poly16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i16>, ptr [[ARRAYIDX]], align 8
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i16> [[TMP5]] to <8 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.poly16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i16>, ptr [[ARRAYIDX2]], align 8
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i16> [[TMP7]] to <8 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.poly16x4x3_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [3 x <4 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i16>, ptr [[ARRAYIDX4]], align 8
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i16> [[TMP9]] to <8 x i8>
// CHECK:   [[TMP11:%.*]] = bitcast <8 x i8> [[TMP6]] to <4 x i16>
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i8> [[TMP8]] to <4 x i16>
// CHECK:   [[TMP13:%.*]] = bitcast <8 x i8> [[TMP10]] to <4 x i16>
// CHECK:   [[VLD3_LANE_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>
poly16x4x3_t test_vld3_lane_p16(poly16_t const * a, poly16x4x3_t b) {
  return vld3_lane_p16(a, b, 3);
}

// CHECK-LABEL: @test_vld4q_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x16x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>, <16 x i8>
uint8x16x4_t test_vld4q_u8(uint8_t const * a) {
  return vld4q_u8(a);
}

// CHECK-LABEL: @test_vld4q_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>, <8 x i16>
uint16x8x4_t test_vld4q_u16(uint16_t const * a) {
  return vld4q_u16(a);
}

// CHECK-LABEL: @test_vld4q_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>
uint32x4x4_t test_vld4q_u32(uint32_t const * a) {
  return vld4q_u32(a);
}

// CHECK-LABEL: @test_vld4q_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x16x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>, <16 x i8>
int8x16x4_t test_vld4q_s8(int8_t const * a) {
  return vld4q_s8(a);
}

// CHECK-LABEL: @test_vld4q_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>, <8 x i16>
int16x8x4_t test_vld4q_s16(int16_t const * a) {
  return vld4q_s16(a);
}

// CHECK-LABEL: @test_vld4q_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>
int32x4x4_t test_vld4q_s32(int32_t const * a) {
  return vld4q_s32(a);
}

// CHECK-LABEL: @test_vld4q_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x8x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <8 x half>, <8 x half>, <8 x half>, <8 x half>
float16x8x4_t test_vld4q_f16(float16_t const * a) {
  return vld4q_f16(a);
}

// CHECK-LABEL: @test_vld4q_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x4x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <4 x float>, <4 x float>, <4 x float>, <4 x float>
float32x4x4_t test_vld4q_f32(float32_t const * a) {
  return vld4q_f32(a);
}

// CHECK-LABEL: @test_vld4q_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x16x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <16 x i8>, <16 x i8>, <16 x i8>, <16 x i8>
poly8x16x4_t test_vld4q_p8(poly8_t const * a) {
  return vld4q_p8(a);
}

// CHECK-LABEL: @test_vld4q_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x8x4_t, align 16
// CHECK:   [[VLD4Q_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>, <8 x i16>
poly16x8x4_t test_vld4q_p16(poly16_t const * a) {
  return vld4q_p16(a);
}

// CHECK-LABEL: @test_vld4_u8(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint8x8x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>, <8 x i8>
uint8x8x4_t test_vld4_u8(uint8_t const * a) {
  return vld4_u8(a);
}

// CHECK-LABEL: @test_vld4_u16(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x4x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>, <4 x i16>
uint16x4x4_t test_vld4_u16(uint16_t const * a) {
  return vld4_u16(a);
}

// CHECK-LABEL: @test_vld4_u32(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x2x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>, <2 x i32>
uint32x2x4_t test_vld4_u32(uint32_t const * a) {
  return vld4_u32(a);
}

// CHECK-LABEL: @test_vld4_u64(
// CHECK:   [[__RET:%.*]] = alloca %struct.uint64x1x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64>
uint64x1x4_t test_vld4_u64(uint64_t const * a) {
  return vld4_u64(a);
}

// CHECK-LABEL: @test_vld4_s8(
// CHECK:   [[__RET:%.*]] = alloca %struct.int8x8x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>, <8 x i8>
int8x8x4_t test_vld4_s8(int8_t const * a) {
  return vld4_s8(a);
}

// CHECK-LABEL: @test_vld4_s16(
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x4x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>, <4 x i16>
int16x4x4_t test_vld4_s16(int16_t const * a) {
  return vld4_s16(a);
}

// CHECK-LABEL: @test_vld4_s32(
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x2x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <2 x i32>, <2 x i32>, <2 x i32>, <2 x i32>
int32x2x4_t test_vld4_s32(int32_t const * a) {
  return vld4_s32(a);
}

// CHECK-LABEL: @test_vld4_s64(
// CHECK:   [[__RET:%.*]] = alloca %struct.int64x1x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <1 x i64>, <1 x i64>, <1 x i64>, <1 x i64>
int64x1x4_t test_vld4_s64(int64_t const * a) {
  return vld4_s64(a);
}

// CHECK-LABEL: @test_vld4_f16(
// CHECK:   [[__RET:%.*]] = alloca %struct.float16x4x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <4 x half>, <4 x half>, <4 x half>, <4 x half>
float16x4x4_t test_vld4_f16(float16_t const * a) {
  return vld4_f16(a);
}

// CHECK-LABEL: @test_vld4_f32(
// CHECK:   [[__RET:%.*]] = alloca %struct.float32x2x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <2 x float>, <2 x float>, <2 x float>, <2 x float>
float32x2x4_t test_vld4_f32(float32_t const * a) {
  return vld4_f32(a);
}

// CHECK-LABEL: @test_vld4_p8(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly8x8x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <8 x i8>, <8 x i8>, <8 x i8>, <8 x i8>
poly8x8x4_t test_vld4_p8(poly8_t const * a) {
  return vld4_p8(a);
}

// CHECK-LABEL: @test_vld4_p16(
// CHECK:   [[__RET:%.*]] = alloca %struct.poly16x4x4_t, align 8
// CHECK:   [[VLD4_V:%.*]] = call { <4 x i16>, <4 x i16>, <4 x i16>, <4 x i16>
poly16x4x4_t test_vld4_p16(poly16_t const * a) {
  return vld4_p16(a);
}

// CHECK-LABEL: @test_vld4q_lane_u16(
// CHECK:   [[B:%.*]] = alloca %struct.uint16x8x4_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint16x8x4_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint16x8x4_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint16x8x4_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [8 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 64, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x i16>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i16> [[TMP9]] to <16 x i8>
// CHECK:   [[VAL5:%.*]] = getelementptr inbounds %struct.uint16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX6:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL5]], i32 0, i32 3
// CHECK:   [[TMP11:%.*]] = load <8 x i16>, ptr [[ARRAYIDX6]], align 16
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i16> [[TMP11]] to <16 x i8>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP14:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[TMP15:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x i16>
// CHECK:   [[TMP16:%.*]] = bitcast <16 x i8> [[TMP12]] to <8 x i16>
// CHECK:   [[VLD4Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>, <8 x i16>
uint16x8x4_t test_vld4q_lane_u16(uint16_t const * a, uint16x8x4_t b) {
  return vld4q_lane_u16(a, b, 7);
}

// CHECK-LABEL: @test_vld4q_lane_u32(
// CHECK:   [[B:%.*]] = alloca %struct.uint32x4x4_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.uint32x4x4_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.uint32x4x4_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.uint32x4x4_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [8 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 64, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.uint32x4x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [4 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.uint32x4x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [4 x <4 x i32>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <4 x i32>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <4 x i32> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.uint32x4x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [4 x <4 x i32>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <4 x i32>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <4 x i32> [[TMP9]] to <16 x i8>
// CHECK:   [[VAL5:%.*]] = getelementptr inbounds %struct.uint32x4x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX6:%.*]] = getelementptr inbounds [4 x <4 x i32>], ptr [[VAL5]], i32 0, i32 3
// CHECK:   [[TMP11:%.*]] = load <4 x i32>, ptr [[ARRAYIDX6]], align 16
// CHECK:   [[TMP12:%.*]] = bitcast <4 x i32> [[TMP11]] to <16 x i8>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP6]] to <4 x i32>
// CHECK:   [[TMP14:%.*]] = bitcast <16 x i8> [[TMP8]] to <4 x i32>
// CHECK:   [[TMP15:%.*]] = bitcast <16 x i8> [[TMP10]] to <4 x i32>
// CHECK:   [[TMP16:%.*]] = bitcast <16 x i8> [[TMP12]] to <4 x i32>
// CHECK:   [[VLD4Q_LANE_V:%.*]] = call { <4 x i32>, <4 x i32>, <4 x i32>, <4 x i32>
uint32x4x4_t test_vld4q_lane_u32(uint32_t const * a, uint32x4x4_t b) {
  return vld4q_lane_u32(a, b, 3);
}

// CHECK-LABEL: @test_vld4q_lane_s16(
// CHECK:   [[B:%.*]] = alloca %struct.int16x8x4_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int16x8x4_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int16x8x4_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int16x8x4_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [8 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 64, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <8 x i16>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <8 x i16> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] = getelementptr inbounds %struct.int16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX2:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL1]], i32 0, i32 1
// CHECK:   [[TMP7:%.*]] = load <8 x i16>, ptr [[ARRAYIDX2]], align 16
// CHECK:   [[TMP8:%.*]] = bitcast <8 x i16> [[TMP7]] to <16 x i8>
// CHECK:   [[VAL3:%.*]] = getelementptr inbounds %struct.int16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX4:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL3]], i32 0, i32 2
// CHECK:   [[TMP9:%.*]] = load <8 x i16>, ptr [[ARRAYIDX4]], align 16
// CHECK:   [[TMP10:%.*]] = bitcast <8 x i16> [[TMP9]] to <16 x i8>
// CHECK:   [[VAL5:%.*]] = getelementptr inbounds %struct.int16x8x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX6:%.*]] = getelementptr inbounds [4 x <8 x i16>], ptr [[VAL5]], i32 0, i32 3
// CHECK:   [[TMP11:%.*]] = load <8 x i16>, ptr [[ARRAYIDX6]], align 16
// CHECK:   [[TMP12:%.*]] = bitcast <8 x i16> [[TMP11]] to <16 x i8>
// CHECK:   [[TMP13:%.*]] = bitcast <16 x i8> [[TMP6]] to <8 x i16>
// CHECK:   [[TMP14:%.*]] = bitcast <16 x i8> [[TMP8]] to <8 x i16>
// CHECK:   [[TMP15:%.*]] = bitcast <16 x i8> [[TMP10]] to <8 x i16>
// CHECK:   [[TMP16:%.*]] = bitcast <16 x i8> [[TMP12]] to <8 x i16>
// CHECK:   [[VLD4Q_LANE_V:%.*]] = call { <8 x i16>, <8 x i16>, <8 x i16>, <8 x i16>
int16x8x4_t test_vld4q_lane_s16(int16_t const * a, int16x8x4_t b) {
  return vld4q_lane_s16(a, b, 7);
}

// CHECK-LABEL: @test_vld4q_lane_s32(
// CHECK:   [[B:%.*]] = alloca %struct.int32x4x4_t, align 16
// CHECK:   [[__RET:%.*]] = alloca %struct.int32x4x4_t, align 16
// CHECK:   [[__S1:%.*]] = alloca %struct.int32x4x4_t, align 16
// CHECK:   [[COERCE_DIVE:%.*]] = getelementptr inbounds %struct.int32x4x4_t, ptr [[B]], i32 0, i32 0
// CHECK:   store [8 x i64] [[B]].coerce, ptr [[COERCE_DIVE]], align 16
// CHECK:   call void @llvm.memcpy.p0.p0.i32(ptr align 16 [[__S1]], ptr align 16 [[B]], i32 64, i1 false)
// CHECK:   [[VAL:%.*]] = getelementptr inbounds %struct.int32x4x4_t, ptr [[__S1]], i32 0, i32 0
// CHECK:   [[ARRAYIDX:%.*]] = getelementptr inbounds [4 x <4 x i32>], ptr [[VAL]], i32 0, i32 0
// CHECK:   [[TMP5:%.*]] = load <4 x i32>, ptr [[ARRAYIDX]], align 16
// CHECK:   [[TMP6:%.*]] = bitcast <4 x i32> [[TMP5]] to <16 x i8>
// CHECK:   [[VAL1:%.*]] =