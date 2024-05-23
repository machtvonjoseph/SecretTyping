// RUN: %clang_cc1 -triple arm64-none-linux-gnu -target-feature +neon \
// RUN:     -S -disable-O0-optnone \
// RUN:  -flax-vector-conversions=none -emit-llvm -o - %s \
// RUN: | opt -S -passes=mem2reg \
// RUN: | FileCheck %s

// REQUIRES: aarch64-registered-target || arm-registered-target

#include <arm_neon.h>

// CHECK-LABEL: @test_vadd_s8(
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[ADD_I]]
int8x8_t test_vadd_s8(int8x8_t v1, int8x8_t v2) {
  return vadd_s8(v1, v2);
}

// CHECK-LABEL: @test_vadd_s16(
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[ADD_I]]
int16x4_t test_vadd_s16(int16x4_t v1, int16x4_t v2) {
  return vadd_s16(v1, v2);
}

// CHECK-LABEL: @test_vadd_s32(
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[ADD_I]]
int32x2_t test_vadd_s32(int32x2_t v1, int32x2_t v2) {
  return vadd_s32(v1, v2);
}

// CHECK-LABEL: @test_vadd_s64(
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> %v1, %v2
// CHECK:   ret <1 x i64> [[ADD_I]]
int64x1_t test_vadd_s64(int64x1_t v1, int64x1_t v2) {
  return vadd_s64(v1, v2);
}

// CHECK-LABEL: @test_vadd_f32(
// CHECK:   [[ADD_I:%.*]] = fadd <2 x float> %v1, %v2
// CHECK:   ret <2 x float> [[ADD_I]]
float32x2_t test_vadd_f32(float32x2_t v1, float32x2_t v2) {
  return vadd_f32(v1, v2);
}

// CHECK-LABEL: @test_vadd_u8(
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[ADD_I]]
uint8x8_t test_vadd_u8(uint8x8_t v1, uint8x8_t v2) {
  return vadd_u8(v1, v2);
}

// CHECK-LABEL: @test_vadd_u16(
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[ADD_I]]
uint16x4_t test_vadd_u16(uint16x4_t v1, uint16x4_t v2) {
  return vadd_u16(v1, v2);
}

// CHECK-LABEL: @test_vadd_u32(
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[ADD_I]]
uint32x2_t test_vadd_u32(uint32x2_t v1, uint32x2_t v2) {
  return vadd_u32(v1, v2);
}

// CHECK-LABEL: @test_vadd_u64(
// CHECK:   [[ADD_I:%.*]] = add <1 x i64> %v1, %v2
// CHECK:   ret <1 x i64> [[ADD_I]]
uint64x1_t test_vadd_u64(uint64x1_t v1, uint64x1_t v2) {
  return vadd_u64(v1, v2);
}

// CHECK-LABEL: @test_vaddq_s8(
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[ADD_I]]
int8x16_t test_vaddq_s8(int8x16_t v1, int8x16_t v2) {
  return vaddq_s8(v1, v2);
}

// CHECK-LABEL: @test_vaddq_s16(
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vaddq_s16(int16x8_t v1, int16x8_t v2) {
  return vaddq_s16(v1, v2);
}

// CHECK-LABEL: @test_vaddq_s32(
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vaddq_s32(int32x4_t v1, int32x4_t v2) {
  return vaddq_s32(v1, v2);
}

// CHECK-LABEL: @test_vaddq_s64(
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %v1, %v2
// CHECK:   ret <2 x i64> [[ADD_I]]
int64x2_t test_vaddq_s64(int64x2_t v1, int64x2_t v2) {
  return vaddq_s64(v1, v2);
}

// CHECK-LABEL: @test_vaddq_f32(
// CHECK:   [[ADD_I:%.*]] = fadd <4 x float> %v1, %v2
// CHECK:   ret <4 x float> [[ADD_I]]
float32x4_t test_vaddq_f32(float32x4_t v1, float32x4_t v2) {
  return vaddq_f32(v1, v2);
}

// CHECK-LABEL: @test_vaddq_f64(
// CHECK:   [[ADD_I:%.*]] = fadd <2 x double> %v1, %v2
// CHECK:   ret <2 x double> [[ADD_I]]
float64x2_t test_vaddq_f64(float64x2_t v1, float64x2_t v2) {
  return vaddq_f64(v1, v2);
}

// CHECK-LABEL: @test_vaddq_u8(
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[ADD_I]]
uint8x16_t test_vaddq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vaddq_u8(v1, v2);
}

// CHECK-LABEL: @test_vaddq_u16(
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vaddq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vaddq_u16(v1, v2);
}

// CHECK-LABEL: @test_vaddq_u32(
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vaddq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vaddq_u32(v1, v2);
}

// CHECK-LABEL: @test_vaddq_u64(
// CHECK:   [[ADD_I:%.*]] = add <2 x i64> %v1, %v2
// CHECK:   ret <2 x i64> [[ADD_I]]
uint64x2_t test_vaddq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vaddq_u64(v1, v2);
}

// CHECK-LABEL: @test_vsub_s8(
// CHECK:   [[SUB_I:%.*]] = sub <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[SUB_I]]
int8x8_t test_vsub_s8(int8x8_t v1, int8x8_t v2) {
  return vsub_s8(v1, v2);
}

// CHECK-LABEL: @test_vsub_s16(
// CHECK:   [[SUB_I:%.*]] = sub <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[SUB_I]]
int16x4_t test_vsub_s16(int16x4_t v1, int16x4_t v2) {
  return vsub_s16(v1, v2);
}

// CHECK-LABEL: @test_vsub_s32(
// CHECK:   [[SUB_I:%.*]] = sub <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[SUB_I]]
int32x2_t test_vsub_s32(int32x2_t v1, int32x2_t v2) {
  return vsub_s32(v1, v2);
}

// CHECK-LABEL: @test_vsub_s64(
// CHECK:   [[SUB_I:%.*]] = sub <1 x i64> %v1, %v2
// CHECK:   ret <1 x i64> [[SUB_I]]
int64x1_t test_vsub_s64(int64x1_t v1, int64x1_t v2) {
  return vsub_s64(v1, v2);
}

// CHECK-LABEL: @test_vsub_f32(
// CHECK:   [[SUB_I:%.*]] = fsub <2 x float> %v1, %v2
// CHECK:   ret <2 x float> [[SUB_I]]
float32x2_t test_vsub_f32(float32x2_t v1, float32x2_t v2) {
  return vsub_f32(v1, v2);
}

// CHECK-LABEL: @test_vsub_u8(
// CHECK:   [[SUB_I:%.*]] = sub <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[SUB_I]]
uint8x8_t test_vsub_u8(uint8x8_t v1, uint8x8_t v2) {
  return vsub_u8(v1, v2);
}

// CHECK-LABEL: @test_vsub_u16(
// CHECK:   [[SUB_I:%.*]] = sub <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[SUB_I]]
uint16x4_t test_vsub_u16(uint16x4_t v1, uint16x4_t v2) {
  return vsub_u16(v1, v2);
}

// CHECK-LABEL: @test_vsub_u32(
// CHECK:   [[SUB_I:%.*]] = sub <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[SUB_I]]
uint32x2_t test_vsub_u32(uint32x2_t v1, uint32x2_t v2) {
  return vsub_u32(v1, v2);
}

// CHECK-LABEL: @test_vsub_u64(
// CHECK:   [[SUB_I:%.*]] = sub <1 x i64> %v1, %v2
// CHECK:   ret <1 x i64> [[SUB_I]]
uint64x1_t test_vsub_u64(uint64x1_t v1, uint64x1_t v2) {
  return vsub_u64(v1, v2);
}

// CHECK-LABEL: @test_vsubq_s8(
// CHECK:   [[SUB_I:%.*]] = sub <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[SUB_I]]
int8x16_t test_vsubq_s8(int8x16_t v1, int8x16_t v2) {
  return vsubq_s8(v1, v2);
}

// CHECK-LABEL: @test_vsubq_s16(
// CHECK:   [[SUB_I:%.*]] = sub <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[SUB_I]]
int16x8_t test_vsubq_s16(int16x8_t v1, int16x8_t v2) {
  return vsubq_s16(v1, v2);
}

// CHECK-LABEL: @test_vsubq_s32(
// CHECK:   [[SUB_I:%.*]] = sub <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[SUB_I]]
int32x4_t test_vsubq_s32(int32x4_t v1, int32x4_t v2) {
  return vsubq_s32(v1, v2);
}

// CHECK-LABEL: @test_vsubq_s64(
// CHECK:   [[SUB_I:%.*]] = sub <2 x i64> %v1, %v2
// CHECK:   ret <2 x i64> [[SUB_I]]
int64x2_t test_vsubq_s64(int64x2_t v1, int64x2_t v2) {
  return vsubq_s64(v1, v2);
}

// CHECK-LABEL: @test_vsubq_f32(
// CHECK:   [[SUB_I:%.*]] = fsub <4 x float> %v1, %v2
// CHECK:   ret <4 x float> [[SUB_I]]
float32x4_t test_vsubq_f32(float32x4_t v1, float32x4_t v2) {
  return vsubq_f32(v1, v2);
}

// CHECK-LABEL: @test_vsubq_f64(
// CHECK:   [[SUB_I:%.*]] = fsub <2 x double> %v1, %v2
// CHECK:   ret <2 x double> [[SUB_I]]
float64x2_t test_vsubq_f64(float64x2_t v1, float64x2_t v2) {
  return vsubq_f64(v1, v2);
}

// CHECK-LABEL: @test_vsubq_u8(
// CHECK:   [[SUB_I:%.*]] = sub <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[SUB_I]]
uint8x16_t test_vsubq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vsubq_u8(v1, v2);
}

// CHECK-LABEL: @test_vsubq_u16(
// CHECK:   [[SUB_I:%.*]] = sub <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[SUB_I]]
uint16x8_t test_vsubq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vsubq_u16(v1, v2);
}

// CHECK-LABEL: @test_vsubq_u32(
// CHECK:   [[SUB_I:%.*]] = sub <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[SUB_I]]
uint32x4_t test_vsubq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vsubq_u32(v1, v2);
}

// CHECK-LABEL: @test_vsubq_u64(
// CHECK:   [[SUB_I:%.*]] = sub <2 x i64> %v1, %v2
// CHECK:   ret <2 x i64> [[SUB_I]]
uint64x2_t test_vsubq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vsubq_u64(v1, v2);
}

// CHECK-LABEL: @test_vmul_s8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[MUL_I]]
int8x8_t test_vmul_s8(int8x8_t v1, int8x8_t v2) {
  return vmul_s8(v1, v2);
}

// CHECK-LABEL: @test_vmul_s16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[MUL_I]]
int16x4_t test_vmul_s16(int16x4_t v1, int16x4_t v2) {
  return vmul_s16(v1, v2);
}

// CHECK-LABEL: @test_vmul_s32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[MUL_I]]
int32x2_t test_vmul_s32(int32x2_t v1, int32x2_t v2) {
  return vmul_s32(v1, v2);
}

// CHECK-LABEL: @test_vmul_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x float> %v1, %v2
// CHECK:   ret <2 x float> [[MUL_I]]
float32x2_t test_vmul_f32(float32x2_t v1, float32x2_t v2) {
  return vmul_f32(v1, v2);
}

// CHECK-LABEL: @test_vmul_u8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v1, %v2
// CHECK:   ret <8 x i8> [[MUL_I]]
uint8x8_t test_vmul_u8(uint8x8_t v1, uint8x8_t v2) {
  return vmul_u8(v1, v2);
}

// CHECK-LABEL: @test_vmul_u16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v1, %v2
// CHECK:   ret <4 x i16> [[MUL_I]]
uint16x4_t test_vmul_u16(uint16x4_t v1, uint16x4_t v2) {
  return vmul_u16(v1, v2);
}

// CHECK-LABEL: @test_vmul_u32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v1, %v2
// CHECK:   ret <2 x i32> [[MUL_I]]
uint32x2_t test_vmul_u32(uint32x2_t v1, uint32x2_t v2) {
  return vmul_u32(v1, v2);
}

// CHECK-LABEL: @test_vmulq_s8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[MUL_I]]
int8x16_t test_vmulq_s8(int8x16_t v1, int8x16_t v2) {
  return vmulq_s8(v1, v2);
}

// CHECK-LABEL: @test_vmulq_s16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[MUL_I]]
int16x8_t test_vmulq_s16(int16x8_t v1, int16x8_t v2) {
  return vmulq_s16(v1, v2);
}

// CHECK-LABEL: @test_vmulq_s32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[MUL_I]]
int32x4_t test_vmulq_s32(int32x4_t v1, int32x4_t v2) {
  return vmulq_s32(v1, v2);
}

// CHECK-LABEL: @test_vmulq_u8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v1, %v2
// CHECK:   ret <16 x i8> [[MUL_I]]
uint8x16_t test_vmulq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vmulq_u8(v1, v2);
}

// CHECK-LABEL: @test_vmulq_u16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v1, %v2
// CHECK:   ret <8 x i16> [[MUL_I]]
uint16x8_t test_vmulq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vmulq_u16(v1, v2);
}

// CHECK-LABEL: @test_vmulq_u32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v1, %v2
// CHECK:   ret <4 x i32> [[MUL_I]]
uint32x4_t test_vmulq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vmulq_u32(v1, v2);
}

// CHECK-LABEL: @test_vmulq_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <4 x float> %v1, %v2
// CHECK:   ret <4 x float> [[MUL_I]]
float32x4_t test_vmulq_f32(float32x4_t v1, float32x4_t v2) {
  return vmulq_f32(v1, v2);
}

// CHECK-LABEL: @test_vmulq_f64(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x double> %v1, %v2
// CHECK:   ret <2 x double> [[MUL_I]]
float64x2_t test_vmulq_f64(float64x2_t v1, float64x2_t v2) {
  return vmulq_f64(v1, v2);
}

// CHECK-LABEL: @test_vmul_p8(
// CHECK:   [[VMUL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.pmul.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VMUL_V_I]]
poly8x8_t test_vmul_p8(poly8x8_t v1, poly8x8_t v2) {
  return vmul_p8(v1, v2);
}

// CHECK-LABEL: @test_vmulq_p8(
// CHECK:   [[VMULQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.pmul.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VMULQ_V_I]]
poly8x16_t test_vmulq_p8(poly8x16_t v1, poly8x16_t v2) {
  return vmulq_p8(v1, v2);
}

// CHECK-LABEL: @test_vmla_s8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, [[MUL_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
int8x8_t test_vmla_s8(int8x8_t v1, int8x8_t v2, int8x8_t v3) {
  return vmla_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_s16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, [[MUL_I]]
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[ADD_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[TMP0]]
int8x8_t test_vmla_s16(int16x4_t v1, int16x4_t v2, int16x4_t v3) {
  return (int8x8_t)vmla_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_s32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, [[MUL_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
int32x2_t test_vmla_s32(int32x2_t v1, int32x2_t v2, int32x2_t v3) {
  return vmla_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x float> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = fadd <2 x float> %v1, [[MUL_I]]
// CHECK:   ret <2 x float> [[ADD_I]]
float32x2_t test_vmla_f32(float32x2_t v1, float32x2_t v2, float32x2_t v3) {
  return vmla_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_u8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, [[MUL_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
uint8x8_t test_vmla_u8(uint8x8_t v1, uint8x8_t v2, uint8x8_t v3) {
  return vmla_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_u16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, [[MUL_I]]
// CHECK:   ret <4 x i16> [[ADD_I]]
uint16x4_t test_vmla_u16(uint16x4_t v1, uint16x4_t v2, uint16x4_t v3) {
  return vmla_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmla_u32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, [[MUL_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
uint32x2_t test_vmla_u32(uint32x2_t v1, uint32x2_t v2, uint32x2_t v3) {
  return vmla_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_s8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, [[MUL_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
int8x16_t test_vmlaq_s8(int8x16_t v1, int8x16_t v2, int8x16_t v3) {
  return vmlaq_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_s16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, [[MUL_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vmlaq_s16(int16x8_t v1, int16x8_t v2, int16x8_t v3) {
  return vmlaq_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_s32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, [[MUL_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vmlaq_s32(int32x4_t v1, int32x4_t v2, int32x4_t v3) {
  return vmlaq_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <4 x float> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = fadd <4 x float> %v1, [[MUL_I]]
// CHECK:   ret <4 x float> [[ADD_I]]
float32x4_t test_vmlaq_f32(float32x4_t v1, float32x4_t v2, float32x4_t v3) {
  return vmlaq_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_u8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, [[MUL_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
uint8x16_t test_vmlaq_u8(uint8x16_t v1, uint8x16_t v2, uint8x16_t v3) {
  return vmlaq_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_u16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, [[MUL_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vmlaq_u16(uint16x8_t v1, uint16x8_t v2, uint16x8_t v3) {
  return vmlaq_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_u32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, [[MUL_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vmlaq_u32(uint32x4_t v1, uint32x4_t v2, uint32x4_t v3) {
  return vmlaq_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlaq_f64(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x double> %v2, %v3
// CHECK:   [[ADD_I:%.*]] = fadd <2 x double> %v1, [[MUL_I]]
// CHECK:   ret <2 x double> [[ADD_I]]
float64x2_t test_vmlaq_f64(float64x2_t v1, float64x2_t v2, float64x2_t v3) {
  return vmlaq_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_s8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <8 x i8> %v1, [[MUL_I]]
// CHECK:   ret <8 x i8> [[SUB_I]]
int8x8_t test_vmls_s8(int8x8_t v1, int8x8_t v2, int8x8_t v3) {
  return vmls_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_s16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <4 x i16> %v1, [[MUL_I]]
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> [[SUB_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[TMP0]]
int8x8_t test_vmls_s16(int16x4_t v1, int16x4_t v2, int16x4_t v3) {
  return (int8x8_t)vmls_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_s32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <2 x i32> %v1, [[MUL_I]]
// CHECK:   ret <2 x i32> [[SUB_I]]
int32x2_t test_vmls_s32(int32x2_t v1, int32x2_t v2, int32x2_t v3) {
  return vmls_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x float> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = fsub <2 x float> %v1, [[MUL_I]]
// CHECK:   ret <2 x float> [[SUB_I]]
float32x2_t test_vmls_f32(float32x2_t v1, float32x2_t v2, float32x2_t v3) {
  return vmls_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_u8(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i8> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <8 x i8> %v1, [[MUL_I]]
// CHECK:   ret <8 x i8> [[SUB_I]]
uint8x8_t test_vmls_u8(uint8x8_t v1, uint8x8_t v2, uint8x8_t v3) {
  return vmls_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_u16(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i16> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <4 x i16> %v1, [[MUL_I]]
// CHECK:   ret <4 x i16> [[SUB_I]]
uint16x4_t test_vmls_u16(uint16x4_t v1, uint16x4_t v2, uint16x4_t v3) {
  return vmls_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmls_u32(
// CHECK:   [[MUL_I:%.*]] = mul <2 x i32> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <2 x i32> %v1, [[MUL_I]]
// CHECK:   ret <2 x i32> [[SUB_I]]
uint32x2_t test_vmls_u32(uint32x2_t v1, uint32x2_t v2, uint32x2_t v3) {
  return vmls_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_s8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <16 x i8> %v1, [[MUL_I]]
// CHECK:   ret <16 x i8> [[SUB_I]]
int8x16_t test_vmlsq_s8(int8x16_t v1, int8x16_t v2, int8x16_t v3) {
  return vmlsq_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_s16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <8 x i16> %v1, [[MUL_I]]
// CHECK:   ret <8 x i16> [[SUB_I]]
int16x8_t test_vmlsq_s16(int16x8_t v1, int16x8_t v2, int16x8_t v3) {
  return vmlsq_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_s32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <4 x i32> %v1, [[MUL_I]]
// CHECK:   ret <4 x i32> [[SUB_I]]
int32x4_t test_vmlsq_s32(int32x4_t v1, int32x4_t v2, int32x4_t v3) {
  return vmlsq_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_f32(
// CHECK:   [[MUL_I:%.*]] = fmul <4 x float> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = fsub <4 x float> %v1, [[MUL_I]]
// CHECK:   ret <4 x float> [[SUB_I]]
float32x4_t test_vmlsq_f32(float32x4_t v1, float32x4_t v2, float32x4_t v3) {
  return vmlsq_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_u8(
// CHECK:   [[MUL_I:%.*]] = mul <16 x i8> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <16 x i8> %v1, [[MUL_I]]
// CHECK:   ret <16 x i8> [[SUB_I]]
uint8x16_t test_vmlsq_u8(uint8x16_t v1, uint8x16_t v2, uint8x16_t v3) {
  return vmlsq_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_u16(
// CHECK:   [[MUL_I:%.*]] = mul <8 x i16> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <8 x i16> %v1, [[MUL_I]]
// CHECK:   ret <8 x i16> [[SUB_I]]
uint16x8_t test_vmlsq_u16(uint16x8_t v1, uint16x8_t v2, uint16x8_t v3) {
  return vmlsq_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_u32(
// CHECK:   [[MUL_I:%.*]] = mul <4 x i32> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = sub <4 x i32> %v1, [[MUL_I]]
// CHECK:   ret <4 x i32> [[SUB_I]]
uint32x4_t test_vmlsq_u32(uint32x4_t v1, uint32x4_t v2, uint32x4_t v3) {
  return vmlsq_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vmlsq_f64(
// CHECK:   [[MUL_I:%.*]] = fmul <2 x double> %v2, %v3
// CHECK:   [[SUB_I:%.*]] = fsub <2 x double> %v1, [[MUL_I]]
// CHECK:   ret <2 x double> [[SUB_I]]
float64x2_t test_vmlsq_f64(float64x2_t v1, float64x2_t v2, float64x2_t v3) {
  return vmlsq_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vfma_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %v3 to <8 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x float> @llvm.fma.v2f32(<2 x float> %v2, <2 x float> %v3, <2 x float> %v1)
// CHECK:   ret <2 x float> [[TMP3]]
float32x2_t test_vfma_f32(float32x2_t v1, float32x2_t v2, float32x2_t v3) {
  return vfma_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vfmaq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %v3 to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <4 x float> @llvm.fma.v4f32(<4 x float> %v2, <4 x float> %v3, <4 x float> %v1)
// CHECK:   ret <4 x float> [[TMP3]]
float32x4_t test_vfmaq_f32(float32x4_t v1, float32x4_t v2, float32x4_t v3) {
  return vfmaq_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vfmaq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x double> %v3 to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x double> @llvm.fma.v2f64(<2 x double> %v2, <2 x double> %v3, <2 x double> %v1)
// CHECK:   ret <2 x double> [[TMP3]]
float64x2_t test_vfmaq_f64(float64x2_t v1, float64x2_t v2, float64x2_t v3) {
  return vfmaq_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vfms_f32(
// CHECK:   [[SUB_I:%.*]] = fneg <2 x float> %v2
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> [[SUB_I]] to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %v3 to <8 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x float> @llvm.fma.v2f32(<2 x float> [[SUB_I]], <2 x float> %v3, <2 x float> %v1)
// CHECK:   ret <2 x float> [[TMP3]]
float32x2_t test_vfms_f32(float32x2_t v1, float32x2_t v2, float32x2_t v3) {
  return vfms_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vfmsq_f32(
// CHECK:   [[SUB_I:%.*]] = fneg <4 x float> %v2
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> [[SUB_I]] to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %v3 to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <4 x float> @llvm.fma.v4f32(<4 x float> [[SUB_I]], <4 x float> %v3, <4 x float> %v1)
// CHECK:   ret <4 x float> [[TMP3]]
float32x4_t test_vfmsq_f32(float32x4_t v1, float32x4_t v2, float32x4_t v3) {
  return vfmsq_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vfmsq_f64(
// CHECK:   [[SUB_I:%.*]] = fneg <2 x double> %v2
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> [[SUB_I]] to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x double> %v3 to <16 x i8>
// CHECK:   [[TMP3:%.*]] = call <2 x double> @llvm.fma.v2f64(<2 x double> [[SUB_I]], <2 x double> %v3, <2 x double> %v1)
// CHECK:   ret <2 x double> [[TMP3]]
float64x2_t test_vfmsq_f64(float64x2_t v1, float64x2_t v2, float64x2_t v3) {
  return vfmsq_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vdivq_f64(
// CHECK:   [[DIV_I:%.*]] = fdiv <2 x double> %v1, %v2
// CHECK:   ret <2 x double> [[DIV_I]]
float64x2_t test_vdivq_f64(float64x2_t v1, float64x2_t v2) {
  return vdivq_f64(v1, v2);
}

// CHECK-LABEL: @test_vdivq_f32(
// CHECK:   [[DIV_I:%.*]] = fdiv <4 x float> %v1, %v2
// CHECK:   ret <4 x float> [[DIV_I]]
float32x4_t test_vdivq_f32(float32x4_t v1, float32x4_t v2) {
  return vdivq_f32(v1, v2);
}

// CHECK-LABEL: @test_vdiv_f32(
// CHECK:   [[DIV_I:%.*]] = fdiv <2 x float> %v1, %v2
// CHECK:   ret <2 x float> [[DIV_I]]
float32x2_t test_vdiv_f32(float32x2_t v1, float32x2_t v2) {
  return vdiv_f32(v1, v2);
}

// CHECK-LABEL: @test_vaba_s8(
// CHECK:   [[VABD_I_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sabd.v8i8(<8 x i8> %v2, <8 x i8> %v3)
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, [[VABD_I_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
int8x8_t test_vaba_s8(int8x8_t v1, int8x8_t v2, int8x8_t v3) {
  return vaba_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vaba_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v3 to <8 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sabd.v4i16(<4 x i16> %v2, <4 x i16> %v3)
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, [[VABD2_I_I]]
// CHECK:   ret <4 x i16> [[ADD_I]]
int16x4_t test_vaba_s16(int16x4_t v1, int16x4_t v2, int16x4_t v3) {
  return vaba_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vaba_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v3 to <8 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sabd.v2i32(<2 x i32> %v2, <2 x i32> %v3)
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, [[VABD2_I_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
int32x2_t test_vaba_s32(int32x2_t v1, int32x2_t v2, int32x2_t v3) {
  return vaba_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vaba_u8(
// CHECK:   [[VABD_I_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uabd.v8i8(<8 x i8> %v2, <8 x i8> %v3)
// CHECK:   [[ADD_I:%.*]] = add <8 x i8> %v1, [[VABD_I_I]]
// CHECK:   ret <8 x i8> [[ADD_I]]
uint8x8_t test_vaba_u8(uint8x8_t v1, uint8x8_t v2, uint8x8_t v3) {
  return vaba_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vaba_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v3 to <8 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uabd.v4i16(<4 x i16> %v2, <4 x i16> %v3)
// CHECK:   [[ADD_I:%.*]] = add <4 x i16> %v1, [[VABD2_I_I]]
// CHECK:   ret <4 x i16> [[ADD_I]]
uint16x4_t test_vaba_u16(uint16x4_t v1, uint16x4_t v2, uint16x4_t v3) {
  return vaba_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vaba_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v3 to <8 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uabd.v2i32(<2 x i32> %v2, <2 x i32> %v3)
// CHECK:   [[ADD_I:%.*]] = add <2 x i32> %v1, [[VABD2_I_I]]
// CHECK:   ret <2 x i32> [[ADD_I]]
uint32x2_t test_vaba_u32(uint32x2_t v1, uint32x2_t v2, uint32x2_t v3) {
  return vaba_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_s8(
// CHECK:   [[VABD_I_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sabd.v16i8(<16 x i8> %v2, <16 x i8> %v3)
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, [[VABD_I_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
int8x16_t test_vabaq_s8(int8x16_t v1, int8x16_t v2, int8x16_t v3) {
  return vabaq_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v3 to <16 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sabd.v8i16(<8 x i16> %v2, <8 x i16> %v3)
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, [[VABD2_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
int16x8_t test_vabaq_s16(int16x8_t v1, int16x8_t v2, int16x8_t v3) {
  return vabaq_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v3 to <16 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sabd.v4i32(<4 x i32> %v2, <4 x i32> %v3)
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, [[VABD2_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
int32x4_t test_vabaq_s32(int32x4_t v1, int32x4_t v2, int32x4_t v3) {
  return vabaq_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_u8(
// CHECK:   [[VABD_I_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uabd.v16i8(<16 x i8> %v2, <16 x i8> %v3)
// CHECK:   [[ADD_I:%.*]] = add <16 x i8> %v1, [[VABD_I_I]]
// CHECK:   ret <16 x i8> [[ADD_I]]
uint8x16_t test_vabaq_u8(uint8x16_t v1, uint8x16_t v2, uint8x16_t v3) {
  return vabaq_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v3 to <16 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uabd.v8i16(<8 x i16> %v2, <8 x i16> %v3)
// CHECK:   [[ADD_I:%.*]] = add <8 x i16> %v1, [[VABD2_I_I]]
// CHECK:   ret <8 x i16> [[ADD_I]]
uint16x8_t test_vabaq_u16(uint16x8_t v1, uint16x8_t v2, uint16x8_t v3) {
  return vabaq_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vabaq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v3 to <16 x i8>
// CHECK:   [[VABD2_I_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uabd.v4i32(<4 x i32> %v2, <4 x i32> %v3)
// CHECK:   [[ADD_I:%.*]] = add <4 x i32> %v1, [[VABD2_I_I]]
// CHECK:   ret <4 x i32> [[ADD_I]]
uint32x4_t test_vabaq_u32(uint32x4_t v1, uint32x4_t v2, uint32x4_t v3) {
  return vabaq_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vabd_s8(
// CHECK:   [[VABD_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sabd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VABD_I]]
int8x8_t test_vabd_s8(int8x8_t v1, int8x8_t v2) {
  return vabd_s8(v1, v2);
}

// CHECK-LABEL: @test_vabd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sabd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   ret <4 x i16> [[VABD2_I]]
int16x4_t test_vabd_s16(int16x4_t v1, int16x4_t v2) {
  return vabd_s16(v1, v2);
}

// CHECK-LABEL: @test_vabd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sabd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   ret <2 x i32> [[VABD2_I]]
int32x2_t test_vabd_s32(int32x2_t v1, int32x2_t v2) {
  return vabd_s32(v1, v2);
}

// CHECK-LABEL: @test_vabd_u8(
// CHECK:   [[VABD_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uabd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VABD_I]]
uint8x8_t test_vabd_u8(uint8x8_t v1, uint8x8_t v2) {
  return vabd_u8(v1, v2);
}

// CHECK-LABEL: @test_vabd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uabd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   ret <4 x i16> [[VABD2_I]]
uint16x4_t test_vabd_u16(uint16x4_t v1, uint16x4_t v2) {
  return vabd_u16(v1, v2);
}

// CHECK-LABEL: @test_vabd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uabd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   ret <2 x i32> [[VABD2_I]]
uint32x2_t test_vabd_u32(uint32x2_t v1, uint32x2_t v2) {
  return vabd_u32(v1, v2);
}

// CHECK-LABEL: @test_vabd_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fabd.v2f32(<2 x float> %v1, <2 x float> %v2)
// CHECK:   ret <2 x float> [[VABD2_I]]
float32x2_t test_vabd_f32(float32x2_t v1, float32x2_t v2) {
  return vabd_f32(v1, v2);
}

// CHECK-LABEL: @test_vabdq_s8(
// CHECK:   [[VABD_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sabd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VABD_I]]
int8x16_t test_vabdq_s8(int8x16_t v1, int8x16_t v2) {
  return vabdq_s8(v1, v2);
}

// CHECK-LABEL: @test_vabdq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sabd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   ret <8 x i16> [[VABD2_I]]
int16x8_t test_vabdq_s16(int16x8_t v1, int16x8_t v2) {
  return vabdq_s16(v1, v2);
}

// CHECK-LABEL: @test_vabdq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sabd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   ret <4 x i32> [[VABD2_I]]
int32x4_t test_vabdq_s32(int32x4_t v1, int32x4_t v2) {
  return vabdq_s32(v1, v2);
}

// CHECK-LABEL: @test_vabdq_u8(
// CHECK:   [[VABD_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uabd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VABD_I]]
uint8x16_t test_vabdq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vabdq_u8(v1, v2);
}

// CHECK-LABEL: @test_vabdq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uabd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   ret <8 x i16> [[VABD2_I]]
uint16x8_t test_vabdq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vabdq_u16(v1, v2);
}

// CHECK-LABEL: @test_vabdq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uabd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   ret <4 x i32> [[VABD2_I]]
uint32x4_t test_vabdq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vabdq_u32(v1, v2);
}

// CHECK-LABEL: @test_vabdq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fabd.v4f32(<4 x float> %v1, <4 x float> %v2)
// CHECK:   ret <4 x float> [[VABD2_I]]
float32x4_t test_vabdq_f32(float32x4_t v1, float32x4_t v2) {
  return vabdq_f32(v1, v2);
}

// CHECK-LABEL: @test_vabdq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VABD2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fabd.v2f64(<2 x double> %v1, <2 x double> %v2)
// CHECK:   ret <2 x double> [[VABD2_I]]
float64x2_t test_vabdq_f64(float64x2_t v1, float64x2_t v2) {
  return vabdq_f64(v1, v2);
}

// CHECK-LABEL: @test_vbsl_s8(
// CHECK:   [[VBSL_I:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <8 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <8 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <8 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <8 x i8> [[VBSL2_I]]
int8x8_t test_vbsl_s8(uint8x8_t v1, int8x8_t v2, int8x8_t v3) {
  return vbsl_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <4 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   [[TMP4:%.*]] = bitcast <4 x i16> [[VBSL5_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[TMP4]]
int8x8_t test_vbsl_s16(uint16x4_t v1, int16x4_t v2, int16x4_t v3) {
  return (int8x8_t)vbsl_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <2 x i32> %v1, <i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i32> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <2 x i32> [[VBSL5_I]]
int32x2_t test_vbsl_s32(uint32x2_t v1, int32x2_t v2, int32x2_t v3) {
  return vbsl_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <1 x i64> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <1 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <1 x i64> %v1, <i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <1 x i64> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <1 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <1 x i64> [[VBSL5_I]]
int64x1_t test_vbsl_s64(uint64x1_t v1, int64x1_t v2, int64x1_t v3) {
  return vbsl_s64(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_u8(
// CHECK:   [[VBSL_I:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <8 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <8 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <8 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <8 x i8> [[VBSL2_I]]
uint8x8_t test_vbsl_u8(uint8x8_t v1, uint8x8_t v2, uint8x8_t v3) {
  return vbsl_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <4 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <4 x i16> [[VBSL5_I]]
uint16x4_t test_vbsl_u16(uint16x4_t v1, uint16x4_t v2, uint16x4_t v3) {
  return vbsl_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i32> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <2 x i32> %v1, <i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i32> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <2 x i32> [[VBSL5_I]]
uint32x2_t test_vbsl_u32(uint32x2_t v1, uint32x2_t v2, uint32x2_t v3) {
  return vbsl_u32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <1 x i64> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <1 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <1 x i64> %v1, <i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <1 x i64> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <1 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <1 x i64> [[VBSL5_I]]
uint64x1_t test_vbsl_u64(uint64x1_t v1, uint64x1_t v2, uint64x1_t v3) {
  return vbsl_u64(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_f32(
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[TMP3:%.*]] = bitcast <2 x float> %v3 to <8 x i8>
// CHECK:   [[VBSL1_I:%.*]] = bitcast <8 x i8> [[TMP2]] to <2 x i32>
// CHECK:   [[VBSL2_I:%.*]] = bitcast <8 x i8> [[TMP3]] to <2 x i32>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i32> %v1, [[VBSL1_I]]
// CHECK:   [[TMP4:%.*]] = xor <2 x i32> %v1, <i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i32> [[TMP4]], [[VBSL2_I]]
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   [[TMP5:%.*]] = bitcast <2 x i32> [[VBSL5_I]] to <2 x float>
// CHECK:   ret <2 x float> [[TMP5]]
float32x2_t test_vbsl_f32(uint32x2_t v1, float32x2_t v2, float32x2_t v3) {
  return vbsl_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x double> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <1 x double> %v3 to <8 x i8>
// CHECK:   [[VBSL1_I:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[VBSL2_I:%.*]] = bitcast <8 x i8> [[TMP2]] to <1 x i64>
// CHECK:   [[VBSL3_I:%.*]] = and <1 x i64> %v1, [[VBSL1_I]]
// CHECK:   [[TMP3:%.*]] = xor <1 x i64> %v1, <i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <1 x i64> [[TMP3]], [[VBSL2_I]]
// CHECK:   [[VBSL5_I:%.*]] = or <1 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   [[TMP4:%.*]] = bitcast <1 x i64> [[VBSL5_I]] to <1 x double>
// CHECK:   ret <1 x double> [[TMP4]]
float64x1_t test_vbsl_f64(uint64x1_t v1, float64x1_t v2, float64x1_t v3) {
  return vbsl_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_p8(
// CHECK:   [[VBSL_I:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <8 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <8 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <8 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <8 x i8> [[VBSL2_I]]
poly8x8_t test_vbsl_p8(uint8x8_t v1, poly8x8_t v2, poly8x8_t v3) {
  return vbsl_p8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbsl_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i16> %v3 to <8 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <4 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <4 x i16> [[VBSL5_I]]
poly16x4_t test_vbsl_p16(uint16x4_t v1, poly16x4_t v2, poly16x4_t v3) {
  return vbsl_p16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_s8(
// CHECK:   [[VBSL_I:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <16 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <16 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <16 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <16 x i8> [[VBSL2_I]]
int8x16_t test_vbslq_s8(uint8x16_t v1, int8x16_t v2, int8x16_t v3) {
  return vbslq_s8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <8 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <8 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <8 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <8 x i16> [[VBSL5_I]]
int16x8_t test_vbslq_s16(uint16x8_t v1, int16x8_t v2, int16x8_t v3) {
  return vbslq_s16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i32> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <4 x i32> %v1, <i32 -1, i32 -1, i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i32> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <4 x i32> [[VBSL5_I]]
int32x4_t test_vbslq_s32(uint32x4_t v1, int32x4_t v2, int32x4_t v3) {
  return vbslq_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i64> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <2 x i64> %v1, <i64 -1, i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i64> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <2 x i64> [[VBSL5_I]]
int64x2_t test_vbslq_s64(uint64x2_t v1, int64x2_t v2, int64x2_t v3) {
  return vbslq_s64(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_u8(
// CHECK:   [[VBSL_I:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <16 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <16 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <16 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <16 x i8> [[VBSL2_I]]
uint8x16_t test_vbslq_u8(uint8x16_t v1, uint8x16_t v2, uint8x16_t v3) {
  return vbslq_u8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <8 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <8 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <8 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <8 x i16> [[VBSL5_I]]
uint16x8_t test_vbslq_u16(uint16x8_t v1, uint16x8_t v2, uint16x8_t v3) {
  return vbslq_u16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x i32> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <4 x i32> %v1, <i32 -1, i32 -1, i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i32> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <4 x i32> [[VBSL5_I]]
int32x4_t test_vbslq_u32(uint32x4_t v1, int32x4_t v2, int32x4_t v3) {
  return vbslq_s32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x i64> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <2 x i64> %v1, <i64 -1, i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i64> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <2 x i64> [[VBSL5_I]]
uint64x2_t test_vbslq_u64(uint64x2_t v1, uint64x2_t v2, uint64x2_t v3) {
  return vbslq_u64(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <4 x float> %v3 to <16 x i8>
// CHECK:   [[VBSL1_I:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VBSL2_I:%.*]] = bitcast <16 x i8> [[TMP2]] to <4 x i32>
// CHECK:   [[VBSL3_I:%.*]] = and <4 x i32> %v1, [[VBSL1_I]]
// CHECK:   [[TMP3:%.*]] = xor <4 x i32> %v1, <i32 -1, i32 -1, i32 -1, i32 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <4 x i32> [[TMP3]], [[VBSL2_I]]
// CHECK:   [[VBSL5_I:%.*]] = or <4 x i32> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   [[TMP4:%.*]] = bitcast <4 x i32> [[VBSL5_I]] to <4 x float>
// CHECK:   ret <4 x float> [[TMP4]]
float32x4_t test_vbslq_f32(uint32x4_t v1, float32x4_t v2, float32x4_t v3) {
  return vbslq_f32(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_p8(
// CHECK:   [[VBSL_I:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP0:%.*]] = xor <16 x i8> %v1, <i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1, i8 -1>
// CHECK:   [[VBSL1_I:%.*]] = and <16 x i8> [[TMP0]], %v3
// CHECK:   [[VBSL2_I:%.*]] = or <16 x i8> [[VBSL_I]], [[VBSL1_I]]
// CHECK:   ret <16 x i8> [[VBSL2_I]]
poly8x16_t test_vbslq_p8(uint8x16_t v1, poly8x16_t v2, poly8x16_t v3) {
  return vbslq_p8(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i16> %v3 to <16 x i8>
// CHECK:   [[VBSL3_I:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = xor <8 x i16> %v1, <i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1, i16 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <8 x i16> [[TMP3]], %v3
// CHECK:   [[VBSL5_I:%.*]] = or <8 x i16> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   ret <8 x i16> [[VBSL5_I]]
poly16x8_t test_vbslq_p16(uint16x8_t v1, poly16x8_t v2, poly16x8_t v3) {
  return vbslq_p16(v1, v2, v3);
}

// CHECK-LABEL: @test_vbslq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <2 x double> %v3 to <16 x i8>
// CHECK:   [[VBSL1_I:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VBSL2_I:%.*]] = bitcast <16 x i8> [[TMP2]] to <2 x i64>
// CHECK:   [[VBSL3_I:%.*]] = and <2 x i64> %v1, [[VBSL1_I]]
// CHECK:   [[TMP3:%.*]] = xor <2 x i64> %v1, <i64 -1, i64 -1>
// CHECK:   [[VBSL4_I:%.*]] = and <2 x i64> [[TMP3]], [[VBSL2_I]]
// CHECK:   [[VBSL5_I:%.*]] = or <2 x i64> [[VBSL3_I]], [[VBSL4_I]]
// CHECK:   [[TMP4:%.*]] = bitcast <2 x i64> [[VBSL5_I]] to <2 x double>
// CHECK:   ret <2 x double> [[TMP4]]
float64x2_t test_vbslq_f64(uint64x2_t v1, float64x2_t v2, float64x2_t v3) {
  return vbslq_f64(v1, v2, v3);
}

// CHECK-LABEL: @test_vrecps_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VRECPS_V2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.frecps.v2f32(<2 x float> %v1, <2 x float> %v2)
// CHECK:   ret <2 x float> [[VRECPS_V2_I]]
float32x2_t test_vrecps_f32(float32x2_t v1, float32x2_t v2) {
  return vrecps_f32(v1, v2);
}

// CHECK-LABEL: @test_vrecpsq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VRECPSQ_V2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.frecps.v4f32(<4 x float> %v1, <4 x float> %v2)
// CHECK:   [[VRECPSQ_V3_I:%.*]] = bitcast <4 x float> [[VRECPSQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x float> [[VRECPSQ_V2_I]]
float32x4_t test_vrecpsq_f32(float32x4_t v1, float32x4_t v2) {
  return vrecpsq_f32(v1, v2);
}

// CHECK-LABEL: @test_vrecpsq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VRECPSQ_V2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.frecps.v2f64(<2 x double> %v1, <2 x double> %v2)
// CHECK:   [[VRECPSQ_V3_I:%.*]] = bitcast <2 x double> [[VRECPSQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x double> [[VRECPSQ_V2_I]]
float64x2_t test_vrecpsq_f64(float64x2_t v1, float64x2_t v2) {
  return vrecpsq_f64(v1, v2);
}

// CHECK-LABEL: @test_vrsqrts_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VRSQRTS_V2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.frsqrts.v2f32(<2 x float> %v1, <2 x float> %v2)
// CHECK:   [[VRSQRTS_V3_I:%.*]] = bitcast <2 x float> [[VRSQRTS_V2_I]] to <8 x i8>
// CHECK:   ret <2 x float> [[VRSQRTS_V2_I]]
float32x2_t test_vrsqrts_f32(float32x2_t v1, float32x2_t v2) {
  return vrsqrts_f32(v1, v2);
}

// CHECK-LABEL: @test_vrsqrtsq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VRSQRTSQ_V2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.frsqrts.v4f32(<4 x float> %v1, <4 x float> %v2)
// CHECK:   [[VRSQRTSQ_V3_I:%.*]] = bitcast <4 x float> [[VRSQRTSQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x float> [[VRSQRTSQ_V2_I]]
float32x4_t test_vrsqrtsq_f32(float32x4_t v1, float32x4_t v2) {
  return vrsqrtsq_f32(v1, v2);
}

// CHECK-LABEL: @test_vrsqrtsq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VRSQRTSQ_V2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.frsqrts.v2f64(<2 x double> %v1, <2 x double> %v2)
// CHECK:   [[VRSQRTSQ_V3_I:%.*]] = bitcast <2 x double> [[VRSQRTSQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x double> [[VRSQRTSQ_V2_I]]
float64x2_t test_vrsqrtsq_f64(float64x2_t v1, float64x2_t v2) {
  return vrsqrtsq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcage_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VCAGE_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.facge.v2i32.v2f32(<2 x float> %v1, <2 x float> %v2)
// CHECK:   ret <2 x i32> [[VCAGE_V2_I]]
uint32x2_t test_vcage_f32(float32x2_t v1, float32x2_t v2) {
  return vcage_f32(v1, v2);
}

// CHECK-LABEL: @test_vcage_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x double> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x double> %b to <8 x i8>
// CHECK:   [[VCAGE_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.facge.v1i64.v1f64(<1 x double> %a, <1 x double> %b)
// CHECK:   ret <1 x i64> [[VCAGE_V2_I]]
uint64x1_t test_vcage_f64(float64x1_t a, float64x1_t b) {
  return vcage_f64(a, b);
}

// CHECK-LABEL: @test_vcageq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VCAGEQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.facge.v4i32.v4f32(<4 x float> %v1, <4 x float> %v2)
// CHECK:   ret <4 x i32> [[VCAGEQ_V2_I]]
uint32x4_t test_vcageq_f32(float32x4_t v1, float32x4_t v2) {
  return vcageq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcageq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VCAGEQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.facge.v2i64.v2f64(<2 x double> %v1, <2 x double> %v2)
// CHECK:   ret <2 x i64> [[VCAGEQ_V2_I]]
uint64x2_t test_vcageq_f64(float64x2_t v1, float64x2_t v2) {
  return vcageq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcagt_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VCAGT_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.facgt.v2i32.v2f32(<2 x float> %v1, <2 x float> %v2)
// CHECK:   ret <2 x i32> [[VCAGT_V2_I]]
uint32x2_t test_vcagt_f32(float32x2_t v1, float32x2_t v2) {
  return vcagt_f32(v1, v2);
}

// CHECK-LABEL: @test_vcagt_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x double> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x double> %b to <8 x i8>
// CHECK:   [[VCAGT_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.facgt.v1i64.v1f64(<1 x double> %a, <1 x double> %b)
// CHECK:   ret <1 x i64> [[VCAGT_V2_I]]
uint64x1_t test_vcagt_f64(float64x1_t a, float64x1_t b) {
  return vcagt_f64(a, b);
}

// CHECK-LABEL: @test_vcagtq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VCAGTQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.facgt.v4i32.v4f32(<4 x float> %v1, <4 x float> %v2)
// CHECK:   ret <4 x i32> [[VCAGTQ_V2_I]]
uint32x4_t test_vcagtq_f32(float32x4_t v1, float32x4_t v2) {
  return vcagtq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcagtq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VCAGTQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.facgt.v2i64.v2f64(<2 x double> %v1, <2 x double> %v2)
// CHECK:   ret <2 x i64> [[VCAGTQ_V2_I]]
uint64x2_t test_vcagtq_f64(float64x2_t v1, float64x2_t v2) {
  return vcagtq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcale_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VCALE_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.facge.v2i32.v2f32(<2 x float> %v2, <2 x float> %v1)
// CHECK:   ret <2 x i32> [[VCALE_V2_I]]
uint32x2_t test_vcale_f32(float32x2_t v1, float32x2_t v2) {
  return vcale_f32(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vcale_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x double> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x double> %b to <8 x i8>
// CHECK:   [[VCALE_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.facge.v1i64.v1f64(<1 x double> %b, <1 x double> %a)
// CHECK:   ret <1 x i64> [[VCALE_V2_I]]
uint64x1_t test_vcale_f64(float64x1_t a, float64x1_t b) {
  return vcale_f64(a, b);
}

// CHECK-LABEL: @test_vcaleq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VCALEQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.facge.v4i32.v4f32(<4 x float> %v2, <4 x float> %v1)
// CHECK:   ret <4 x i32> [[VCALEQ_V2_I]]
uint32x4_t test_vcaleq_f32(float32x4_t v1, float32x4_t v2) {
  return vcaleq_f32(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vcaleq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VCALEQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.facge.v2i64.v2f64(<2 x double> %v2, <2 x double> %v1)
// CHECK:   ret <2 x i64> [[VCALEQ_V2_I]]
uint64x2_t test_vcaleq_f64(float64x2_t v1, float64x2_t v2) {
  return vcaleq_f64(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vcalt_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %v2 to <8 x i8>
// CHECK:   [[VCALT_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.facgt.v2i32.v2f32(<2 x float> %v2, <2 x float> %v1)
// CHECK:   ret <2 x i32> [[VCALT_V2_I]]
uint32x2_t test_vcalt_f32(float32x2_t v1, float32x2_t v2) {
  return vcalt_f32(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vcalt_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x double> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x double> %b to <8 x i8>
// CHECK:   [[VCALT_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.facgt.v1i64.v1f64(<1 x double> %b, <1 x double> %a)
// CHECK:   ret <1 x i64> [[VCALT_V2_I]]
uint64x1_t test_vcalt_f64(float64x1_t a, float64x1_t b) {
  return vcalt_f64(a, b);
}

// CHECK-LABEL: @test_vcaltq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %v2 to <16 x i8>
// CHECK:   [[VCALTQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.facgt.v4i32.v4f32(<4 x float> %v2, <4 x float> %v1)
// CHECK:   ret <4 x i32> [[VCALTQ_V2_I]]
uint32x4_t test_vcaltq_f32(float32x4_t v1, float32x4_t v2) {
  return vcaltq_f32(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vcaltq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %v2 to <16 x i8>
// CHECK:   [[VCALTQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.facgt.v2i64.v2f64(<2 x double> %v2, <2 x double> %v1)
// CHECK:   ret <2 x i64> [[VCALTQ_V2_I]]
uint64x2_t test_vcaltq_f64(float64x2_t v1, float64x2_t v2) {
  return vcaltq_f64(v1, v2);
  // Using registers other than v0, v1 are possible, but would be odd.
}

// CHECK-LABEL: @test_vtst_s8(
// CHECK:   [[TMP0:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <8 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP1]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VTST_I]]
uint8x8_t test_vtst_s8(int8x8_t v1, int8x8_t v2) {
  return vtst_s8(v1, v2);
}

// CHECK-LABEL: @test_vtst_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <4 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <4 x i1> [[TMP3]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VTST_I]]
uint16x4_t test_vtst_s16(int16x4_t v1, int16x4_t v2) {
  return vtst_s16(v1, v2);
}

// CHECK-LABEL: @test_vtst_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <2 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <2 x i32> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <2 x i1> [[TMP3]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VTST_I]]
uint32x2_t test_vtst_s32(int32x2_t v1, int32x2_t v2) {
  return vtst_s32(v1, v2);
}

// CHECK-LABEL: @test_vtst_u8(
// CHECK:   [[TMP0:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <8 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP1]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VTST_I]]
uint8x8_t test_vtst_u8(uint8x8_t v1, uint8x8_t v2) {
  return vtst_u8(v1, v2);
}

// CHECK-LABEL: @test_vtst_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <4 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <4 x i1> [[TMP3]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VTST_I]]
uint16x4_t test_vtst_u16(uint16x4_t v1, uint16x4_t v2) {
  return vtst_u16(v1, v2);
}

// CHECK-LABEL: @test_vtst_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <2 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <2 x i32> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <2 x i1> [[TMP3]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VTST_I]]
uint32x2_t test_vtst_u32(uint32x2_t v1, uint32x2_t v2) {
  return vtst_u32(v1, v2);
}

// CHECK-LABEL: @test_vtstq_s8(
// CHECK:   [[TMP0:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <16 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <16 x i1> [[TMP1]] to <16 x i8>
// CHECK:   ret <16 x i8> [[VTST_I]]
uint8x16_t test_vtstq_s8(int8x16_t v1, int8x16_t v2) {
  return vtstq_s8(v1, v2);
}

// CHECK-LABEL: @test_vtstq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <8 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP3]] to <8 x i16>
// CHECK:   ret <8 x i16> [[VTST_I]]
uint16x8_t test_vtstq_s16(int16x8_t v1, int16x8_t v2) {
  return vtstq_s16(v1, v2);
}

// CHECK-LABEL: @test_vtstq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <4 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <4 x i32> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <4 x i1> [[TMP3]] to <4 x i32>
// CHECK:   ret <4 x i32> [[VTST_I]]
uint32x4_t test_vtstq_s32(int32x4_t v1, int32x4_t v2) {
  return vtstq_s32(v1, v2);
}

// CHECK-LABEL: @test_vtstq_u8(
// CHECK:   [[TMP0:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <16 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <16 x i1> [[TMP1]] to <16 x i8>
// CHECK:   ret <16 x i8> [[VTST_I]]
uint8x16_t test_vtstq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vtstq_u8(v1, v2);
}

// CHECK-LABEL: @test_vtstq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <8 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP3]] to <8 x i16>
// CHECK:   ret <8 x i16> [[VTST_I]]
uint16x8_t test_vtstq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vtstq_u16(v1, v2);
}

// CHECK-LABEL: @test_vtstq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <4 x i32> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <4 x i32> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <4 x i1> [[TMP3]] to <4 x i32>
// CHECK:   ret <4 x i32> [[VTST_I]]
uint32x4_t test_vtstq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vtstq_u32(v1, v2);
}

// CHECK-LABEL: @test_vtstq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <2 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <2 x i64> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <2 x i1> [[TMP3]] to <2 x i64>
// CHECK:   ret <2 x i64> [[VTST_I]]
uint64x2_t test_vtstq_s64(int64x2_t v1, int64x2_t v2) {
  return vtstq_s64(v1, v2);
}

// CHECK-LABEL: @test_vtstq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <2 x i64> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <2 x i64> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <2 x i1> [[TMP3]] to <2 x i64>
// CHECK:   ret <2 x i64> [[VTST_I]]
uint64x2_t test_vtstq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vtstq_u64(v1, v2);
}

// CHECK-LABEL: @test_vtst_p8(
// CHECK:   [[TMP0:%.*]] = and <8 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <8 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP1]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VTST_I]]
uint8x8_t test_vtst_p8(poly8x8_t v1, poly8x8_t v2) {
  return vtst_p8(v1, v2);
}

// CHECK-LABEL: @test_vtst_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <4 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <4 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <4 x i1> [[TMP3]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VTST_I]]
uint16x4_t test_vtst_p16(poly16x4_t v1, poly16x4_t v2) {
  return vtst_p16(v1, v2);
}

// CHECK-LABEL: @test_vtstq_p8(
// CHECK:   [[TMP0:%.*]] = and <16 x i8> %v1, %v2
// CHECK:   [[TMP1:%.*]] = icmp ne <16 x i8> [[TMP0]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <16 x i1> [[TMP1]] to <16 x i8>
// CHECK:   ret <16 x i8> [[VTST_I]]
uint8x16_t test_vtstq_p8(poly8x16_t v1, poly8x16_t v2) {
  return vtstq_p8(v1, v2);
}

// CHECK-LABEL: @test_vtstq_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[TMP2:%.*]] = and <8 x i16> %v1, %v2
// CHECK:   [[TMP3:%.*]] = icmp ne <8 x i16> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <8 x i1> [[TMP3]] to <8 x i16>
// CHECK:   ret <8 x i16> [[VTST_I]]
uint16x8_t test_vtstq_p16(poly16x8_t v1, poly16x8_t v2) {
  return vtstq_p16(v1, v2);
}

// CHECK-LABEL: @test_vtst_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <1 x i64> %a, %b
// CHECK:   [[TMP3:%.*]] = icmp ne <1 x i64> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <1 x i1> [[TMP3]] to <1 x i64>
// CHECK:   ret <1 x i64> [[VTST_I]]
uint64x1_t test_vtst_s64(int64x1_t a, int64x1_t b) {
  return vtst_s64(a, b);
}

// CHECK-LABEL: @test_vtst_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = and <1 x i64> %a, %b
// CHECK:   [[TMP3:%.*]] = icmp ne <1 x i64> [[TMP2]], zeroinitializer
// CHECK:   [[VTST_I:%.*]] = sext <1 x i1> [[TMP3]] to <1 x i64>
// CHECK:   ret <1 x i64> [[VTST_I]]
uint64x1_t test_vtst_u64(uint64x1_t a, uint64x1_t b) {
  return vtst_u64(a, b);
}

// CHECK-LABEL: @test_vceq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_s8(int8x8_t v1, int8x8_t v2) {
  return vceq_s8(v1, v2);
}

// CHECK-LABEL: @test_vceq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vceq_s16(int16x4_t v1, int16x4_t v2) {
  return vceq_s16(v1, v2);
}

// CHECK-LABEL: @test_vceq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_s32(int32x2_t v1, int32x2_t v2) {
  return vceq_s32(v1, v2);
}

// CHECK-LABEL: @test_vceq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp eq <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vceq_s64(int64x1_t a, int64x1_t b) {
  return vceq_s64(a, b);
}

// CHECK-LABEL: @test_vceq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp eq <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vceq_u64(uint64x1_t a, uint64x1_t b) {
  return vceq_u64(a, b);
}

// CHECK-LABEL: @test_vceq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <2 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_f32(float32x2_t v1, float32x2_t v2) {
  return vceq_f32(v1, v2);
}

// CHECK-LABEL: @test_vceq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <1 x double> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vceq_f64(float64x1_t a, float64x1_t b) {
  return vceq_f64(a, b);
}

// CHECK-LABEL: @test_vceq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_u8(uint8x8_t v1, uint8x8_t v2) {
  return vceq_u8(v1, v2);
}

// CHECK-LABEL: @test_vceq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vceq_u16(uint16x4_t v1, uint16x4_t v2) {
  return vceq_u16(v1, v2);
}

// CHECK-LABEL: @test_vceq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vceq_u32(uint32x2_t v1, uint32x2_t v2) {
  return vceq_u32(v1, v2);
}

// CHECK-LABEL: @test_vceq_p8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vceq_p8(poly8x8_t v1, poly8x8_t v2) {
  return vceq_p8(v1, v2);
}

// CHECK-LABEL: @test_vceqq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_s8(int8x16_t v1, int8x16_t v2) {
  return vceqq_s8(v1, v2);
}

// CHECK-LABEL: @test_vceqq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vceqq_s16(int16x8_t v1, int16x8_t v2) {
  return vceqq_s16(v1, v2);
}

// CHECK-LABEL: @test_vceqq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_s32(int32x4_t v1, int32x4_t v2) {
  return vceqq_s32(v1, v2);
}

// CHECK-LABEL: @test_vceqq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <4 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_f32(float32x4_t v1, float32x4_t v2) {
  return vceqq_f32(v1, v2);
}

// CHECK-LABEL: @test_vceqq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vceqq_u8(v1, v2);
}

// CHECK-LABEL: @test_vceqq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp eq <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vceqq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vceqq_u16(v1, v2);
}

// CHECK-LABEL: @test_vceqq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp eq <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vceqq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vceqq_u32(v1, v2);
}

// CHECK-LABEL: @test_vceqq_p8(
// CHECK:   [[CMP_I:%.*]] = icmp eq <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vceqq_p8(poly8x16_t v1, poly8x16_t v2) {
  return vceqq_p8(v1, v2);
}

// CHECK-LABEL: @test_vceqq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vceqq_s64(int64x2_t v1, int64x2_t v2) {
  return vceqq_s64(v1, v2);
}

// CHECK-LABEL: @test_vceqq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp eq <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vceqq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vceqq_u64(v1, v2);
}

// CHECK-LABEL: @test_vceqq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp oeq <2 x double> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vceqq_f64(float64x2_t v1, float64x2_t v2) {
  return vceqq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcge_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sge <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcge_s8(int8x8_t v1, int8x8_t v2) {
  return vcge_s8(v1, v2);
}

// CHECK-LABEL: @test_vcge_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sge <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcge_s16(int16x4_t v1, int16x4_t v2) {
  return vcge_s16(v1, v2);
}

// CHECK-LABEL: @test_vcge_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sge <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_s32(int32x2_t v1, int32x2_t v2) {
  return vcge_s32(v1, v2);
}

// CHECK-LABEL: @test_vcge_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sge <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcge_s64(int64x1_t a, int64x1_t b) {
  return vcge_s64(a, b);
}

// CHECK-LABEL: @test_vcge_u64(
// CHECK:   [[CMP_I:%.*]] = icmp uge <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcge_u64(uint64x1_t a, uint64x1_t b) {
  return vcge_u64(a, b);
}

// CHECK-LABEL: @test_vcge_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <2 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_f32(float32x2_t v1, float32x2_t v2) {
  return vcge_f32(v1, v2);
}

// CHECK-LABEL: @test_vcge_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <1 x double> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcge_f64(float64x1_t a, float64x1_t b) {
  return vcge_f64(a, b);
}

// CHECK-LABEL: @test_vcge_u8(
// CHECK:   [[CMP_I:%.*]] = icmp uge <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcge_u8(uint8x8_t v1, uint8x8_t v2) {
  return vcge_u8(v1, v2);
}

// CHECK-LABEL: @test_vcge_u16(
// CHECK:   [[CMP_I:%.*]] = icmp uge <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcge_u16(uint16x4_t v1, uint16x4_t v2) {
  return vcge_u16(v1, v2);
}

// CHECK-LABEL: @test_vcge_u32(
// CHECK:   [[CMP_I:%.*]] = icmp uge <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcge_u32(uint32x2_t v1, uint32x2_t v2) {
  return vcge_u32(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sge <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgeq_s8(int8x16_t v1, int8x16_t v2) {
  return vcgeq_s8(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sge <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgeq_s16(int16x8_t v1, int16x8_t v2) {
  return vcgeq_s16(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sge <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_s32(int32x4_t v1, int32x4_t v2) {
  return vcgeq_s32(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <4 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_f32(float32x4_t v1, float32x4_t v2) {
  return vcgeq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp uge <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgeq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vcgeq_u8(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp uge <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgeq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vcgeq_u16(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp uge <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgeq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vcgeq_u32(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sge <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgeq_s64(int64x2_t v1, int64x2_t v2) {
  return vcgeq_s64(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp uge <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgeq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vcgeq_u64(v1, v2);
}

// CHECK-LABEL: @test_vcgeq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp oge <2 x double> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgeq_f64(float64x2_t v1, float64x2_t v2) {
  return vcgeq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcle_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sle <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
// Notes about vcle:
// LE condition predicate implemented as GE, so check reversed operands.
// Using registers other than v0, v1 are possible, but would be odd.
uint8x8_t test_vcle_s8(int8x8_t v1, int8x8_t v2) {
  return vcle_s8(v1, v2);
}

// CHECK-LABEL: @test_vcle_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sle <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcle_s16(int16x4_t v1, int16x4_t v2) {
  return vcle_s16(v1, v2);
}

// CHECK-LABEL: @test_vcle_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sle <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_s32(int32x2_t v1, int32x2_t v2) {
  return vcle_s32(v1, v2);
}

// CHECK-LABEL: @test_vcle_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sle <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcle_s64(int64x1_t a, int64x1_t b) {
  return vcle_s64(a, b);
}

// CHECK-LABEL: @test_vcle_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ule <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcle_u64(uint64x1_t a, uint64x1_t b) {
  return vcle_u64(a, b);
}

// CHECK-LABEL: @test_vcle_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <2 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_f32(float32x2_t v1, float32x2_t v2) {
  return vcle_f32(v1, v2);
}

// CHECK-LABEL: @test_vcle_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <1 x double> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcle_f64(float64x1_t a, float64x1_t b) {
  return vcle_f64(a, b);
}

// CHECK-LABEL: @test_vcle_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ule <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcle_u8(uint8x8_t v1, uint8x8_t v2) {
  return vcle_u8(v1, v2);
}

// CHECK-LABEL: @test_vcle_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ule <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcle_u16(uint16x4_t v1, uint16x4_t v2) {
  return vcle_u16(v1, v2);
}

// CHECK-LABEL: @test_vcle_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ule <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcle_u32(uint32x2_t v1, uint32x2_t v2) {
  return vcle_u32(v1, v2);
}

// CHECK-LABEL: @test_vcleq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sle <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcleq_s8(int8x16_t v1, int8x16_t v2) {
  return vcleq_s8(v1, v2);
}

// CHECK-LABEL: @test_vcleq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sle <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcleq_s16(int16x8_t v1, int16x8_t v2) {
  return vcleq_s16(v1, v2);
}

// CHECK-LABEL: @test_vcleq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sle <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_s32(int32x4_t v1, int32x4_t v2) {
  return vcleq_s32(v1, v2);
}

// CHECK-LABEL: @test_vcleq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <4 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_f32(float32x4_t v1, float32x4_t v2) {
  return vcleq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcleq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ule <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcleq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vcleq_u8(v1, v2);
}

// CHECK-LABEL: @test_vcleq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ule <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcleq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vcleq_u16(v1, v2);
}

// CHECK-LABEL: @test_vcleq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ule <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcleq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vcleq_u32(v1, v2);
}

// CHECK-LABEL: @test_vcleq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sle <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcleq_s64(int64x2_t v1, int64x2_t v2) {
  return vcleq_s64(v1, v2);
}

// CHECK-LABEL: @test_vcleq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ule <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcleq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vcleq_u64(v1, v2);
}

// CHECK-LABEL: @test_vcleq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp ole <2 x double> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcleq_f64(float64x2_t v1, float64x2_t v2) {
  return vcleq_f64(v1, v2);
}

// CHECK-LABEL: @test_vcgt_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcgt_s8(int8x8_t v1, int8x8_t v2) {
  return vcgt_s8(v1, v2);
}

// CHECK-LABEL: @test_vcgt_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcgt_s16(int16x4_t v1, int16x4_t v2) {
  return vcgt_s16(v1, v2);
}

// CHECK-LABEL: @test_vcgt_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_s32(int32x2_t v1, int32x2_t v2) {
  return vcgt_s32(v1, v2);
}

// CHECK-LABEL: @test_vcgt_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcgt_s64(int64x1_t a, int64x1_t b) {
  return vcgt_s64(a, b);
}

// CHECK-LABEL: @test_vcgt_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcgt_u64(uint64x1_t a, uint64x1_t b) {
  return vcgt_u64(a, b);
}

// CHECK-LABEL: @test_vcgt_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <2 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_f32(float32x2_t v1, float32x2_t v2) {
  return vcgt_f32(v1, v2);
}

// CHECK-LABEL: @test_vcgt_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <1 x double> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vcgt_f64(float64x1_t a, float64x1_t b) {
  return vcgt_f64(a, b);
}

// CHECK-LABEL: @test_vcgt_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vcgt_u8(uint8x8_t v1, uint8x8_t v2) {
  return vcgt_u8(v1, v2);
}

// CHECK-LABEL: @test_vcgt_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vcgt_u16(uint16x4_t v1, uint16x4_t v2) {
  return vcgt_u16(v1, v2);
}

// CHECK-LABEL: @test_vcgt_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vcgt_u32(uint32x2_t v1, uint32x2_t v2) {
  return vcgt_u32(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgtq_s8(int8x16_t v1, int8x16_t v2) {
  return vcgtq_s8(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgtq_s16(int16x8_t v1, int16x8_t v2) {
  return vcgtq_s16(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_s32(int32x4_t v1, int32x4_t v2) {
  return vcgtq_s32(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <4 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_f32(float32x4_t v1, float32x4_t v2) {
  return vcgtq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcgtq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vcgtq_u8(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcgtq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vcgtq_u16(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcgtq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vcgtq_u32(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp sgt <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgtq_s64(int64x2_t v1, int64x2_t v2) {
  return vcgtq_s64(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ugt <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgtq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vcgtq_u64(v1, v2);
}

// CHECK-LABEL: @test_vcgtq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp ogt <2 x double> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcgtq_f64(float64x2_t v1, float64x2_t v2) {
  return vcgtq_f64(v1, v2);
}

// CHECK-LABEL: @test_vclt_s8(
// CHECK:   [[CMP_I:%.*]] = icmp slt <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
// Notes about vclt:
// LT condition predicate implemented as GT, so check reversed operands.
// Using registers other than v0, v1 are possible, but would be odd.
uint8x8_t test_vclt_s8(int8x8_t v1, int8x8_t v2) {
  return vclt_s8(v1, v2);
}

// CHECK-LABEL: @test_vclt_s16(
// CHECK:   [[CMP_I:%.*]] = icmp slt <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vclt_s16(int16x4_t v1, int16x4_t v2) {
  return vclt_s16(v1, v2);
}

// CHECK-LABEL: @test_vclt_s32(
// CHECK:   [[CMP_I:%.*]] = icmp slt <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_s32(int32x2_t v1, int32x2_t v2) {
  return vclt_s32(v1, v2);
}

// CHECK-LABEL: @test_vclt_s64(
// CHECK:   [[CMP_I:%.*]] = icmp slt <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vclt_s64(int64x1_t a, int64x1_t b) {
  return vclt_s64(a, b);
}

// CHECK-LABEL: @test_vclt_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ult <1 x i64> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vclt_u64(uint64x1_t a, uint64x1_t b) {
  return vclt_u64(a, b);
}

// CHECK-LABEL: @test_vclt_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <2 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_f32(float32x2_t v1, float32x2_t v2) {
  return vclt_f32(v1, v2);
}

// CHECK-LABEL: @test_vclt_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <1 x double> %a, %b
// CHECK:   [[SEXT_I:%.*]] = sext <1 x i1> [[CMP_I]] to <1 x i64>
// CHECK:   ret <1 x i64> [[SEXT_I]]
uint64x1_t test_vclt_f64(float64x1_t a, float64x1_t b) {
  return vclt_f64(a, b);
}

// CHECK-LABEL: @test_vclt_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ult <8 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i8>
// CHECK:   ret <8 x i8> [[SEXT_I]]
uint8x8_t test_vclt_u8(uint8x8_t v1, uint8x8_t v2) {
  return vclt_u8(v1, v2);
}

// CHECK-LABEL: @test_vclt_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ult <4 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i16>
// CHECK:   ret <4 x i16> [[SEXT_I]]
uint16x4_t test_vclt_u16(uint16x4_t v1, uint16x4_t v2) {
  return vclt_u16(v1, v2);
}

// CHECK-LABEL: @test_vclt_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ult <2 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i32>
// CHECK:   ret <2 x i32> [[SEXT_I]]
uint32x2_t test_vclt_u32(uint32x2_t v1, uint32x2_t v2) {
  return vclt_u32(v1, v2);
}

// CHECK-LABEL: @test_vcltq_s8(
// CHECK:   [[CMP_I:%.*]] = icmp slt <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcltq_s8(int8x16_t v1, int8x16_t v2) {
  return vcltq_s8(v1, v2);
}

// CHECK-LABEL: @test_vcltq_s16(
// CHECK:   [[CMP_I:%.*]] = icmp slt <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcltq_s16(int16x8_t v1, int16x8_t v2) {
  return vcltq_s16(v1, v2);
}

// CHECK-LABEL: @test_vcltq_s32(
// CHECK:   [[CMP_I:%.*]] = icmp slt <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_s32(int32x4_t v1, int32x4_t v2) {
  return vcltq_s32(v1, v2);
}

// CHECK-LABEL: @test_vcltq_f32(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <4 x float> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_f32(float32x4_t v1, float32x4_t v2) {
  return vcltq_f32(v1, v2);
}

// CHECK-LABEL: @test_vcltq_u8(
// CHECK:   [[CMP_I:%.*]] = icmp ult <16 x i8> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <16 x i1> [[CMP_I]] to <16 x i8>
// CHECK:   ret <16 x i8> [[SEXT_I]]
uint8x16_t test_vcltq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vcltq_u8(v1, v2);
}

// CHECK-LABEL: @test_vcltq_u16(
// CHECK:   [[CMP_I:%.*]] = icmp ult <8 x i16> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <8 x i1> [[CMP_I]] to <8 x i16>
// CHECK:   ret <8 x i16> [[SEXT_I]]
uint16x8_t test_vcltq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vcltq_u16(v1, v2);
}

// CHECK-LABEL: @test_vcltq_u32(
// CHECK:   [[CMP_I:%.*]] = icmp ult <4 x i32> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <4 x i1> [[CMP_I]] to <4 x i32>
// CHECK:   ret <4 x i32> [[SEXT_I]]
uint32x4_t test_vcltq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vcltq_u32(v1, v2);
}

// CHECK-LABEL: @test_vcltq_s64(
// CHECK:   [[CMP_I:%.*]] = icmp slt <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcltq_s64(int64x2_t v1, int64x2_t v2) {
  return vcltq_s64(v1, v2);
}

// CHECK-LABEL: @test_vcltq_u64(
// CHECK:   [[CMP_I:%.*]] = icmp ult <2 x i64> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcltq_u64(uint64x2_t v1, uint64x2_t v2) {
  return vcltq_u64(v1, v2);
}

// CHECK-LABEL: @test_vcltq_f64(
// CHECK:   [[CMP_I:%.*]] = fcmp olt <2 x double> %v1, %v2
// CHECK:   [[SEXT_I:%.*]] = sext <2 x i1> [[CMP_I]] to <2 x i64>
// CHECK:   ret <2 x i64> [[SEXT_I]]
uint64x2_t test_vcltq_f64(float64x2_t v1, float64x2_t v2) {
  return vcltq_f64(v1, v2);
}

// CHECK-LABEL: @test_vhadd_s8(
// CHECK:   [[VHADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.shadd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VHADD_V_I]]
int8x8_t test_vhadd_s8(int8x8_t v1, int8x8_t v2) {
  return vhadd_s8(v1, v2);
}

// CHECK-LABEL: @test_vhadd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.shadd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <4 x i16> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHADD_V2_I]]
int16x4_t test_vhadd_s16(int16x4_t v1, int16x4_t v2) {
  return vhadd_s16(v1, v2);
}

// CHECK-LABEL: @test_vhadd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.shadd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <2 x i32> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHADD_V2_I]]
int32x2_t test_vhadd_s32(int32x2_t v1, int32x2_t v2) {
  return vhadd_s32(v1, v2);
}

// CHECK-LABEL: @test_vhadd_u8(
// CHECK:   [[VHADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uhadd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VHADD_V_I]]
uint8x8_t test_vhadd_u8(uint8x8_t v1, uint8x8_t v2) {
  return vhadd_u8(v1, v2);
}

// CHECK-LABEL: @test_vhadd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uhadd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <4 x i16> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHADD_V2_I]]
uint16x4_t test_vhadd_u16(uint16x4_t v1, uint16x4_t v2) {
  return vhadd_u16(v1, v2);
}

// CHECK-LABEL: @test_vhadd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VHADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uhadd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VHADD_V3_I:%.*]] = bitcast <2 x i32> [[VHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHADD_V2_I]]
uint32x2_t test_vhadd_u32(uint32x2_t v1, uint32x2_t v2) {
  return vhadd_u32(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_s8(
// CHECK:   [[VHADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.shadd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VHADDQ_V_I]]
int8x16_t test_vhaddq_s8(int8x16_t v1, int8x16_t v2) {
  return vhaddq_s8(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.shadd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHADDQ_V2_I]]
int16x8_t test_vhaddq_s16(int16x8_t v1, int16x8_t v2) {
  return vhaddq_s16(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.shadd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHADDQ_V2_I]]
int32x4_t test_vhaddq_s32(int32x4_t v1, int32x4_t v2) {
  return vhaddq_s32(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_u8(
// CHECK:   [[VHADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uhadd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VHADDQ_V_I]]
uint8x16_t test_vhaddq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vhaddq_u8(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uhadd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHADDQ_V2_I]]
uint16x8_t test_vhaddq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vhaddq_u16(v1, v2);
}

// CHECK-LABEL: @test_vhaddq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uhadd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHADDQ_V2_I]]
uint32x4_t test_vhaddq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vhaddq_u32(v1, v2);
}

// CHECK-LABEL: @test_vhsub_s8(
// CHECK:   [[VHSUB_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.shsub.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VHSUB_V_I]]
int8x8_t test_vhsub_s8(int8x8_t v1, int8x8_t v2) {
  return vhsub_s8(v1, v2);
}

// CHECK-LABEL: @test_vhsub_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.shsub.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <4 x i16> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHSUB_V2_I]]
int16x4_t test_vhsub_s16(int16x4_t v1, int16x4_t v2) {
  return vhsub_s16(v1, v2);
}

// CHECK-LABEL: @test_vhsub_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.shsub.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <2 x i32> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHSUB_V2_I]]
int32x2_t test_vhsub_s32(int32x2_t v1, int32x2_t v2) {
  return vhsub_s32(v1, v2);
}

// CHECK-LABEL: @test_vhsub_u8(
// CHECK:   [[VHSUB_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uhsub.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VHSUB_V_I]]
uint8x8_t test_vhsub_u8(uint8x8_t v1, uint8x8_t v2) {
  return vhsub_u8(v1, v2);
}

// CHECK-LABEL: @test_vhsub_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uhsub.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <4 x i16> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VHSUB_V2_I]]
uint16x4_t test_vhsub_u16(uint16x4_t v1, uint16x4_t v2) {
  return vhsub_u16(v1, v2);
}

// CHECK-LABEL: @test_vhsub_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VHSUB_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uhsub.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VHSUB_V3_I:%.*]] = bitcast <2 x i32> [[VHSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VHSUB_V2_I]]
uint32x2_t test_vhsub_u32(uint32x2_t v1, uint32x2_t v2) {
  return vhsub_u32(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_s8(
// CHECK:   [[VHSUBQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.shsub.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VHSUBQ_V_I]]
int8x16_t test_vhsubq_s8(int8x16_t v1, int8x16_t v2) {
  return vhsubq_s8(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.shsub.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHSUBQ_V2_I]]
int16x8_t test_vhsubq_s16(int16x8_t v1, int16x8_t v2) {
  return vhsubq_s16(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.shsub.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHSUBQ_V2_I]]
int32x4_t test_vhsubq_s32(int32x4_t v1, int32x4_t v2) {
  return vhsubq_s32(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_u8(
// CHECK:   [[VHSUBQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uhsub.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VHSUBQ_V_I]]
uint8x16_t test_vhsubq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vhsubq_u8(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uhsub.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VHSUBQ_V2_I]]
uint16x8_t test_vhsubq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vhsubq_u16(v1, v2);
}

// CHECK-LABEL: @test_vhsubq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VHSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uhsub.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VHSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VHSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VHSUBQ_V2_I]]
uint32x4_t test_vhsubq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vhsubq_u32(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_s8(
// CHECK:   [[VRHADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.srhadd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VRHADD_V_I]]
int8x8_t test_vrhadd_s8(int8x8_t v1, int8x8_t v2) {
  return vrhadd_s8(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VRHADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.srhadd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VRHADD_V3_I:%.*]] = bitcast <4 x i16> [[VRHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VRHADD_V2_I]]
int16x4_t test_vrhadd_s16(int16x4_t v1, int16x4_t v2) {
  return vrhadd_s16(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VRHADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.srhadd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VRHADD_V3_I:%.*]] = bitcast <2 x i32> [[VRHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VRHADD_V2_I]]
int32x2_t test_vrhadd_s32(int32x2_t v1, int32x2_t v2) {
  return vrhadd_s32(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_u8(
// CHECK:   [[VRHADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.urhadd.v8i8(<8 x i8> %v1, <8 x i8> %v2)
// CHECK:   ret <8 x i8> [[VRHADD_V_I]]
uint8x8_t test_vrhadd_u8(uint8x8_t v1, uint8x8_t v2) {
  return vrhadd_u8(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %v2 to <8 x i8>
// CHECK:   [[VRHADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.urhadd.v4i16(<4 x i16> %v1, <4 x i16> %v2)
// CHECK:   [[VRHADD_V3_I:%.*]] = bitcast <4 x i16> [[VRHADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VRHADD_V2_I]]
uint16x4_t test_vrhadd_u16(uint16x4_t v1, uint16x4_t v2) {
  return vrhadd_u16(v1, v2);
}

// CHECK-LABEL: @test_vrhadd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %v1 to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %v2 to <8 x i8>
// CHECK:   [[VRHADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.urhadd.v2i32(<2 x i32> %v1, <2 x i32> %v2)
// CHECK:   [[VRHADD_V3_I:%.*]] = bitcast <2 x i32> [[VRHADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VRHADD_V2_I]]
uint32x2_t test_vrhadd_u32(uint32x2_t v1, uint32x2_t v2) {
  return vrhadd_u32(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_s8(
// CHECK:   [[VRHADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.srhadd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VRHADDQ_V_I]]
int8x16_t test_vrhaddq_s8(int8x16_t v1, int8x16_t v2) {
  return vrhaddq_s8(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VRHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.srhadd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VRHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VRHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VRHADDQ_V2_I]]
int16x8_t test_vrhaddq_s16(int16x8_t v1, int16x8_t v2) {
  return vrhaddq_s16(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VRHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.srhadd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VRHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VRHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VRHADDQ_V2_I]]
int32x4_t test_vrhaddq_s32(int32x4_t v1, int32x4_t v2) {
  return vrhaddq_s32(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_u8(
// CHECK:   [[VRHADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.urhadd.v16i8(<16 x i8> %v1, <16 x i8> %v2)
// CHECK:   ret <16 x i8> [[VRHADDQ_V_I]]
uint8x16_t test_vrhaddq_u8(uint8x16_t v1, uint8x16_t v2) {
  return vrhaddq_u8(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %v2 to <16 x i8>
// CHECK:   [[VRHADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.urhadd.v8i16(<8 x i16> %v1, <8 x i16> %v2)
// CHECK:   [[VRHADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VRHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VRHADDQ_V2_I]]
uint16x8_t test_vrhaddq_u16(uint16x8_t v1, uint16x8_t v2) {
  return vrhaddq_u16(v1, v2);
}

// CHECK-LABEL: @test_vrhaddq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %v1 to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %v2 to <16 x i8>
// CHECK:   [[VRHADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.urhadd.v4i32(<4 x i32> %v1, <4 x i32> %v2)
// CHECK:   [[VRHADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VRHADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VRHADDQ_V2_I]]
uint32x4_t test_vrhaddq_u32(uint32x4_t v1, uint32x4_t v2) {
  return vrhaddq_u32(v1, v2);
}

// CHECK-LABEL: @test_vqadd_s8(
// CHECK:   [[VQADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqadd.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQADD_V_I]]
int8x8_t test_vqadd_s8(int8x8_t a, int8x8_t b) {
  return vqadd_s8(a, b);
}

// CHECK-LABEL: @test_vqadd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqadd.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <4 x i16> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQADD_V2_I]]
int16x4_t test_vqadd_s16(int16x4_t a, int16x4_t b) {
  return vqadd_s16(a, b);
}

// CHECK-LABEL: @test_vqadd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqadd.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <2 x i32> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQADD_V2_I]]
int32x2_t test_vqadd_s32(int32x2_t a, int32x2_t b) {
  return vqadd_s32(a, b);
}

// CHECK-LABEL: @test_vqadd_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.sqadd.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <1 x i64> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQADD_V2_I]]
int64x1_t test_vqadd_s64(int64x1_t a, int64x1_t b) {
  return vqadd_s64(a, b);
}

// CHECK-LABEL: @test_vqadd_u8(
// CHECK:   [[VQADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqadd.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQADD_V_I]]
uint8x8_t test_vqadd_u8(uint8x8_t a, uint8x8_t b) {
  return vqadd_u8(a, b);
}

// CHECK-LABEL: @test_vqadd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqadd.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <4 x i16> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQADD_V2_I]]
uint16x4_t test_vqadd_u16(uint16x4_t a, uint16x4_t b) {
  return vqadd_u16(a, b);
}

// CHECK-LABEL: @test_vqadd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqadd.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <2 x i32> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQADD_V2_I]]
uint32x2_t test_vqadd_u32(uint32x2_t a, uint32x2_t b) {
  return vqadd_u32(a, b);
}

// CHECK-LABEL: @test_vqadd_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQADD_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.uqadd.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQADD_V3_I:%.*]] = bitcast <1 x i64> [[VQADD_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQADD_V2_I]]
uint64x1_t test_vqadd_u64(uint64x1_t a, uint64x1_t b) {
  return vqadd_u64(a, b);
}

// CHECK-LABEL: @test_vqaddq_s8(
// CHECK:   [[VQADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sqadd.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQADDQ_V_I]]
int8x16_t test_vqaddq_s8(int8x16_t a, int8x16_t b) {
  return vqaddq_s8(a, b);
}

// CHECK-LABEL: @test_vqaddq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqadd.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQADDQ_V2_I]]
int16x8_t test_vqaddq_s16(int16x8_t a, int16x8_t b) {
  return vqaddq_s16(a, b);
}

// CHECK-LABEL: @test_vqaddq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqadd.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQADDQ_V2_I]]
int32x4_t test_vqaddq_s32(int32x4_t a, int32x4_t b) {
  return vqaddq_s32(a, b);
}

// CHECK-LABEL: @test_vqaddq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.sqadd.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <2 x i64> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQADDQ_V2_I]]
int64x2_t test_vqaddq_s64(int64x2_t a, int64x2_t b) {
  return vqaddq_s64(a, b);
}

// CHECK-LABEL: @test_vqaddq_u8(
// CHECK:   [[VQADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uqadd.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQADDQ_V_I]]
uint8x16_t test_vqaddq_u8(uint8x16_t a, uint8x16_t b) {
  return vqaddq_u8(a, b);
}

// CHECK-LABEL: @test_vqaddq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uqadd.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQADDQ_V2_I]]
uint16x8_t test_vqaddq_u16(uint16x8_t a, uint16x8_t b) {
  return vqaddq_u16(a, b);
}

// CHECK-LABEL: @test_vqaddq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uqadd.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQADDQ_V2_I]]
uint32x4_t test_vqaddq_u32(uint32x4_t a, uint32x4_t b) {
  return vqaddq_u32(a, b);
}

// CHECK-LABEL: @test_vqaddq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQADDQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.uqadd.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQADDQ_V3_I:%.*]] = bitcast <2 x i64> [[VQADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQADDQ_V2_I]]
uint64x2_t test_vqaddq_u64(uint64x2_t a, uint64x2_t b) {
  return vqaddq_u64(a, b);
}

// CHECK-LABEL: @test_vqsub_s8(
// CHECK:   [[VQSUB_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqsub.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQSUB_V_I]]
int8x8_t test_vqsub_s8(int8x8_t a, int8x8_t b) {
  return vqsub_s8(a, b);
}

// CHECK-LABEL: @test_vqsub_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqsub.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <4 x i16> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQSUB_V2_I]]
int16x4_t test_vqsub_s16(int16x4_t a, int16x4_t b) {
  return vqsub_s16(a, b);
}

// CHECK-LABEL: @test_vqsub_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqsub.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <2 x i32> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQSUB_V2_I]]
int32x2_t test_vqsub_s32(int32x2_t a, int32x2_t b) {
  return vqsub_s32(a, b);
}

// CHECK-LABEL: @test_vqsub_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.sqsub.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <1 x i64> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQSUB_V2_I]]
int64x1_t test_vqsub_s64(int64x1_t a, int64x1_t b) {
  return vqsub_s64(a, b);
}

// CHECK-LABEL: @test_vqsub_u8(
// CHECK:   [[VQSUB_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqsub.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQSUB_V_I]]
uint8x8_t test_vqsub_u8(uint8x8_t a, uint8x8_t b) {
  return vqsub_u8(a, b);
}

// CHECK-LABEL: @test_vqsub_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqsub.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <4 x i16> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQSUB_V2_I]]
uint16x4_t test_vqsub_u16(uint16x4_t a, uint16x4_t b) {
  return vqsub_u16(a, b);
}

// CHECK-LABEL: @test_vqsub_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqsub.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <2 x i32> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQSUB_V2_I]]
uint32x2_t test_vqsub_u32(uint32x2_t a, uint32x2_t b) {
  return vqsub_u32(a, b);
}

// CHECK-LABEL: @test_vqsub_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQSUB_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.uqsub.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQSUB_V3_I:%.*]] = bitcast <1 x i64> [[VQSUB_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQSUB_V2_I]]
uint64x1_t test_vqsub_u64(uint64x1_t a, uint64x1_t b) {
  return vqsub_u64(a, b);
}

// CHECK-LABEL: @test_vqsubq_s8(
// CHECK:   [[VQSUBQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sqsub.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQSUBQ_V_I]]
int8x16_t test_vqsubq_s8(int8x16_t a, int8x16_t b) {
  return vqsubq_s8(a, b);
}

// CHECK-LABEL: @test_vqsubq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqsub.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQSUBQ_V2_I]]
int16x8_t test_vqsubq_s16(int16x8_t a, int16x8_t b) {
  return vqsubq_s16(a, b);
}

// CHECK-LABEL: @test_vqsubq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqsub.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQSUBQ_V2_I]]
int32x4_t test_vqsubq_s32(int32x4_t a, int32x4_t b) {
  return vqsubq_s32(a, b);
}

// CHECK-LABEL: @test_vqsubq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.sqsub.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <2 x i64> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQSUBQ_V2_I]]
int64x2_t test_vqsubq_s64(int64x2_t a, int64x2_t b) {
  return vqsubq_s64(a, b);
}

// CHECK-LABEL: @test_vqsubq_u8(
// CHECK:   [[VQSUBQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uqsub.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQSUBQ_V_I]]
uint8x16_t test_vqsubq_u8(uint8x16_t a, uint8x16_t b) {
  return vqsubq_u8(a, b);
}

// CHECK-LABEL: @test_vqsubq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uqsub.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <8 x i16> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQSUBQ_V2_I]]
uint16x8_t test_vqsubq_u16(uint16x8_t a, uint16x8_t b) {
  return vqsubq_u16(a, b);
}

// CHECK-LABEL: @test_vqsubq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uqsub.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <4 x i32> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQSUBQ_V2_I]]
uint32x4_t test_vqsubq_u32(uint32x4_t a, uint32x4_t b) {
  return vqsubq_u32(a, b);
}

// CHECK-LABEL: @test_vqsubq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSUBQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.uqsub.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQSUBQ_V3_I:%.*]] = bitcast <2 x i64> [[VQSUBQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQSUBQ_V2_I]]
uint64x2_t test_vqsubq_u64(uint64x2_t a, uint64x2_t b) {
  return vqsubq_u64(a, b);
}

// CHECK-LABEL: @test_vshl_s8(
// CHECK:   [[VSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VSHL_V_I]]
int8x8_t test_vshl_s8(int8x8_t a, int8x8_t b) {
  return vshl_s8(a, b);
}

// CHECK-LABEL: @test_vshl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <4 x i16> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VSHL_V2_I]]
int16x4_t test_vshl_s16(int16x4_t a, int16x4_t b) {
  return vshl_s16(a, b);
}

// CHECK-LABEL: @test_vshl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <2 x i32> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VSHL_V2_I]]
int32x2_t test_vshl_s32(int32x2_t a, int32x2_t b) {
  return vshl_s32(a, b);
}

// CHECK-LABEL: @test_vshl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.sshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <1 x i64> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VSHL_V2_I]]
int64x1_t test_vshl_s64(int64x1_t a, int64x1_t b) {
  return vshl_s64(a, b);
}

// CHECK-LABEL: @test_vshl_u8(
// CHECK:   [[VSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.ushl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VSHL_V_I]]
uint8x8_t test_vshl_u8(uint8x8_t a, int8x8_t b) {
  return vshl_u8(a, b);
}

// CHECK-LABEL: @test_vshl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.ushl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <4 x i16> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VSHL_V2_I]]
uint16x4_t test_vshl_u16(uint16x4_t a, int16x4_t b) {
  return vshl_u16(a, b);
}

// CHECK-LABEL: @test_vshl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.ushl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <2 x i32> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VSHL_V2_I]]
uint32x2_t test_vshl_u32(uint32x2_t a, int32x2_t b) {
  return vshl_u32(a, b);
}

// CHECK-LABEL: @test_vshl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.ushl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VSHL_V3_I:%.*]] = bitcast <1 x i64> [[VSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VSHL_V2_I]]
uint64x1_t test_vshl_u64(uint64x1_t a, int64x1_t b) {
  return vshl_u64(a, b);
}

// CHECK-LABEL: @test_vshlq_s8(
// CHECK:   [[VSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VSHLQ_V_I]]
int8x16_t test_vshlq_s8(int8x16_t a, int8x16_t b) {
  return vshlq_s8(a, b);
}

// CHECK-LABEL: @test_vshlq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VSHLQ_V2_I]]
int16x8_t test_vshlq_s16(int16x8_t a, int16x8_t b) {
  return vshlq_s16(a, b);
}

// CHECK-LABEL: @test_vshlq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VSHLQ_V2_I]]
int32x4_t test_vshlq_s32(int32x4_t a, int32x4_t b) {
  return vshlq_s32(a, b);
}

// CHECK-LABEL: @test_vshlq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.sshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VSHLQ_V2_I]]
int64x2_t test_vshlq_s64(int64x2_t a, int64x2_t b) {
  return vshlq_s64(a, b);
}

// CHECK-LABEL: @test_vshlq_u8(
// CHECK:   [[VSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.ushl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VSHLQ_V_I]]
uint8x16_t test_vshlq_u8(uint8x16_t a, int8x16_t b) {
  return vshlq_u8(a, b);
}

// CHECK-LABEL: @test_vshlq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.ushl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VSHLQ_V2_I]]
uint16x8_t test_vshlq_u16(uint16x8_t a, int16x8_t b) {
  return vshlq_u16(a, b);
}

// CHECK-LABEL: @test_vshlq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.ushl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VSHLQ_V2_I]]
uint32x4_t test_vshlq_u32(uint32x4_t a, int32x4_t b) {
  return vshlq_u32(a, b);
}

// CHECK-LABEL: @test_vshlq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.ushl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VSHLQ_V2_I]]
uint64x2_t test_vshlq_u64(uint64x2_t a, int64x2_t b) {
  return vshlq_u64(a, b);
}

// CHECK-LABEL: @test_vqshl_s8(
// CHECK:   [[VQSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQSHL_V_I]]
int8x8_t test_vqshl_s8(int8x8_t a, int8x8_t b) {
  return vqshl_s8(a, b);
}

// CHECK-LABEL: @test_vqshl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <4 x i16> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQSHL_V2_I]]
int16x4_t test_vqshl_s16(int16x4_t a, int16x4_t b) {
  return vqshl_s16(a, b);
}

// CHECK-LABEL: @test_vqshl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <2 x i32> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQSHL_V2_I]]
int32x2_t test_vqshl_s32(int32x2_t a, int32x2_t b) {
  return vqshl_s32(a, b);
}

// CHECK-LABEL: @test_vqshl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.sqshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <1 x i64> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQSHL_V2_I]]
int64x1_t test_vqshl_s64(int64x1_t a, int64x1_t b) {
  return vqshl_s64(a, b);
}

// CHECK-LABEL: @test_vqshl_u8(
// CHECK:   [[VQSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQSHL_V_I]]
uint8x8_t test_vqshl_u8(uint8x8_t a, int8x8_t b) {
  return vqshl_u8(a, b);
}

// CHECK-LABEL: @test_vqshl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <4 x i16> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQSHL_V2_I]]
uint16x4_t test_vqshl_u16(uint16x4_t a, int16x4_t b) {
  return vqshl_u16(a, b);
}

// CHECK-LABEL: @test_vqshl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <2 x i32> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQSHL_V2_I]]
uint32x2_t test_vqshl_u32(uint32x2_t a, int32x2_t b) {
  return vqshl_u32(a, b);
}

// CHECK-LABEL: @test_vqshl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.uqshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQSHL_V3_I:%.*]] = bitcast <1 x i64> [[VQSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQSHL_V2_I]]
uint64x1_t test_vqshl_u64(uint64x1_t a, int64x1_t b) {
  return vqshl_u64(a, b);
}

// CHECK-LABEL: @test_vqshlq_s8(
// CHECK:   [[VQSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sqshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQSHLQ_V_I]]
int8x16_t test_vqshlq_s8(int8x16_t a, int8x16_t b) {
  return vqshlq_s8(a, b);
}

// CHECK-LABEL: @test_vqshlq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQSHLQ_V2_I]]
int16x8_t test_vqshlq_s16(int16x8_t a, int16x8_t b) {
  return vqshlq_s16(a, b);
}

// CHECK-LABEL: @test_vqshlq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQSHLQ_V2_I]]
int32x4_t test_vqshlq_s32(int32x4_t a, int32x4_t b) {
  return vqshlq_s32(a, b);
}

// CHECK-LABEL: @test_vqshlq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.sqshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQSHLQ_V2_I]]
int64x2_t test_vqshlq_s64(int64x2_t a, int64x2_t b) {
  return vqshlq_s64(a, b);
}

// CHECK-LABEL: @test_vqshlq_u8(
// CHECK:   [[VQSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uqshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQSHLQ_V_I]]
uint8x16_t test_vqshlq_u8(uint8x16_t a, int8x16_t b) {
  return vqshlq_u8(a, b);
}

// CHECK-LABEL: @test_vqshlq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uqshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQSHLQ_V2_I]]
uint16x8_t test_vqshlq_u16(uint16x8_t a, int16x8_t b) {
  return vqshlq_u16(a, b);
}

// CHECK-LABEL: @test_vqshlq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uqshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQSHLQ_V2_I]]
uint32x4_t test_vqshlq_u32(uint32x4_t a, int32x4_t b) {
  return vqshlq_u32(a, b);
}

// CHECK-LABEL: @test_vqshlq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.uqshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VQSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQSHLQ_V2_I]]
uint64x2_t test_vqshlq_u64(uint64x2_t a, int64x2_t b) {
  return vqshlq_u64(a, b);
}

// CHECK-LABEL: @test_vrshl_s8(
// CHECK:   [[VRSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.srshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VRSHL_V_I]]
int8x8_t test_vrshl_s8(int8x8_t a, int8x8_t b) {
  return vrshl_s8(a, b);
}

// CHECK-LABEL: @test_vrshl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.srshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <4 x i16> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VRSHL_V2_I]]
int16x4_t test_vrshl_s16(int16x4_t a, int16x4_t b) {
  return vrshl_s16(a, b);
}

// CHECK-LABEL: @test_vrshl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.srshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <2 x i32> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VRSHL_V2_I]]
int32x2_t test_vrshl_s32(int32x2_t a, int32x2_t b) {
  return vrshl_s32(a, b);
}

// CHECK-LABEL: @test_vrshl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.srshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <1 x i64> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VRSHL_V2_I]]
int64x1_t test_vrshl_s64(int64x1_t a, int64x1_t b) {
  return vrshl_s64(a, b);
}

// CHECK-LABEL: @test_vrshl_u8(
// CHECK:   [[VRSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.urshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VRSHL_V_I]]
uint8x8_t test_vrshl_u8(uint8x8_t a, int8x8_t b) {
  return vrshl_u8(a, b);
}

// CHECK-LABEL: @test_vrshl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.urshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <4 x i16> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VRSHL_V2_I]]
uint16x4_t test_vrshl_u16(uint16x4_t a, int16x4_t b) {
  return vrshl_u16(a, b);
}

// CHECK-LABEL: @test_vrshl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.urshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <2 x i32> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VRSHL_V2_I]]
uint32x2_t test_vrshl_u32(uint32x2_t a, int32x2_t b) {
  return vrshl_u32(a, b);
}

// CHECK-LABEL: @test_vrshl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VRSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.urshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VRSHL_V3_I:%.*]] = bitcast <1 x i64> [[VRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VRSHL_V2_I]]
uint64x1_t test_vrshl_u64(uint64x1_t a, int64x1_t b) {
  return vrshl_u64(a, b);
}

// CHECK-LABEL: @test_vrshlq_s8(
// CHECK:   [[VRSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.srshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VRSHLQ_V_I]]
int8x16_t test_vrshlq_s8(int8x16_t a, int8x16_t b) {
  return vrshlq_s8(a, b);
}

// CHECK-LABEL: @test_vrshlq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.srshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VRSHLQ_V2_I]]
int16x8_t test_vrshlq_s16(int16x8_t a, int16x8_t b) {
  return vrshlq_s16(a, b);
}

// CHECK-LABEL: @test_vrshlq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.srshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VRSHLQ_V2_I]]
int32x4_t test_vrshlq_s32(int32x4_t a, int32x4_t b) {
  return vrshlq_s32(a, b);
}

// CHECK-LABEL: @test_vrshlq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.srshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VRSHLQ_V2_I]]
int64x2_t test_vrshlq_s64(int64x2_t a, int64x2_t b) {
  return vrshlq_s64(a, b);
}

// CHECK-LABEL: @test_vrshlq_u8(
// CHECK:   [[VRSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.urshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VRSHLQ_V_I]]
uint8x16_t test_vrshlq_u8(uint8x16_t a, int8x16_t b) {
  return vrshlq_u8(a, b);
}

// CHECK-LABEL: @test_vrshlq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.urshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VRSHLQ_V2_I]]
uint16x8_t test_vrshlq_u16(uint16x8_t a, int16x8_t b) {
  return vrshlq_u16(a, b);
}

// CHECK-LABEL: @test_vrshlq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.urshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VRSHLQ_V2_I]]
uint32x4_t test_vrshlq_u32(uint32x4_t a, int32x4_t b) {
  return vrshlq_u32(a, b);
}

// CHECK-LABEL: @test_vrshlq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.urshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VRSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VRSHLQ_V2_I]]
uint64x2_t test_vrshlq_u64(uint64x2_t a, int64x2_t b) {
  return vrshlq_u64(a, b);
}

// CHECK-LABEL: @test_vqrshl_s8(
// CHECK:   [[VQRSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqrshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQRSHL_V_I]]
int8x8_t test_vqrshl_s8(int8x8_t a, int8x8_t b) {
  return vqrshl_s8(a, b);
}

// CHECK-LABEL: @test_vqrshl_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqrshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <4 x i16> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQRSHL_V2_I]]
int16x4_t test_vqrshl_s16(int16x4_t a, int16x4_t b) {
  return vqrshl_s16(a, b);
}

// CHECK-LABEL: @test_vqrshl_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqrshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <2 x i32> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQRSHL_V2_I]]
int32x2_t test_vqrshl_s32(int32x2_t a, int32x2_t b) {
  return vqrshl_s32(a, b);
}

// CHECK-LABEL: @test_vqrshl_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.sqrshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <1 x i64> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQRSHL_V2_I]]
int64x1_t test_vqrshl_s64(int64x1_t a, int64x1_t b) {
  return vqrshl_s64(a, b);
}

// CHECK-LABEL: @test_vqrshl_u8(
// CHECK:   [[VQRSHL_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqrshl.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VQRSHL_V_I]]
uint8x8_t test_vqrshl_u8(uint8x8_t a, int8x8_t b) {
  return vqrshl_u8(a, b);
}

// CHECK-LABEL: @test_vqrshl_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqrshl.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <4 x i16> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQRSHL_V2_I]]
uint16x4_t test_vqrshl_u16(uint16x4_t a, int16x4_t b) {
  return vqrshl_u16(a, b);
}

// CHECK-LABEL: @test_vqrshl_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqrshl.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <2 x i32> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQRSHL_V2_I]]
uint32x2_t test_vqrshl_u32(uint32x2_t a, int32x2_t b) {
  return vqrshl_u32(a, b);
}

// CHECK-LABEL: @test_vqrshl_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VQRSHL_V2_I:%.*]] = call <1 x i64> @llvm.aarch64.neon.uqrshl.v1i64(<1 x i64> %a, <1 x i64> %b)
// CHECK:   [[VQRSHL_V3_I:%.*]] = bitcast <1 x i64> [[VQRSHL_V2_I]] to <8 x i8>
// CHECK:   ret <1 x i64> [[VQRSHL_V2_I]]
uint64x1_t test_vqrshl_u64(uint64x1_t a, int64x1_t b) {
  return vqrshl_u64(a, b);
}

// CHECK-LABEL: @test_vqrshlq_s8(
// CHECK:   [[VQRSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sqrshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQRSHLQ_V_I]]
int8x16_t test_vqrshlq_s8(int8x16_t a, int8x16_t b) {
  return vqrshlq_s8(a, b);
}

// CHECK-LABEL: @test_vqrshlq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqrshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQRSHLQ_V2_I]]
int16x8_t test_vqrshlq_s16(int16x8_t a, int16x8_t b) {
  return vqrshlq_s16(a, b);
}

// CHECK-LABEL: @test_vqrshlq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqrshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQRSHLQ_V2_I]]
int32x4_t test_vqrshlq_s32(int32x4_t a, int32x4_t b) {
  return vqrshlq_s32(a, b);
}

// CHECK-LABEL: @test_vqrshlq_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.sqrshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQRSHLQ_V2_I]]
int64x2_t test_vqrshlq_s64(int64x2_t a, int64x2_t b) {
  return vqrshlq_s64(a, b);
}

// CHECK-LABEL: @test_vqrshlq_u8(
// CHECK:   [[VQRSHLQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uqrshl.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VQRSHLQ_V_I]]
uint8x16_t test_vqrshlq_u8(uint8x16_t a, int8x16_t b) {
  return vqrshlq_u8(a, b);
}

// CHECK-LABEL: @test_vqrshlq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uqrshl.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <8 x i16> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQRSHLQ_V2_I]]
uint16x8_t test_vqrshlq_u16(uint16x8_t a, int16x8_t b) {
  return vqrshlq_u16(a, b);
}

// CHECK-LABEL: @test_vqrshlq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uqrshl.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <4 x i32> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQRSHLQ_V2_I]]
uint32x4_t test_vqrshlq_u32(uint32x4_t a, int32x4_t b) {
  return vqrshlq_u32(a, b);
}

// CHECK-LABEL: @test_vqrshlq_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQRSHLQ_V2_I:%.*]] = call <2 x i64> @llvm.aarch64.neon.uqrshl.v2i64(<2 x i64> %a, <2 x i64> %b)
// CHECK:   [[VQRSHLQ_V3_I:%.*]] = bitcast <2 x i64> [[VQRSHLQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x i64> [[VQRSHLQ_V2_I]]
uint64x2_t test_vqrshlq_u64(uint64x2_t a, int64x2_t b) {
  return vqrshlq_u64(a, b);
}

// CHECK-LABEL: @test_vsli_n_p64(
// CHECK:   [[TMP0:%.*]] = bitcast <1 x i64> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <1 x i64> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <1 x i64>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <1 x i64>
// CHECK:   [[VSLI_N2:%.*]] = call <1 x i64> @llvm.aarch64.neon.vsli.v1i64(<1 x i64> [[VSLI_N]], <1 x i64> [[VSLI_N1]], i32 0)
// CHECK:   ret <1 x i64> [[VSLI_N2]]
poly64x1_t test_vsli_n_p64(poly64x1_t a, poly64x1_t b) {
  return vsli_n_p64(a, b, 0);
}

// CHECK-LABEL: @test_vsliq_n_p64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSLI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsli.v2i64(<2 x i64> [[VSLI_N]], <2 x i64> [[VSLI_N1]], i32 0)
// CHECK:   ret <2 x i64> [[VSLI_N2]]
poly64x2_t test_vsliq_n_p64(poly64x2_t a, poly64x2_t b) {
  return vsliq_n_p64(a, b, 0);
}

// CHECK-LABEL: @test_vmax_s8(
// CHECK:   [[VMAX_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.smax.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VMAX_I]]
int8x8_t test_vmax_s8(int8x8_t a, int8x8_t b) {
  return vmax_s8(a, b);
}

// CHECK-LABEL: @test_vmax_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.smax.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VMAX2_I]]
int16x4_t test_vmax_s16(int16x4_t a, int16x4_t b) {
  return vmax_s16(a, b);
}

// CHECK-LABEL: @test_vmax_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.smax.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VMAX2_I]]
int32x2_t test_vmax_s32(int32x2_t a, int32x2_t b) {
  return vmax_s32(a, b);
}

// CHECK-LABEL: @test_vmax_u8(
// CHECK:   [[VMAX_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.umax.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VMAX_I]]
uint8x8_t test_vmax_u8(uint8x8_t a, uint8x8_t b) {
  return vmax_u8(a, b);
}

// CHECK-LABEL: @test_vmax_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.umax.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VMAX2_I]]
uint16x4_t test_vmax_u16(uint16x4_t a, uint16x4_t b) {
  return vmax_u16(a, b);
}

// CHECK-LABEL: @test_vmax_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.umax.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VMAX2_I]]
uint32x2_t test_vmax_u32(uint32x2_t a, uint32x2_t b) {
  return vmax_u32(a, b);
}

// CHECK-LABEL: @test_vmax_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmax.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VMAX2_I]]
float32x2_t test_vmax_f32(float32x2_t a, float32x2_t b) {
  return vmax_f32(a, b);
}

// CHECK-LABEL: @test_vmaxq_s8(
// CHECK:   [[VMAX_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.smax.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VMAX_I]]
int8x16_t test_vmaxq_s8(int8x16_t a, int8x16_t b) {
  return vmaxq_s8(a, b);
}

// CHECK-LABEL: @test_vmaxq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.smax.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VMAX2_I]]
int16x8_t test_vmaxq_s16(int16x8_t a, int16x8_t b) {
  return vmaxq_s16(a, b);
}

// CHECK-LABEL: @test_vmaxq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.smax.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VMAX2_I]]
int32x4_t test_vmaxq_s32(int32x4_t a, int32x4_t b) {
  return vmaxq_s32(a, b);
}

// CHECK-LABEL: @test_vmaxq_u8(
// CHECK:   [[VMAX_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.umax.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VMAX_I]]
uint8x16_t test_vmaxq_u8(uint8x16_t a, uint8x16_t b) {
  return vmaxq_u8(a, b);
}

// CHECK-LABEL: @test_vmaxq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.umax.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VMAX2_I]]
uint16x8_t test_vmaxq_u16(uint16x8_t a, uint16x8_t b) {
  return vmaxq_u16(a, b);
}

// CHECK-LABEL: @test_vmaxq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.umax.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VMAX2_I]]
uint32x4_t test_vmaxq_u32(uint32x4_t a, uint32x4_t b) {
  return vmaxq_u32(a, b);
}

// CHECK-LABEL: @test_vmaxq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmax.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VMAX2_I]]
float32x4_t test_vmaxq_f32(float32x4_t a, float32x4_t b) {
  return vmaxq_f32(a, b);
}

// CHECK-LABEL: @test_vmaxq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VMAX2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmax.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VMAX2_I]]
float64x2_t test_vmaxq_f64(float64x2_t a, float64x2_t b) {
  return vmaxq_f64(a, b);
}

// CHECK-LABEL: @test_vmin_s8(
// CHECK:   [[VMIN_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.smin.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VMIN_I]]
int8x8_t test_vmin_s8(int8x8_t a, int8x8_t b) {
  return vmin_s8(a, b);
}

// CHECK-LABEL: @test_vmin_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.smin.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VMIN2_I]]
int16x4_t test_vmin_s16(int16x4_t a, int16x4_t b) {
  return vmin_s16(a, b);
}

// CHECK-LABEL: @test_vmin_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.smin.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VMIN2_I]]
int32x2_t test_vmin_s32(int32x2_t a, int32x2_t b) {
  return vmin_s32(a, b);
}

// CHECK-LABEL: @test_vmin_u8(
// CHECK:   [[VMIN_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.umin.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VMIN_I]]
uint8x8_t test_vmin_u8(uint8x8_t a, uint8x8_t b) {
  return vmin_u8(a, b);
}

// CHECK-LABEL: @test_vmin_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.umin.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VMIN2_I]]
uint16x4_t test_vmin_u16(uint16x4_t a, uint16x4_t b) {
  return vmin_u16(a, b);
}

// CHECK-LABEL: @test_vmin_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.umin.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VMIN2_I]]
uint32x2_t test_vmin_u32(uint32x2_t a, uint32x2_t b) {
  return vmin_u32(a, b);
}

// CHECK-LABEL: @test_vmin_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmin.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VMIN2_I]]
float32x2_t test_vmin_f32(float32x2_t a, float32x2_t b) {
  return vmin_f32(a, b);
}

// CHECK-LABEL: @test_vminq_s8(
// CHECK:   [[VMIN_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.smin.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VMIN_I]]
int8x16_t test_vminq_s8(int8x16_t a, int8x16_t b) {
  return vminq_s8(a, b);
}

// CHECK-LABEL: @test_vminq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.smin.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VMIN2_I]]
int16x8_t test_vminq_s16(int16x8_t a, int16x8_t b) {
  return vminq_s16(a, b);
}

// CHECK-LABEL: @test_vminq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.smin.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VMIN2_I]]
int32x4_t test_vminq_s32(int32x4_t a, int32x4_t b) {
  return vminq_s32(a, b);
}

// CHECK-LABEL: @test_vminq_u8(
// CHECK:   [[VMIN_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.umin.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VMIN_I]]
uint8x16_t test_vminq_u8(uint8x16_t a, uint8x16_t b) {
  return vminq_u8(a, b);
}

// CHECK-LABEL: @test_vminq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.umin.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VMIN2_I]]
uint16x8_t test_vminq_u16(uint16x8_t a, uint16x8_t b) {
  return vminq_u16(a, b);
}

// CHECK-LABEL: @test_vminq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.umin.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VMIN2_I]]
uint32x4_t test_vminq_u32(uint32x4_t a, uint32x4_t b) {
  return vminq_u32(a, b);
}

// CHECK-LABEL: @test_vminq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmin.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VMIN2_I]]
float32x4_t test_vminq_f32(float32x4_t a, float32x4_t b) {
  return vminq_f32(a, b);
}

// CHECK-LABEL: @test_vminq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VMIN2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmin.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VMIN2_I]]
float64x2_t test_vminq_f64(float64x2_t a, float64x2_t b) {
  return vminq_f64(a, b);
}

// CHECK-LABEL: @test_vmaxnm_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VMAXNM2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmaxnm.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VMAXNM2_I]]
float32x2_t test_vmaxnm_f32(float32x2_t a, float32x2_t b) {
  return vmaxnm_f32(a, b);
}

// CHECK-LABEL: @test_vmaxnmq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VMAXNM2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmaxnm.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VMAXNM2_I]]
float32x4_t test_vmaxnmq_f32(float32x4_t a, float32x4_t b) {
  return vmaxnmq_f32(a, b);
}

// CHECK-LABEL: @test_vmaxnmq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VMAXNM2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmaxnm.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VMAXNM2_I]]
float64x2_t test_vmaxnmq_f64(float64x2_t a, float64x2_t b) {
  return vmaxnmq_f64(a, b);
}

// CHECK-LABEL: @test_vminnm_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VMINNM2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fminnm.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VMINNM2_I]]
float32x2_t test_vminnm_f32(float32x2_t a, float32x2_t b) {
  return vminnm_f32(a, b);
}

// CHECK-LABEL: @test_vminnmq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VMINNM2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fminnm.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VMINNM2_I]]
float32x4_t test_vminnmq_f32(float32x4_t a, float32x4_t b) {
  return vminnmq_f32(a, b);
}

// CHECK-LABEL: @test_vminnmq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VMINNM2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fminnm.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VMINNM2_I]]
float64x2_t test_vminnmq_f64(float64x2_t a, float64x2_t b) {
  return vminnmq_f64(a, b);
}

// CHECK-LABEL: @test_vpmax_s8(
// CHECK:   [[VPMAX_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.smaxp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPMAX_I]]
int8x8_t test_vpmax_s8(int8x8_t a, int8x8_t b) {
  return vpmax_s8(a, b);
}

// CHECK-LABEL: @test_vpmax_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.smaxp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VPMAX2_I]]
int16x4_t test_vpmax_s16(int16x4_t a, int16x4_t b) {
  return vpmax_s16(a, b);
}

// CHECK-LABEL: @test_vpmax_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.smaxp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VPMAX2_I]]
int32x2_t test_vpmax_s32(int32x2_t a, int32x2_t b) {
  return vpmax_s32(a, b);
}

// CHECK-LABEL: @test_vpmax_u8(
// CHECK:   [[VPMAX_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.umaxp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPMAX_I]]
uint8x8_t test_vpmax_u8(uint8x8_t a, uint8x8_t b) {
  return vpmax_u8(a, b);
}

// CHECK-LABEL: @test_vpmax_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.umaxp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VPMAX2_I]]
uint16x4_t test_vpmax_u16(uint16x4_t a, uint16x4_t b) {
  return vpmax_u16(a, b);
}

// CHECK-LABEL: @test_vpmax_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.umaxp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VPMAX2_I]]
uint32x2_t test_vpmax_u32(uint32x2_t a, uint32x2_t b) {
  return vpmax_u32(a, b);
}

// CHECK-LABEL: @test_vpmax_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmaxp.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VPMAX2_I]]
float32x2_t test_vpmax_f32(float32x2_t a, float32x2_t b) {
  return vpmax_f32(a, b);
}

// CHECK-LABEL: @test_vpmaxq_s8(
// CHECK:   [[VPMAX_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.smaxp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPMAX_I]]
int8x16_t test_vpmaxq_s8(int8x16_t a, int8x16_t b) {
  return vpmaxq_s8(a, b);
}

// CHECK-LABEL: @test_vpmaxq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.smaxp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VPMAX2_I]]
int16x8_t test_vpmaxq_s16(int16x8_t a, int16x8_t b) {
  return vpmaxq_s16(a, b);
}

// CHECK-LABEL: @test_vpmaxq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.smaxp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VPMAX2_I]]
int32x4_t test_vpmaxq_s32(int32x4_t a, int32x4_t b) {
  return vpmaxq_s32(a, b);
}

// CHECK-LABEL: @test_vpmaxq_u8(
// CHECK:   [[VPMAX_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.umaxp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPMAX_I]]
uint8x16_t test_vpmaxq_u8(uint8x16_t a, uint8x16_t b) {
  return vpmaxq_u8(a, b);
}

// CHECK-LABEL: @test_vpmaxq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.umaxp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VPMAX2_I]]
uint16x8_t test_vpmaxq_u16(uint16x8_t a, uint16x8_t b) {
  return vpmaxq_u16(a, b);
}

// CHECK-LABEL: @test_vpmaxq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.umaxp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VPMAX2_I]]
uint32x4_t test_vpmaxq_u32(uint32x4_t a, uint32x4_t b) {
  return vpmaxq_u32(a, b);
}

// CHECK-LABEL: @test_vpmaxq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmaxp.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VPMAX2_I]]
float32x4_t test_vpmaxq_f32(float32x4_t a, float32x4_t b) {
  return vpmaxq_f32(a, b);
}

// CHECK-LABEL: @test_vpmaxq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VPMAX2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmaxp.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VPMAX2_I]]
float64x2_t test_vpmaxq_f64(float64x2_t a, float64x2_t b) {
  return vpmaxq_f64(a, b);
}

// CHECK-LABEL: @test_vpmin_s8(
// CHECK:   [[VPMIN_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.sminp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPMIN_I]]
int8x8_t test_vpmin_s8(int8x8_t a, int8x8_t b) {
  return vpmin_s8(a, b);
}

// CHECK-LABEL: @test_vpmin_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sminp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VPMIN2_I]]
int16x4_t test_vpmin_s16(int16x4_t a, int16x4_t b) {
  return vpmin_s16(a, b);
}

// CHECK-LABEL: @test_vpmin_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sminp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VPMIN2_I]]
int32x2_t test_vpmin_s32(int32x2_t a, int32x2_t b) {
  return vpmin_s32(a, b);
}

// CHECK-LABEL: @test_vpmin_u8(
// CHECK:   [[VPMIN_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.uminp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPMIN_I]]
uint8x8_t test_vpmin_u8(uint8x8_t a, uint8x8_t b) {
  return vpmin_u8(a, b);
}

// CHECK-LABEL: @test_vpmin_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.uminp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   ret <4 x i16> [[VPMIN2_I]]
uint16x4_t test_vpmin_u16(uint16x4_t a, uint16x4_t b) {
  return vpmin_u16(a, b);
}

// CHECK-LABEL: @test_vpmin_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.uminp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   ret <2 x i32> [[VPMIN2_I]]
uint32x2_t test_vpmin_u32(uint32x2_t a, uint32x2_t b) {
  return vpmin_u32(a, b);
}

// CHECK-LABEL: @test_vpmin_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fminp.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VPMIN2_I]]
float32x2_t test_vpmin_f32(float32x2_t a, float32x2_t b) {
  return vpmin_f32(a, b);
}

// CHECK-LABEL: @test_vpminq_s8(
// CHECK:   [[VPMIN_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.sminp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPMIN_I]]
int8x16_t test_vpminq_s8(int8x16_t a, int8x16_t b) {
  return vpminq_s8(a, b);
}

// CHECK-LABEL: @test_vpminq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sminp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VPMIN2_I]]
int16x8_t test_vpminq_s16(int16x8_t a, int16x8_t b) {
  return vpminq_s16(a, b);
}

// CHECK-LABEL: @test_vpminq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sminp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VPMIN2_I]]
int32x4_t test_vpminq_s32(int32x4_t a, int32x4_t b) {
  return vpminq_s32(a, b);
}

// CHECK-LABEL: @test_vpminq_u8(
// CHECK:   [[VPMIN_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.uminp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPMIN_I]]
uint8x16_t test_vpminq_u8(uint8x16_t a, uint8x16_t b) {
  return vpminq_u8(a, b);
}

// CHECK-LABEL: @test_vpminq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.uminp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   ret <8 x i16> [[VPMIN2_I]]
uint16x8_t test_vpminq_u16(uint16x8_t a, uint16x8_t b) {
  return vpminq_u16(a, b);
}

// CHECK-LABEL: @test_vpminq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.uminp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   ret <4 x i32> [[VPMIN2_I]]
uint32x4_t test_vpminq_u32(uint32x4_t a, uint32x4_t b) {
  return vpminq_u32(a, b);
}

// CHECK-LABEL: @test_vpminq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fminp.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VPMIN2_I]]
float32x4_t test_vpminq_f32(float32x4_t a, float32x4_t b) {
  return vpminq_f32(a, b);
}

// CHECK-LABEL: @test_vpminq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VPMIN2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fminp.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VPMIN2_I]]
float64x2_t test_vpminq_f64(float64x2_t a, float64x2_t b) {
  return vpminq_f64(a, b);
}

// CHECK-LABEL: @test_vpmaxnm_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VPMAXNM2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmaxnmp.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VPMAXNM2_I]]
float32x2_t test_vpmaxnm_f32(float32x2_t a, float32x2_t b) {
  return vpmaxnm_f32(a, b);
}

// CHECK-LABEL: @test_vpmaxnmq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VPMAXNM2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmaxnmp.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VPMAXNM2_I]]
float32x4_t test_vpmaxnmq_f32(float32x4_t a, float32x4_t b) {
  return vpmaxnmq_f32(a, b);
}

// CHECK-LABEL: @test_vpmaxnmq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VPMAXNM2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmaxnmp.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VPMAXNM2_I]]
float64x2_t test_vpmaxnmq_f64(float64x2_t a, float64x2_t b) {
  return vpmaxnmq_f64(a, b);
}

// CHECK-LABEL: @test_vpminnm_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VPMINNM2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fminnmp.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VPMINNM2_I]]
float32x2_t test_vpminnm_f32(float32x2_t a, float32x2_t b) {
  return vpminnm_f32(a, b);
}

// CHECK-LABEL: @test_vpminnmq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VPMINNM2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fminnmp.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VPMINNM2_I]]
float32x4_t test_vpminnmq_f32(float32x4_t a, float32x4_t b) {
  return vpminnmq_f32(a, b);
}

// CHECK-LABEL: @test_vpminnmq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VPMINNM2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fminnmp.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VPMINNM2_I]]
float64x2_t test_vpminnmq_f64(float64x2_t a, float64x2_t b) {
  return vpminnmq_f64(a, b);
}

// CHECK-LABEL: @test_vpadd_s8(
// CHECK:   [[VPADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.addp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPADD_V_I]]
int8x8_t test_vpadd_s8(int8x8_t a, int8x8_t b) {
  return vpadd_s8(a, b);
}

// CHECK-LABEL: @test_vpadd_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.addp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VPADD_V3_I:%.*]] = bitcast <4 x i16> [[VPADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VPADD_V2_I]]
int16x4_t test_vpadd_s16(int16x4_t a, int16x4_t b) {
  return vpadd_s16(a, b);
}

// CHECK-LABEL: @test_vpadd_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.addp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VPADD_V3_I:%.*]] = bitcast <2 x i32> [[VPADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VPADD_V2_I]]
int32x2_t test_vpadd_s32(int32x2_t a, int32x2_t b) {
  return vpadd_s32(a, b);
}

// CHECK-LABEL: @test_vpadd_u8(
// CHECK:   [[VPADD_V_I:%.*]] = call <8 x i8> @llvm.aarch64.neon.addp.v8i8(<8 x i8> %a, <8 x i8> %b)
// CHECK:   ret <8 x i8> [[VPADD_V_I]]
uint8x8_t test_vpadd_u8(uint8x8_t a, uint8x8_t b) {
  return vpadd_u8(a, b);
}

// CHECK-LABEL: @test_vpadd_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VPADD_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.addp.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VPADD_V3_I:%.*]] = bitcast <4 x i16> [[VPADD_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VPADD_V2_I]]
uint16x4_t test_vpadd_u16(uint16x4_t a, uint16x4_t b) {
  return vpadd_u16(a, b);
}

// CHECK-LABEL: @test_vpadd_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VPADD_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.addp.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VPADD_V3_I:%.*]] = bitcast <2 x i32> [[VPADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VPADD_V2_I]]
uint32x2_t test_vpadd_u32(uint32x2_t a, uint32x2_t b) {
  return vpadd_u32(a, b);
}

// CHECK-LABEL: @test_vpadd_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VPADD_V2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.faddp.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   [[VPADD_V3_I:%.*]] = bitcast <2 x float> [[VPADD_V2_I]] to <8 x i8>
// CHECK:   ret <2 x float> [[VPADD_V2_I]]
float32x2_t test_vpadd_f32(float32x2_t a, float32x2_t b) {
  return vpadd_f32(a, b);
}

// CHECK-LABEL: @test_vpaddq_s8(
// CHECK:   [[VPADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.addp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPADDQ_V_I]]
int8x16_t test_vpaddq_s8(int8x16_t a, int8x16_t b) {
  return vpaddq_s8(a, b);
}

// CHECK-LABEL: @test_vpaddq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.addp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VPADDQ_V2_I]]
int16x8_t test_vpaddq_s16(int16x8_t a, int16x8_t b) {
  return vpaddq_s16(a, b);
}

// CHECK-LABEL: @test_vpaddq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.addp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VPADDQ_V2_I]]
int32x4_t test_vpaddq_s32(int32x4_t a, int32x4_t b) {
  return vpaddq_s32(a, b);
}

// CHECK-LABEL: @test_vpaddq_u8(
// CHECK:   [[VPADDQ_V_I:%.*]] = call <16 x i8> @llvm.aarch64.neon.addp.v16i8(<16 x i8> %a, <16 x i8> %b)
// CHECK:   ret <16 x i8> [[VPADDQ_V_I]]
uint8x16_t test_vpaddq_u8(uint8x16_t a, uint8x16_t b) {
  return vpaddq_u8(a, b);
}

// CHECK-LABEL: @test_vpaddq_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.addp.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <8 x i16> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VPADDQ_V2_I]]
uint16x8_t test_vpaddq_u16(uint16x8_t a, uint16x8_t b) {
  return vpaddq_u16(a, b);
}

// CHECK-LABEL: @test_vpaddq_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.addp.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <4 x i32> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VPADDQ_V2_I]]
uint32x4_t test_vpaddq_u32(uint32x4_t a, uint32x4_t b) {
  return vpaddq_u32(a, b);
}

// CHECK-LABEL: @test_vpaddq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.faddp.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <4 x float> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x float> [[VPADDQ_V2_I]]
float32x4_t test_vpaddq_f32(float32x4_t a, float32x4_t b) {
  return vpaddq_f32(a, b);
}

// CHECK-LABEL: @test_vpaddq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VPADDQ_V2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.faddp.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   [[VPADDQ_V3_I:%.*]] = bitcast <2 x double> [[VPADDQ_V2_I]] to <16 x i8>
// CHECK:   ret <2 x double> [[VPADDQ_V2_I]]
float64x2_t test_vpaddq_f64(float64x2_t a, float64x2_t b) {
  return vpaddq_f64(a, b);
}

// CHECK-LABEL: @test_vqdmulh_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQDMULH_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqdmulh.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQDMULH_V3_I:%.*]] = bitcast <4 x i16> [[VQDMULH_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQDMULH_V2_I]]
int16x4_t test_vqdmulh_s16(int16x4_t a, int16x4_t b) {
  return vqdmulh_s16(a, b);
}

// CHECK-LABEL: @test_vqdmulh_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQDMULH_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqdmulh.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQDMULH_V3_I:%.*]] = bitcast <2 x i32> [[VQDMULH_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQDMULH_V2_I]]
int32x2_t test_vqdmulh_s32(int32x2_t a, int32x2_t b) {
  return vqdmulh_s32(a, b);
}

// CHECK-LABEL: @test_vqdmulhq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQDMULHQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqdmulh.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQDMULHQ_V3_I:%.*]] = bitcast <8 x i16> [[VQDMULHQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQDMULHQ_V2_I]]
int16x8_t test_vqdmulhq_s16(int16x8_t a, int16x8_t b) {
  return vqdmulhq_s16(a, b);
}

// CHECK-LABEL: @test_vqdmulhq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQDMULHQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqdmulh.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQDMULHQ_V3_I:%.*]] = bitcast <4 x i32> [[VQDMULHQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQDMULHQ_V2_I]]
int32x4_t test_vqdmulhq_s32(int32x4_t a, int32x4_t b) {
  return vqdmulhq_s32(a, b);
}

// CHECK-LABEL: @test_vqrdmulh_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VQRDMULH_V2_I:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqrdmulh.v4i16(<4 x i16> %a, <4 x i16> %b)
// CHECK:   [[VQRDMULH_V3_I:%.*]] = bitcast <4 x i16> [[VQRDMULH_V2_I]] to <8 x i8>
// CHECK:   ret <4 x i16> [[VQRDMULH_V2_I]]
int16x4_t test_vqrdmulh_s16(int16x4_t a, int16x4_t b) {
  return vqrdmulh_s16(a, b);
}

// CHECK-LABEL: @test_vqrdmulh_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VQRDMULH_V2_I:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqrdmulh.v2i32(<2 x i32> %a, <2 x i32> %b)
// CHECK:   [[VQRDMULH_V3_I:%.*]] = bitcast <2 x i32> [[VQRDMULH_V2_I]] to <8 x i8>
// CHECK:   ret <2 x i32> [[VQRDMULH_V2_I]]
int32x2_t test_vqrdmulh_s32(int32x2_t a, int32x2_t b) {
  return vqrdmulh_s32(a, b);
}

// CHECK-LABEL: @test_vqrdmulhq_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQRDMULHQ_V2_I:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqrdmulh.v8i16(<8 x i16> %a, <8 x i16> %b)
// CHECK:   [[VQRDMULHQ_V3_I:%.*]] = bitcast <8 x i16> [[VQRDMULHQ_V2_I]] to <16 x i8>
// CHECK:   ret <8 x i16> [[VQRDMULHQ_V2_I]]
int16x8_t test_vqrdmulhq_s16(int16x8_t a, int16x8_t b) {
  return vqrdmulhq_s16(a, b);
}

// CHECK-LABEL: @test_vqrdmulhq_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQRDMULHQ_V2_I:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqrdmulh.v4i32(<4 x i32> %a, <4 x i32> %b)
// CHECK:   [[VQRDMULHQ_V3_I:%.*]] = bitcast <4 x i32> [[VQRDMULHQ_V2_I]] to <16 x i8>
// CHECK:   ret <4 x i32> [[VQRDMULHQ_V2_I]]
int32x4_t test_vqrdmulhq_s32(int32x4_t a, int32x4_t b) {
  return vqrdmulhq_s32(a, b);
}

// CHECK-LABEL: @test_vmulx_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x float> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x float> %b to <8 x i8>
// CHECK:   [[VMULX2_I:%.*]] = call <2 x float> @llvm.aarch64.neon.fmulx.v2f32(<2 x float> %a, <2 x float> %b)
// CHECK:   ret <2 x float> [[VMULX2_I]]
float32x2_t test_vmulx_f32(float32x2_t a, float32x2_t b) {
  return vmulx_f32(a, b);
}

// CHECK-LABEL: @test_vmulxq_f32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x float> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x float> %b to <16 x i8>
// CHECK:   [[VMULX2_I:%.*]] = call <4 x float> @llvm.aarch64.neon.fmulx.v4f32(<4 x float> %a, <4 x float> %b)
// CHECK:   ret <4 x float> [[VMULX2_I]]
float32x4_t test_vmulxq_f32(float32x4_t a, float32x4_t b) {
  return vmulxq_f32(a, b);
}

// CHECK-LABEL: @test_vmulxq_f64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x double> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x double> %b to <16 x i8>
// CHECK:   [[VMULX2_I:%.*]] = call <2 x double> @llvm.aarch64.neon.fmulx.v2f64(<2 x double> %a, <2 x double> %b)
// CHECK:   ret <2 x double> [[VMULX2_I]]
float64x2_t test_vmulxq_f64(float64x2_t a, float64x2_t b) {
  return vmulxq_f64(a, b);
}

// CHECK-LABEL: @test_vshl_n_s8(
// CHECK:   [[VSHL_N:%.*]] = shl <8 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <8 x i8> [[VSHL_N]]
int8x8_t test_vshl_n_s8(int8x8_t a) {
  return vshl_n_s8(a, 3);
}

// CHECK-LABEL: @test_vshl_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSHL_N:%.*]] = shl <4 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <4 x i16> [[VSHL_N]]
int16x4_t test_vshl_n_s16(int16x4_t a) {
  return vshl_n_s16(a, 3);
}

// CHECK-LABEL: @test_vshl_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSHL_N:%.*]] = shl <2 x i32> [[TMP1]], <i32 3, i32 3>
// CHECK:   ret <2 x i32> [[VSHL_N]]
int32x2_t test_vshl_n_s32(int32x2_t a) {
  return vshl_n_s32(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_s8(
// CHECK:   [[VSHL_N:%.*]] = shl <16 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <16 x i8> [[VSHL_N]]
int8x16_t test_vshlq_n_s8(int8x16_t a) {
  return vshlq_n_s8(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSHL_N:%.*]] = shl <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <8 x i16> [[VSHL_N]]
int16x8_t test_vshlq_n_s16(int16x8_t a) {
  return vshlq_n_s16(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSHL_N:%.*]] = shl <4 x i32> [[TMP1]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i32> [[VSHL_N]]
int32x4_t test_vshlq_n_s32(int32x4_t a) {
  return vshlq_n_s32(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSHL_N:%.*]] = shl <2 x i64> [[TMP1]], <i64 3, i64 3>
// CHECK:   ret <2 x i64> [[VSHL_N]]
int64x2_t test_vshlq_n_s64(int64x2_t a) {
  return vshlq_n_s64(a, 3);
}

// CHECK-LABEL: @test_vshl_n_u8(
// CHECK:   [[VSHL_N:%.*]] = shl <8 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <8 x i8> [[VSHL_N]]
uint8x8_t test_vshl_n_u8(uint8x8_t a) {
  return vshl_n_u8(a, 3);
}

// CHECK-LABEL: @test_vshl_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSHL_N:%.*]] = shl <4 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <4 x i16> [[VSHL_N]]
uint16x4_t test_vshl_n_u16(uint16x4_t a) {
  return vshl_n_u16(a, 3);
}

// CHECK-LABEL: @test_vshl_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSHL_N:%.*]] = shl <2 x i32> [[TMP1]], <i32 3, i32 3>
// CHECK:   ret <2 x i32> [[VSHL_N]]
uint32x2_t test_vshl_n_u32(uint32x2_t a) {
  return vshl_n_u32(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_u8(
// CHECK:   [[VSHL_N:%.*]] = shl <16 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <16 x i8> [[VSHL_N]]
uint8x16_t test_vshlq_n_u8(uint8x16_t a) {
  return vshlq_n_u8(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSHL_N:%.*]] = shl <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <8 x i16> [[VSHL_N]]
uint16x8_t test_vshlq_n_u16(uint16x8_t a) {
  return vshlq_n_u16(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSHL_N:%.*]] = shl <4 x i32> [[TMP1]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i32> [[VSHL_N]]
uint32x4_t test_vshlq_n_u32(uint32x4_t a) {
  return vshlq_n_u32(a, 3);
}

// CHECK-LABEL: @test_vshlq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSHL_N:%.*]] = shl <2 x i64> [[TMP1]], <i64 3, i64 3>
// CHECK:   ret <2 x i64> [[VSHL_N]]
uint64x2_t test_vshlq_n_u64(uint64x2_t a) {
  return vshlq_n_u64(a, 3);
}

// CHECK-LABEL: @test_vshr_n_s8(
// CHECK:   [[VSHR_N:%.*]] = ashr <8 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <8 x i8> [[VSHR_N]]
int8x8_t test_vshr_n_s8(int8x8_t a) {
  return vshr_n_s8(a, 3);
}

// CHECK-LABEL: @test_vshr_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSHR_N:%.*]] = ashr <4 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <4 x i16> [[VSHR_N]]
int16x4_t test_vshr_n_s16(int16x4_t a) {
  return vshr_n_s16(a, 3);
}

// CHECK-LABEL: @test_vshr_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSHR_N:%.*]] = ashr <2 x i32> [[TMP1]], <i32 3, i32 3>
// CHECK:   ret <2 x i32> [[VSHR_N]]
int32x2_t test_vshr_n_s32(int32x2_t a) {
  return vshr_n_s32(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_s8(
// CHECK:   [[VSHR_N:%.*]] = ashr <16 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <16 x i8> [[VSHR_N]]
int8x16_t test_vshrq_n_s8(int8x16_t a) {
  return vshrq_n_s8(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSHR_N:%.*]] = ashr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <8 x i16> [[VSHR_N]]
int16x8_t test_vshrq_n_s16(int16x8_t a) {
  return vshrq_n_s16(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSHR_N:%.*]] = ashr <4 x i32> [[TMP1]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i32> [[VSHR_N]]
int32x4_t test_vshrq_n_s32(int32x4_t a) {
  return vshrq_n_s32(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSHR_N:%.*]] = ashr <2 x i64> [[TMP1]], <i64 3, i64 3>
// CHECK:   ret <2 x i64> [[VSHR_N]]
int64x2_t test_vshrq_n_s64(int64x2_t a) {
  return vshrq_n_s64(a, 3);
}

// CHECK-LABEL: @test_vshr_n_u8(
// CHECK:   [[VSHR_N:%.*]] = lshr <8 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <8 x i8> [[VSHR_N]]
uint8x8_t test_vshr_n_u8(uint8x8_t a) {
  return vshr_n_u8(a, 3);
}

// CHECK-LABEL: @test_vshr_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSHR_N:%.*]] = lshr <4 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <4 x i16> [[VSHR_N]]
uint16x4_t test_vshr_n_u16(uint16x4_t a) {
  return vshr_n_u16(a, 3);
}

// CHECK-LABEL: @test_vshr_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSHR_N:%.*]] = lshr <2 x i32> [[TMP1]], <i32 3, i32 3>
// CHECK:   ret <2 x i32> [[VSHR_N]]
uint32x2_t test_vshr_n_u32(uint32x2_t a) {
  return vshr_n_u32(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_u8(
// CHECK:   [[VSHR_N:%.*]] = lshr <16 x i8> %a, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   ret <16 x i8> [[VSHR_N]]
uint8x16_t test_vshrq_n_u8(uint8x16_t a) {
  return vshrq_n_u8(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSHR_N:%.*]] = lshr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   ret <8 x i16> [[VSHR_N]]
uint16x8_t test_vshrq_n_u16(uint16x8_t a) {
  return vshrq_n_u16(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSHR_N:%.*]] = lshr <4 x i32> [[TMP1]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   ret <4 x i32> [[VSHR_N]]
uint32x4_t test_vshrq_n_u32(uint32x4_t a) {
  return vshrq_n_u32(a, 3);
}

// CHECK-LABEL: @test_vshrq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSHR_N:%.*]] = lshr <2 x i64> [[TMP1]], <i64 3, i64 3>
// CHECK:   ret <2 x i64> [[VSHR_N]]
uint64x2_t test_vshrq_n_u64(uint64x2_t a) {
  return vshrq_n_u64(a, 3);
}

// CHECK-LABEL: @test_vsra_n_s8(
// CHECK:   [[VSRA_N:%.*]] = ashr <8 x i8> %b, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   [[TMP0:%.*]] = add <8 x i8> %a, [[VSRA_N]]
// CHECK:   ret <8 x i8> [[TMP0]]
int8x8_t test_vsra_n_s8(int8x8_t a, int8x8_t b) {
  return vsra_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsra_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSRA_N:%.*]] = ashr <4 x i16> [[TMP3]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[TMP4:%.*]] = add <4 x i16> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <4 x i16> [[TMP4]]
int16x4_t test_vsra_n_s16(int16x4_t a, int16x4_t b) {
  return vsra_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsra_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSRA_N:%.*]] = ashr <2 x i32> [[TMP3]], <i32 3, i32 3>
// CHECK:   [[TMP4:%.*]] = add <2 x i32> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <2 x i32> [[TMP4]]
int32x2_t test_vsra_n_s32(int32x2_t a, int32x2_t b) {
  return vsra_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_s8(
// CHECK:   [[VSRA_N:%.*]] = ashr <16 x i8> %b, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   [[TMP0:%.*]] = add <16 x i8> %a, [[VSRA_N]]
// CHECK:   ret <16 x i8> [[TMP0]]
int8x16_t test_vsraq_n_s8(int8x16_t a, int8x16_t b) {
  return vsraq_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSRA_N:%.*]] = ashr <8 x i16> [[TMP3]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[TMP4:%.*]] = add <8 x i16> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <8 x i16> [[TMP4]]
int16x8_t test_vsraq_n_s16(int16x8_t a, int16x8_t b) {
  return vsraq_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSRA_N:%.*]] = ashr <4 x i32> [[TMP3]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   [[TMP4:%.*]] = add <4 x i32> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <4 x i32> [[TMP4]]
int32x4_t test_vsraq_n_s32(int32x4_t a, int32x4_t b) {
  return vsraq_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSRA_N:%.*]] = ashr <2 x i64> [[TMP3]], <i64 3, i64 3>
// CHECK:   [[TMP4:%.*]] = add <2 x i64> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <2 x i64> [[TMP4]]
int64x2_t test_vsraq_n_s64(int64x2_t a, int64x2_t b) {
  return vsraq_n_s64(a, b, 3);
}

// CHECK-LABEL: @test_vsra_n_u8(
// CHECK:   [[VSRA_N:%.*]] = lshr <8 x i8> %b, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   [[TMP0:%.*]] = add <8 x i8> %a, [[VSRA_N]]
// CHECK:   ret <8 x i8> [[TMP0]]
uint8x8_t test_vsra_n_u8(uint8x8_t a, uint8x8_t b) {
  return vsra_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsra_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSRA_N:%.*]] = lshr <4 x i16> [[TMP3]], <i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[TMP4:%.*]] = add <4 x i16> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <4 x i16> [[TMP4]]
uint16x4_t test_vsra_n_u16(uint16x4_t a, uint16x4_t b) {
  return vsra_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsra_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSRA_N:%.*]] = lshr <2 x i32> [[TMP3]], <i32 3, i32 3>
// CHECK:   [[TMP4:%.*]] = add <2 x i32> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <2 x i32> [[TMP4]]
uint32x2_t test_vsra_n_u32(uint32x2_t a, uint32x2_t b) {
  return vsra_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_u8(
// CHECK:   [[VSRA_N:%.*]] = lshr <16 x i8> %b, <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>
// CHECK:   [[TMP0:%.*]] = add <16 x i8> %a, [[VSRA_N]]
// CHECK:   ret <16 x i8> [[TMP0]]
uint8x16_t test_vsraq_n_u8(uint8x16_t a, uint8x16_t b) {
  return vsraq_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSRA_N:%.*]] = lshr <8 x i16> [[TMP3]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[TMP4:%.*]] = add <8 x i16> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <8 x i16> [[TMP4]]
uint16x8_t test_vsraq_n_u16(uint16x8_t a, uint16x8_t b) {
  return vsraq_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSRA_N:%.*]] = lshr <4 x i32> [[TMP3]], <i32 3, i32 3, i32 3, i32 3>
// CHECK:   [[TMP4:%.*]] = add <4 x i32> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <4 x i32> [[TMP4]]
uint32x4_t test_vsraq_n_u32(uint32x4_t a, uint32x4_t b) {
  return vsraq_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsraq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSRA_N:%.*]] = lshr <2 x i64> [[TMP3]], <i64 3, i64 3>
// CHECK:   [[TMP4:%.*]] = add <2 x i64> [[TMP2]], [[VSRA_N]]
// CHECK:   ret <2 x i64> [[TMP4]]
uint64x2_t test_vsraq_n_u64(uint64x2_t a, uint64x2_t b) {
  return vsraq_n_u64(a, b, 3);
}

// CHECK-LABEL: @test_vrshr_n_s8(
// CHECK:   [[VRSHR_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.srshl.v8i8(<8 x i8> %a, <8 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   ret <8 x i8> [[VRSHR_N]]
int8x8_t test_vrshr_n_s8(int8x8_t a) {
  return vrshr_n_s8(a, 3);
}

// CHECK-LABEL: @test_vrshr_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.srshl.v4i16(<4 x i16> [[VRSHR_N]], <4 x i16> <i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   ret <4 x i16> [[VRSHR_N1]]
int16x4_t test_vrshr_n_s16(int16x4_t a) {
  return vrshr_n_s16(a, 3);
}

// CHECK-LABEL: @test_vrshr_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.srshl.v2i32(<2 x i32> [[VRSHR_N]], <2 x i32> <i32 -3, i32 -3>)
// CHECK:   ret <2 x i32> [[VRSHR_N1]]
int32x2_t test_vrshr_n_s32(int32x2_t a) {
  return vrshr_n_s32(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_s8(
// CHECK:   [[VRSHR_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.srshl.v16i8(<16 x i8> %a, <16 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   ret <16 x i8> [[VRSHR_N]]
int8x16_t test_vrshrq_n_s8(int8x16_t a) {
  return vrshrq_n_s8(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <8 x i16> @llvm.aarch64.neon.srshl.v8i16(<8 x i16> [[VRSHR_N]], <8 x i16> <i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   ret <8 x i16> [[VRSHR_N1]]
int16x8_t test_vrshrq_n_s16(int16x8_t a) {
  return vrshrq_n_s16(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i32> @llvm.aarch64.neon.srshl.v4i32(<4 x i32> [[VRSHR_N]], <4 x i32> <i32 -3, i32 -3, i32 -3, i32 -3>)
// CHECK:   ret <4 x i32> [[VRSHR_N1]]
int32x4_t test_vrshrq_n_s32(int32x4_t a) {
  return vrshrq_n_s32(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i64> @llvm.aarch64.neon.srshl.v2i64(<2 x i64> [[VRSHR_N]], <2 x i64> <i64 -3, i64 -3>)
// CHECK:   ret <2 x i64> [[VRSHR_N1]]
int64x2_t test_vrshrq_n_s64(int64x2_t a) {
  return vrshrq_n_s64(a, 3);
}

// CHECK-LABEL: @test_vrshr_n_u8(
// CHECK:   [[VRSHR_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.urshl.v8i8(<8 x i8> %a, <8 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   ret <8 x i8> [[VRSHR_N]]
uint8x8_t test_vrshr_n_u8(uint8x8_t a) {
  return vrshr_n_u8(a, 3);
}

// CHECK-LABEL: @test_vrshr_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.urshl.v4i16(<4 x i16> [[VRSHR_N]], <4 x i16> <i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   ret <4 x i16> [[VRSHR_N1]]
uint16x4_t test_vrshr_n_u16(uint16x4_t a) {
  return vrshr_n_u16(a, 3);
}

// CHECK-LABEL: @test_vrshr_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.urshl.v2i32(<2 x i32> [[VRSHR_N]], <2 x i32> <i32 -3, i32 -3>)
// CHECK:   ret <2 x i32> [[VRSHR_N1]]
uint32x2_t test_vrshr_n_u32(uint32x2_t a) {
  return vrshr_n_u32(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_u8(
// CHECK:   [[VRSHR_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.urshl.v16i8(<16 x i8> %a, <16 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   ret <16 x i8> [[VRSHR_N]]
uint8x16_t test_vrshrq_n_u8(uint8x16_t a) {
  return vrshrq_n_u8(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <8 x i16> @llvm.aarch64.neon.urshl.v8i16(<8 x i16> [[VRSHR_N]], <8 x i16> <i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   ret <8 x i16> [[VRSHR_N1]]
uint16x8_t test_vrshrq_n_u16(uint16x8_t a) {
  return vrshrq_n_u16(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i32> @llvm.aarch64.neon.urshl.v4i32(<4 x i32> [[VRSHR_N]], <4 x i32> <i32 -3, i32 -3, i32 -3, i32 -3>)
// CHECK:   ret <4 x i32> [[VRSHR_N1]]
uint32x4_t test_vrshrq_n_u32(uint32x4_t a) {
  return vrshrq_n_u32(a, 3);
}

// CHECK-LABEL: @test_vrshrq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i64> @llvm.aarch64.neon.urshl.v2i64(<2 x i64> [[VRSHR_N]], <2 x i64> <i64 -3, i64 -3>)
// CHECK:   ret <2 x i64> [[VRSHR_N1]]
uint64x2_t test_vrshrq_n_u64(uint64x2_t a) {
  return vrshrq_n_u64(a, 3);
}

// CHECK-LABEL: @test_vrsra_n_s8(
// CHECK:   [[VRSHR_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.srshl.v8i8(<8 x i8> %b, <8 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   [[TMP0:%.*]] = add <8 x i8> %a, [[VRSHR_N]]
// CHECK:   ret <8 x i8> [[TMP0]]
int8x8_t test_vrsra_n_s8(int8x8_t a, int8x8_t b) {
  return vrsra_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vrsra_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.srshl.v4i16(<4 x i16> [[VRSHR_N]], <4 x i16> <i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = add <4 x i16> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <4 x i16> [[TMP3]]
int16x4_t test_vrsra_n_s16(int16x4_t a, int16x4_t b) {
  return vrsra_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vrsra_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.srshl.v2i32(<2 x i32> [[VRSHR_N]], <2 x i32> <i32 -3, i32 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = add <2 x i32> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <2 x i32> [[TMP3]]
int32x2_t test_vrsra_n_s32(int32x2_t a, int32x2_t b) {
  return vrsra_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_s8(
// CHECK:   [[VRSHR_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.srshl.v16i8(<16 x i8> %b, <16 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   [[TMP0:%.*]] = add <16 x i8> %a, [[VRSHR_N]]
// CHECK:   ret <16 x i8> [[TMP0]]
int8x16_t test_vrsraq_n_s8(int8x16_t a, int8x16_t b) {
  return vrsraq_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <8 x i16> @llvm.aarch64.neon.srshl.v8i16(<8 x i16> [[VRSHR_N]], <8 x i16> <i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = add <8 x i16> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <8 x i16> [[TMP3]]
int16x8_t test_vrsraq_n_s16(int16x8_t a, int16x8_t b) {
  return vrsraq_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i32> @llvm.aarch64.neon.srshl.v4i32(<4 x i32> [[VRSHR_N]], <4 x i32> <i32 -3, i32 -3, i32 -3, i32 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = add <4 x i32> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <4 x i32> [[TMP3]]
int32x4_t test_vrsraq_n_s32(int32x4_t a, int32x4_t b) {
  return vrsraq_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i64> @llvm.aarch64.neon.srshl.v2i64(<2 x i64> [[VRSHR_N]], <2 x i64> <i64 -3, i64 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = add <2 x i64> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <2 x i64> [[TMP3]]
int64x2_t test_vrsraq_n_s64(int64x2_t a, int64x2_t b) {
  return vrsraq_n_s64(a, b, 3);
}

// CHECK-LABEL: @test_vrsra_n_u8(
// CHECK:   [[VRSHR_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.urshl.v8i8(<8 x i8> %b, <8 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   [[TMP0:%.*]] = add <8 x i8> %a, [[VRSHR_N]]
// CHECK:   ret <8 x i8> [[TMP0]]
uint8x8_t test_vrsra_n_u8(uint8x8_t a, uint8x8_t b) {
  return vrsra_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vrsra_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.urshl.v4i16(<4 x i16> [[VRSHR_N]], <4 x i16> <i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[TMP3:%.*]] = add <4 x i16> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <4 x i16> [[TMP3]]
uint16x4_t test_vrsra_n_u16(uint16x4_t a, uint16x4_t b) {
  return vrsra_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vrsra_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.urshl.v2i32(<2 x i32> [[VRSHR_N]], <2 x i32> <i32 -3, i32 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[TMP3:%.*]] = add <2 x i32> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <2 x i32> [[TMP3]]
uint32x2_t test_vrsra_n_u32(uint32x2_t a, uint32x2_t b) {
  return vrsra_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_u8(
// CHECK:   [[VRSHR_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.urshl.v16i8(<16 x i8> %b, <16 x i8> <i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3, i8 -3>)
// CHECK:   [[TMP0:%.*]] = add <16 x i8> %a, [[VRSHR_N]]
// CHECK:   ret <16 x i8> [[TMP0]]
uint8x16_t test_vrsraq_n_u8(uint8x16_t a, uint8x16_t b) {
  return vrsraq_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VRSHR_N1:%.*]] = call <8 x i16> @llvm.aarch64.neon.urshl.v8i16(<8 x i16> [[VRSHR_N]], <8 x i16> <i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3, i16 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP3:%.*]] = add <8 x i16> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <8 x i16> [[TMP3]]
uint16x8_t test_vrsraq_n_u16(uint16x8_t a, uint16x8_t b) {
  return vrsraq_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VRSHR_N1:%.*]] = call <4 x i32> @llvm.aarch64.neon.urshl.v4i32(<4 x i32> [[VRSHR_N]], <4 x i32> <i32 -3, i32 -3, i32 -3, i32 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP3:%.*]] = add <4 x i32> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <4 x i32> [[TMP3]]
uint32x4_t test_vrsraq_n_u32(uint32x4_t a, uint32x4_t b) {
  return vrsraq_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vrsraq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHR_N:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VRSHR_N1:%.*]] = call <2 x i64> @llvm.aarch64.neon.urshl.v2i64(<2 x i64> [[VRSHR_N]], <2 x i64> <i64 -3, i64 -3>)
// CHECK:   [[TMP2:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP3:%.*]] = add <2 x i64> [[TMP2]], [[VRSHR_N1]]
// CHECK:   ret <2 x i64> [[TMP3]]
uint64x2_t test_vrsraq_n_u64(uint64x2_t a, uint64x2_t b) {
  return vrsraq_n_u64(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_s8(
// CHECK:   [[VSRI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsri.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSRI_N]]
int8x8_t test_vsri_n_s8(int8x8_t a, int8x8_t b) {
  return vsri_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsri.v4i16(<4 x i16> [[VSRI_N]], <4 x i16> [[VSRI_N1]], i32 3)
// CHECK:   ret <4 x i16> [[VSRI_N2]]
int16x4_t test_vsri_n_s16(int16x4_t a, int16x4_t b) {
  return vsri_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSRI_N2:%.*]] = call <2 x i32> @llvm.aarch64.neon.vsri.v2i32(<2 x i32> [[VSRI_N]], <2 x i32> [[VSRI_N1]], i32 3)
// CHECK:   ret <2 x i32> [[VSRI_N2]]
int32x2_t test_vsri_n_s32(int32x2_t a, int32x2_t b) {
  return vsri_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_s8(
// CHECK:   [[VSRI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsri.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSRI_N]]
int8x16_t test_vsriq_n_s8(int8x16_t a, int8x16_t b) {
  return vsriq_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsri.v8i16(<8 x i16> [[VSRI_N]], <8 x i16> [[VSRI_N1]], i32 3)
// CHECK:   ret <8 x i16> [[VSRI_N2]]
int16x8_t test_vsriq_n_s16(int16x8_t a, int16x8_t b) {
  return vsriq_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSRI_N2:%.*]] = call <4 x i32> @llvm.aarch64.neon.vsri.v4i32(<4 x i32> [[VSRI_N]], <4 x i32> [[VSRI_N1]], i32 3)
// CHECK:   ret <4 x i32> [[VSRI_N2]]
int32x4_t test_vsriq_n_s32(int32x4_t a, int32x4_t b) {
  return vsriq_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSRI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsri.v2i64(<2 x i64> [[VSRI_N]], <2 x i64> [[VSRI_N1]], i32 3)
// CHECK:   ret <2 x i64> [[VSRI_N2]]
int64x2_t test_vsriq_n_s64(int64x2_t a, int64x2_t b) {
  return vsriq_n_s64(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_u8(
// CHECK:   [[VSRI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsri.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSRI_N]]
uint8x8_t test_vsri_n_u8(uint8x8_t a, uint8x8_t b) {
  return vsri_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsri.v4i16(<4 x i16> [[VSRI_N]], <4 x i16> [[VSRI_N1]], i32 3)
// CHECK:   ret <4 x i16> [[VSRI_N2]]
uint16x4_t test_vsri_n_u16(uint16x4_t a, uint16x4_t b) {
  return vsri_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSRI_N2:%.*]] = call <2 x i32> @llvm.aarch64.neon.vsri.v2i32(<2 x i32> [[VSRI_N]], <2 x i32> [[VSRI_N1]], i32 3)
// CHECK:   ret <2 x i32> [[VSRI_N2]]
uint32x2_t test_vsri_n_u32(uint32x2_t a, uint32x2_t b) {
  return vsri_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_u8(
// CHECK:   [[VSRI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsri.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSRI_N]]
uint8x16_t test_vsriq_n_u8(uint8x16_t a, uint8x16_t b) {
  return vsriq_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsri.v8i16(<8 x i16> [[VSRI_N]], <8 x i16> [[VSRI_N1]], i32 3)
// CHECK:   ret <8 x i16> [[VSRI_N2]]
uint16x8_t test_vsriq_n_u16(uint16x8_t a, uint16x8_t b) {
  return vsriq_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSRI_N2:%.*]] = call <4 x i32> @llvm.aarch64.neon.vsri.v4i32(<4 x i32> [[VSRI_N]], <4 x i32> [[VSRI_N1]], i32 3)
// CHECK:   ret <4 x i32> [[VSRI_N2]]
uint32x4_t test_vsriq_n_u32(uint32x4_t a, uint32x4_t b) {
  return vsriq_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSRI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsri.v2i64(<2 x i64> [[VSRI_N]], <2 x i64> [[VSRI_N1]], i32 3)
// CHECK:   ret <2 x i64> [[VSRI_N2]]
uint64x2_t test_vsriq_n_u64(uint64x2_t a, uint64x2_t b) {
  return vsriq_n_u64(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_p8(
// CHECK:   [[VSRI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsri.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSRI_N]]
poly8x8_t test_vsri_n_p8(poly8x8_t a, poly8x8_t b) {
  return vsri_n_p8(a, b, 3);
}

// CHECK-LABEL: @test_vsri_n_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsri.v4i16(<4 x i16> [[VSRI_N]], <4 x i16> [[VSRI_N1]], i32 15)
// CHECK:   ret <4 x i16> [[VSRI_N2]]
poly16x4_t test_vsri_n_p16(poly16x4_t a, poly16x4_t b) {
  return vsri_n_p16(a, b, 15);
}

// CHECK-LABEL: @test_vsriq_n_p8(
// CHECK:   [[VSRI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsri.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSRI_N]]
poly8x16_t test_vsriq_n_p8(poly8x16_t a, poly8x16_t b) {
  return vsriq_n_p8(a, b, 3);
}

// CHECK-LABEL: @test_vsriq_n_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSRI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSRI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSRI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsri.v8i16(<8 x i16> [[VSRI_N]], <8 x i16> [[VSRI_N1]], i32 15)
// CHECK:   ret <8 x i16> [[VSRI_N2]]
poly16x8_t test_vsriq_n_p16(poly16x8_t a, poly16x8_t b) {
  return vsriq_n_p16(a, b, 15);
}

// CHECK-LABEL: @test_vsli_n_s8(
// CHECK:   [[VSLI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsli.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSLI_N]]
int8x8_t test_vsli_n_s8(int8x8_t a, int8x8_t b) {
  return vsli_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsli.v4i16(<4 x i16> [[VSLI_N]], <4 x i16> [[VSLI_N1]], i32 3)
// CHECK:   ret <4 x i16> [[VSLI_N2]]
int16x4_t test_vsli_n_s16(int16x4_t a, int16x4_t b) {
  return vsli_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSLI_N2:%.*]] = call <2 x i32> @llvm.aarch64.neon.vsli.v2i32(<2 x i32> [[VSLI_N]], <2 x i32> [[VSLI_N1]], i32 3)
// CHECK:   ret <2 x i32> [[VSLI_N2]]
int32x2_t test_vsli_n_s32(int32x2_t a, int32x2_t b) {
  return vsli_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_s8(
// CHECK:   [[VSLI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsli.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSLI_N]]
int8x16_t test_vsliq_n_s8(int8x16_t a, int8x16_t b) {
  return vsliq_n_s8(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsli.v8i16(<8 x i16> [[VSLI_N]], <8 x i16> [[VSLI_N1]], i32 3)
// CHECK:   ret <8 x i16> [[VSLI_N2]]
int16x8_t test_vsliq_n_s16(int16x8_t a, int16x8_t b) {
  return vsliq_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSLI_N2:%.*]] = call <4 x i32> @llvm.aarch64.neon.vsli.v4i32(<4 x i32> [[VSLI_N]], <4 x i32> [[VSLI_N1]], i32 3)
// CHECK:   ret <4 x i32> [[VSLI_N2]]
int32x4_t test_vsliq_n_s32(int32x4_t a, int32x4_t b) {
  return vsliq_n_s32(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSLI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsli.v2i64(<2 x i64> [[VSLI_N]], <2 x i64> [[VSLI_N1]], i32 3)
// CHECK:   ret <2 x i64> [[VSLI_N2]]
int64x2_t test_vsliq_n_s64(int64x2_t a, int64x2_t b) {
  return vsliq_n_s64(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_u8(
// CHECK:   [[VSLI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsli.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSLI_N]]
uint8x8_t test_vsli_n_u8(uint8x8_t a, uint8x8_t b) {
  return vsli_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsli.v4i16(<4 x i16> [[VSLI_N]], <4 x i16> [[VSLI_N1]], i32 3)
// CHECK:   ret <4 x i16> [[VSLI_N2]]
uint16x4_t test_vsli_n_u16(uint16x4_t a, uint16x4_t b) {
  return vsli_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i32> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <2 x i32>
// CHECK:   [[VSLI_N2:%.*]] = call <2 x i32> @llvm.aarch64.neon.vsli.v2i32(<2 x i32> [[VSLI_N]], <2 x i32> [[VSLI_N1]], i32 3)
// CHECK:   ret <2 x i32> [[VSLI_N2]]
uint32x2_t test_vsli_n_u32(uint32x2_t a, uint32x2_t b) {
  return vsli_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_u8(
// CHECK:   [[VSLI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsli.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSLI_N]]
uint8x16_t test_vsliq_n_u8(uint8x16_t a, uint8x16_t b) {
  return vsliq_n_u8(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsli.v8i16(<8 x i16> [[VSLI_N]], <8 x i16> [[VSLI_N1]], i32 3)
// CHECK:   ret <8 x i16> [[VSLI_N2]]
uint16x8_t test_vsliq_n_u16(uint16x8_t a, uint16x8_t b) {
  return vsliq_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <4 x i32>
// CHECK:   [[VSLI_N2:%.*]] = call <4 x i32> @llvm.aarch64.neon.vsli.v4i32(<4 x i32> [[VSLI_N]], <4 x i32> [[VSLI_N1]], i32 3)
// CHECK:   ret <4 x i32> [[VSLI_N2]]
uint32x4_t test_vsliq_n_u32(uint32x4_t a, uint32x4_t b) {
  return vsliq_n_u32(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <2 x i64>
// CHECK:   [[VSLI_N2:%.*]] = call <2 x i64> @llvm.aarch64.neon.vsli.v2i64(<2 x i64> [[VSLI_N]], <2 x i64> [[VSLI_N1]], i32 3)
// CHECK:   ret <2 x i64> [[VSLI_N2]]
uint64x2_t test_vsliq_n_u64(uint64x2_t a, uint64x2_t b) {
  return vsliq_n_u64(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_p8(
// CHECK:   [[VSLI_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.vsli.v8i8(<8 x i8> %a, <8 x i8> %b, i32 3)
// CHECK:   ret <8 x i8> [[VSLI_N]]
poly8x8_t test_vsli_n_p8(poly8x8_t a, poly8x8_t b) {
  return vsli_n_p8(a, b, 3);
}

// CHECK-LABEL: @test_vsli_n_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <4 x i16> %b to <8 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <8 x i8> [[TMP1]] to <4 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <4 x i16> @llvm.aarch64.neon.vsli.v4i16(<4 x i16> [[VSLI_N]], <4 x i16> [[VSLI_N1]], i32 15)
// CHECK:   ret <4 x i16> [[VSLI_N2]]
poly16x4_t test_vsli_n_p16(poly16x4_t a, poly16x4_t b) {
  return vsli_n_p16(a, b, 15);
}

// CHECK-LABEL: @test_vsliq_n_p8(
// CHECK:   [[VSLI_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.vsli.v16i8(<16 x i8> %a, <16 x i8> %b, i32 3)
// CHECK:   ret <16 x i8> [[VSLI_N]]
poly8x16_t test_vsliq_n_p8(poly8x16_t a, poly8x16_t b) {
  return vsliq_n_p8(a, b, 3);
}

// CHECK-LABEL: @test_vsliq_n_p16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VSLI_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VSLI_N1:%.*]] = bitcast <16 x i8> [[TMP1]] to <8 x i16>
// CHECK:   [[VSLI_N2:%.*]] = call <8 x i16> @llvm.aarch64.neon.vsli.v8i16(<8 x i16> [[VSLI_N]], <8 x i16> [[VSLI_N1]], i32 15)
// CHECK:   ret <8 x i16> [[VSLI_N2]]
poly16x8_t test_vsliq_n_p16(poly16x8_t a, poly16x8_t b) {
  return vsliq_n_p16(a, b, 15);
}

// CHECK-LABEL: @test_vqshlu_n_s8(
// CHECK:   [[VQSHLU_N:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshlu.v8i8(<8 x i8> %a, <8 x i8> <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>)
// CHECK:   ret <8 x i8> [[VQSHLU_N]]
uint8x8_t test_vqshlu_n_s8(int8x8_t a) {
  return vqshlu_n_s8(a, 3);
}

// CHECK-LABEL: @test_vqshlu_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i16> %a to <8 x i8>
// CHECK:   [[VQSHLU_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <4 x i16>
// CHECK:   [[VQSHLU_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshlu.v4i16(<4 x i16> [[VQSHLU_N]], <4 x i16> <i16 3, i16 3, i16 3, i16 3>)
// CHECK:   ret <4 x i16> [[VQSHLU_N1]]
uint16x4_t test_vqshlu_n_s16(int16x4_t a) {
  return vqshlu_n_s16(a, 3);
}

// CHECK-LABEL: @test_vqshlu_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i32> %a to <8 x i8>
// CHECK:   [[VQSHLU_N:%.*]] = bitcast <8 x i8> [[TMP0]] to <2 x i32>
// CHECK:   [[VQSHLU_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshlu.v2i32(<2 x i32> [[VQSHLU_N]], <2 x i32> <i32 3, i32 3>)
// CHECK:   ret <2 x i32> [[VQSHLU_N1]]
uint32x2_t test_vqshlu_n_s32(int32x2_t a) {
  return vqshlu_n_s32(a, 3);
}

// CHECK-LABEL: @test_vqshluq_n_s8(
// CHECK:   [[VQSHLU_N:%.*]] = call <16 x i8> @llvm.aarch64.neon.sqshlu.v16i8(<16 x i8> %a, <16 x i8> <i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3, i8 3>)
// CHECK:   ret <16 x i8> [[VQSHLU_N]]
uint8x16_t test_vqshluq_n_s8(int8x16_t a) {
  return vqshluq_n_s8(a, 3);
}

// CHECK-LABEL: @test_vqshluq_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VQSHLU_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHLU_N1:%.*]] = call <8 x i16> @llvm.aarch64.neon.sqshlu.v8i16(<8 x i16> [[VQSHLU_N]], <8 x i16> <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>)
// CHECK:   ret <8 x i16> [[VQSHLU_N1]]
uint16x8_t test_vqshluq_n_s16(int16x8_t a) {
  return vqshluq_n_s16(a, 3);
}

// CHECK-LABEL: @test_vqshluq_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VQSHLU_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHLU_N1:%.*]] = call <4 x i32> @llvm.aarch64.neon.sqshlu.v4i32(<4 x i32> [[VQSHLU_N]], <4 x i32> <i32 3, i32 3, i32 3, i32 3>)
// CHECK:   ret <4 x i32> [[VQSHLU_N1]]
uint32x4_t test_vqshluq_n_s32(int32x4_t a) {
  return vqshluq_n_s32(a, 3);
}

// CHECK-LABEL: @test_vqshluq_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VQSHLU_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHLU_N1:%.*]] = call <2 x i64> @llvm.aarch64.neon.sqshlu.v2i64(<2 x i64> [[VQSHLU_N]], <2 x i64> <i64 3, i64 3>)
// CHECK:   ret <2 x i64> [[VQSHLU_N1]]
uint64x2_t test_vqshluq_n_s64(int64x2_t a) {
  return vqshluq_n_s64(a, 3);
}

// CHECK-LABEL: @test_vshrn_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP2:%.*]] = ashr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[VSHRN_N:%.*]] = trunc <8 x i16> [[TMP2]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VSHRN_N]]
int8x8_t test_vshrn_n_s16(int16x8_t a) {
  return vshrn_n_s16(a, 3);
}

// CHECK-LABEL: @test_vshrn_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP2:%.*]] = ashr <4 x i32> [[TMP1]], <i32 9, i32 9, i32 9, i32 9>
// CHECK:   [[VSHRN_N:%.*]] = trunc <4 x i32> [[TMP2]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VSHRN_N]]
int16x4_t test_vshrn_n_s32(int32x4_t a) {
  return vshrn_n_s32(a, 9);
}

// CHECK-LABEL: @test_vshrn_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP2:%.*]] = ashr <2 x i64> [[TMP1]], <i64 19, i64 19>
// CHECK:   [[VSHRN_N:%.*]] = trunc <2 x i64> [[TMP2]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VSHRN_N]]
int32x2_t test_vshrn_n_s64(int64x2_t a) {
  return vshrn_n_s64(a, 19);
}

// CHECK-LABEL: @test_vshrn_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP2:%.*]] = lshr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[VSHRN_N:%.*]] = trunc <8 x i16> [[TMP2]] to <8 x i8>
// CHECK:   ret <8 x i8> [[VSHRN_N]]
uint8x8_t test_vshrn_n_u16(uint16x8_t a) {
  return vshrn_n_u16(a, 3);
}

// CHECK-LABEL: @test_vshrn_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP2:%.*]] = lshr <4 x i32> [[TMP1]], <i32 9, i32 9, i32 9, i32 9>
// CHECK:   [[VSHRN_N:%.*]] = trunc <4 x i32> [[TMP2]] to <4 x i16>
// CHECK:   ret <4 x i16> [[VSHRN_N]]
uint16x4_t test_vshrn_n_u32(uint32x4_t a) {
  return vshrn_n_u32(a, 9);
}

// CHECK-LABEL: @test_vshrn_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP2:%.*]] = lshr <2 x i64> [[TMP1]], <i64 19, i64 19>
// CHECK:   [[VSHRN_N:%.*]] = trunc <2 x i64> [[TMP2]] to <2 x i32>
// CHECK:   ret <2 x i32> [[VSHRN_N]]
uint32x2_t test_vshrn_n_u64(uint64x2_t a) {
  return vshrn_n_u64(a, 19);
}

// CHECK-LABEL: @test_vshrn_high_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP2:%.*]] = ashr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[VSHRN_N:%.*]] = trunc <8 x i16> [[TMP2]] to <8 x i8>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VSHRN_N]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vshrn_high_n_s16(int8x8_t a, int16x8_t b) {
  return vshrn_high_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vshrn_high_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP2:%.*]] = ashr <4 x i32> [[TMP1]], <i32 9, i32 9, i32 9, i32 9>
// CHECK:   [[VSHRN_N:%.*]] = trunc <4 x i32> [[TMP2]] to <4 x i16>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VSHRN_N]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vshrn_high_n_s32(int16x4_t a, int32x4_t b) {
  return vshrn_high_n_s32(a, b, 9);
}

// CHECK-LABEL: @test_vshrn_high_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP2:%.*]] = ashr <2 x i64> [[TMP1]], <i64 19, i64 19>
// CHECK:   [[VSHRN_N:%.*]] = trunc <2 x i64> [[TMP2]] to <2 x i32>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VSHRN_N]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vshrn_high_n_s64(int32x2_t a, int64x2_t b) {
  return vshrn_high_n_s64(a, b, 19);
}

// CHECK-LABEL: @test_vshrn_high_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[TMP2:%.*]] = lshr <8 x i16> [[TMP1]], <i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3, i16 3>
// CHECK:   [[VSHRN_N:%.*]] = trunc <8 x i16> [[TMP2]] to <8 x i8>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VSHRN_N]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
uint8x16_t test_vshrn_high_n_u16(uint8x8_t a, uint16x8_t b) {
  return vshrn_high_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vshrn_high_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[TMP2:%.*]] = lshr <4 x i32> [[TMP1]], <i32 9, i32 9, i32 9, i32 9>
// CHECK:   [[VSHRN_N:%.*]] = trunc <4 x i32> [[TMP2]] to <4 x i16>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VSHRN_N]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
uint16x8_t test_vshrn_high_n_u32(uint16x4_t a, uint32x4_t b) {
  return vshrn_high_n_u32(a, b, 9);
}

// CHECK-LABEL: @test_vshrn_high_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[TMP1:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[TMP2:%.*]] = lshr <2 x i64> [[TMP1]], <i64 19, i64 19>
// CHECK:   [[VSHRN_N:%.*]] = trunc <2 x i64> [[TMP2]] to <2 x i32>
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VSHRN_N]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
uint32x4_t test_vshrn_high_n_u64(uint32x2_t a, uint64x2_t b) {
  return vshrn_high_n_u64(a, b, 19);
}

// CHECK-LABEL: @test_vqshrun_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshrun.v8i8(<8 x i16> [[VQSHRUN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VQSHRUN_N1]]
uint8x8_t test_vqshrun_n_s16(int16x8_t a) {
  return vqshrun_n_s16(a, 3);
}

// CHECK-LABEL: @test_vqshrun_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshrun.v4i16(<4 x i32> [[VQSHRUN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VQSHRUN_N1]]
uint16x4_t test_vqshrun_n_s32(int32x4_t a) {
  return vqshrun_n_s32(a, 9);
}

// CHECK-LABEL: @test_vqshrun_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshrun.v2i32(<2 x i64> [[VQSHRUN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VQSHRUN_N1]]
uint32x2_t test_vqshrun_n_s64(int64x2_t a) {
  return vqshrun_n_s64(a, 19);
}

// CHECK-LABEL: @test_vqshrun_high_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshrun.v8i8(<8 x i16> [[VQSHRUN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VQSHRUN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vqshrun_high_n_s16(int8x8_t a, int16x8_t b) {
  return vqshrun_high_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vqshrun_high_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshrun.v4i16(<4 x i32> [[VQSHRUN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VQSHRUN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vqshrun_high_n_s32(int16x4_t a, int32x4_t b) {
  return vqshrun_high_n_s32(a, b, 9);
}

// CHECK-LABEL: @test_vqshrun_high_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRUN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshrun.v2i32(<2 x i64> [[VQSHRUN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VQSHRUN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vqshrun_high_n_s64(int32x2_t a, int64x2_t b) {
  return vqshrun_high_n_s64(a, b, 19);
}

// CHECK-LABEL: @test_vrshrn_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.rshrn.v8i8(<8 x i16> [[VRSHRN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VRSHRN_N1]]
int8x8_t test_vrshrn_n_s16(int16x8_t a) {
  return vrshrn_n_s16(a, 3);
}

// CHECK-LABEL: @test_vrshrn_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.rshrn.v4i16(<4 x i32> [[VRSHRN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VRSHRN_N1]]
int16x4_t test_vrshrn_n_s32(int32x4_t a) {
  return vrshrn_n_s32(a, 9);
}

// CHECK-LABEL: @test_vrshrn_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.rshrn.v2i32(<2 x i64> [[VRSHRN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VRSHRN_N1]]
int32x2_t test_vrshrn_n_s64(int64x2_t a) {
  return vrshrn_n_s64(a, 19);
}

// CHECK-LABEL: @test_vrshrn_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.rshrn.v8i8(<8 x i16> [[VRSHRN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VRSHRN_N1]]
uint8x8_t test_vrshrn_n_u16(uint16x8_t a) {
  return vrshrn_n_u16(a, 3);
}

// CHECK-LABEL: @test_vrshrn_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.rshrn.v4i16(<4 x i32> [[VRSHRN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VRSHRN_N1]]
uint16x4_t test_vrshrn_n_u32(uint32x4_t a) {
  return vrshrn_n_u32(a, 9);
}

// CHECK-LABEL: @test_vrshrn_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.rshrn.v2i32(<2 x i64> [[VRSHRN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VRSHRN_N1]]
uint32x2_t test_vrshrn_n_u64(uint64x2_t a) {
  return vrshrn_n_u64(a, 19);
}

// CHECK-LABEL: @test_vrshrn_high_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.rshrn.v8i8(<8 x i16> [[VRSHRN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VRSHRN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vrshrn_high_n_s16(int8x8_t a, int16x8_t b) {
  return vrshrn_high_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vrshrn_high_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.rshrn.v4i16(<4 x i32> [[VRSHRN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VRSHRN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vrshrn_high_n_s32(int16x4_t a, int32x4_t b) {
  return vrshrn_high_n_s32(a, b, 9);
}

// CHECK-LABEL: @test_vrshrn_high_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.rshrn.v2i32(<2 x i64> [[VRSHRN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VRSHRN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vrshrn_high_n_s64(int32x2_t a, int64x2_t b) {
  return vrshrn_high_n_s64(a, b, 19);
}

// CHECK-LABEL: @test_vrshrn_high_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VRSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.rshrn.v8i8(<8 x i16> [[VRSHRN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VRSHRN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
uint8x16_t test_vrshrn_high_n_u16(uint8x8_t a, uint16x8_t b) {
  return vrshrn_high_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vrshrn_high_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VRSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.rshrn.v4i16(<4 x i32> [[VRSHRN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VRSHRN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
uint16x8_t test_vrshrn_high_n_u32(uint16x4_t a, uint32x4_t b) {
  return vrshrn_high_n_u32(a, b, 9);
}

// CHECK-LABEL: @test_vrshrn_high_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VRSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VRSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.rshrn.v2i32(<2 x i64> [[VRSHRN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VRSHRN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
uint32x4_t test_vrshrn_high_n_u64(uint32x2_t a, uint64x2_t b) {
  return vrshrn_high_n_u64(a, b, 19);
}

// CHECK-LABEL: @test_vqrshrun_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqrshrun.v8i8(<8 x i16> [[VQRSHRUN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VQRSHRUN_N1]]
uint8x8_t test_vqrshrun_n_s16(int16x8_t a) {
  return vqrshrun_n_s16(a, 3);
}

// CHECK-LABEL: @test_vqrshrun_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqrshrun.v4i16(<4 x i32> [[VQRSHRUN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VQRSHRUN_N1]]
uint16x4_t test_vqrshrun_n_s32(int32x4_t a) {
  return vqrshrun_n_s32(a, 9);
}

// CHECK-LABEL: @test_vqrshrun_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqrshrun.v2i32(<2 x i64> [[VQRSHRUN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VQRSHRUN_N1]]
uint32x2_t test_vqrshrun_n_s64(int64x2_t a) {
  return vqrshrun_n_s64(a, 19);
}

// CHECK-LABEL: @test_vqrshrun_high_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqrshrun.v8i8(<8 x i16> [[VQRSHRUN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VQRSHRUN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vqrshrun_high_n_s16(int8x8_t a, int16x8_t b) {
  return vqrshrun_high_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vqrshrun_high_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqrshrun.v4i16(<4 x i32> [[VQRSHRUN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VQRSHRUN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vqrshrun_high_n_s32(int16x4_t a, int32x4_t b) {
  return vqrshrun_high_n_s32(a, b, 9);
}

// CHECK-LABEL: @test_vqrshrun_high_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQRSHRUN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQRSHRUN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqrshrun.v2i32(<2 x i64> [[VQRSHRUN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VQRSHRUN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vqrshrun_high_n_s64(int32x2_t a, int64x2_t b) {
  return vqrshrun_high_n_s64(a, b, 19);
}

// CHECK-LABEL: @test_vqshrn_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshrn.v8i8(<8 x i16> [[VQSHRN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VQSHRN_N1]]
int8x8_t test_vqshrn_n_s16(int16x8_t a) {
  return vqshrn_n_s16(a, 3);
}

// CHECK-LABEL: @test_vqshrn_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshrn.v4i16(<4 x i32> [[VQSHRN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VQSHRN_N1]]
int16x4_t test_vqshrn_n_s32(int32x4_t a) {
  return vqshrn_n_s32(a, 9);
}

// CHECK-LABEL: @test_vqshrn_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshrn.v2i32(<2 x i64> [[VQSHRN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VQSHRN_N1]]
int32x2_t test_vqshrn_n_s64(int64x2_t a) {
  return vqshrn_n_s64(a, 19);
}

// CHECK-LABEL: @test_vqshrn_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqshrn.v8i8(<8 x i16> [[VQSHRN_N]], i32 3)
// CHECK:   ret <8 x i8> [[VQSHRN_N1]]
uint8x8_t test_vqshrn_n_u16(uint16x8_t a) {
  return vqshrn_n_u16(a, 3);
}

// CHECK-LABEL: @test_vqshrn_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqshrn.v4i16(<4 x i32> [[VQSHRN_N]], i32 9)
// CHECK:   ret <4 x i16> [[VQSHRN_N1]]
uint16x4_t test_vqshrn_n_u32(uint32x4_t a) {
  return vqshrn_n_u32(a, 9);
}

// CHECK-LABEL: @test_vqshrn_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %a to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqshrn.v2i32(<2 x i64> [[VQSHRN_N]], i32 19)
// CHECK:   ret <2 x i32> [[VQSHRN_N1]]
uint32x2_t test_vqshrn_n_u64(uint64x2_t a) {
  return vqshrn_n_u64(a, 19);
}

// CHECK-LABEL: @test_vqshrn_high_n_s16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.sqshrn.v8i8(<8 x i16> [[VQSHRN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VQSHRN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
int8x16_t test_vqshrn_high_n_s16(int8x8_t a, int16x8_t b) {
  return vqshrn_high_n_s16(a, b, 3);
}

// CHECK-LABEL: @test_vqshrn_high_n_s32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.sqshrn.v4i16(<4 x i32> [[VQSHRN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VQSHRN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
int16x8_t test_vqshrn_high_n_s32(int16x4_t a, int32x4_t b) {
  return vqshrn_high_n_s32(a, b, 9);
}

// CHECK-LABEL: @test_vqshrn_high_n_s64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.sqshrn.v2i32(<2 x i64> [[VQSHRN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VQSHRN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
int32x4_t test_vqshrn_high_n_s64(int32x2_t a, int64x2_t b) {
  return vqshrn_high_n_s64(a, b, 19);
}

// CHECK-LABEL: @test_vqshrn_high_n_u16(
// CHECK:   [[TMP0:%.*]] = bitcast <8 x i16> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <8 x i16>
// CHECK:   [[VQSHRN_N1:%.*]] = call <8 x i8> @llvm.aarch64.neon.uqshrn.v8i8(<8 x i16> [[VQSHRN_N]], i32 3)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <8 x i8> %a, <8 x i8> [[VQSHRN_N1]], <16 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 15>
// CHECK:   ret <16 x i8> [[SHUFFLE_I]]
uint8x16_t test_vqshrn_high_n_u16(uint8x8_t a, uint16x8_t b) {
  return vqshrn_high_n_u16(a, b, 3);
}

// CHECK-LABEL: @test_vqshrn_high_n_u32(
// CHECK:   [[TMP0:%.*]] = bitcast <4 x i32> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <4 x i32>
// CHECK:   [[VQSHRN_N1:%.*]] = call <4 x i16> @llvm.aarch64.neon.uqshrn.v4i16(<4 x i32> [[VQSHRN_N]], i32 9)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <4 x i16> %a, <4 x i16> [[VQSHRN_N1]], <8 x i32> <i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7>
// CHECK:   ret <8 x i16> [[SHUFFLE_I]]
uint16x8_t test_vqshrn_high_n_u32(uint16x4_t a, uint32x4_t b) {
  return vqshrn_high_n_u32(a, b, 9);
}

// CHECK-LABEL: @test_vqshrn_high_n_u64(
// CHECK:   [[TMP0:%.*]] = bitcast <2 x i64> %b to <16 x i8>
// CHECK:   [[VQSHRN_N:%.*]] = bitcast <16 x i8> [[TMP0]] to <2 x i64>
// CHECK:   [[VQSHRN_N1:%.*]] = call <2 x i32> @llvm.aarch64.neon.uqshrn.v2i32(<2 x i64> [[VQSHRN_N]], i32 19)
// CHECK:   [[SHUFFLE_I:%.*]] = shufflevector <2 x i32> %a, <2 x i32> [[VQSHRN_N1]], <4 x i32> <i32 0, i32 1, i32 2, i32 3>
// CHECK:   ret <4 x i32> [[SHUFFLE_I]]
uint32x4_t test_vqshrn_high_n_u64(uint32x2_t a, uint64x2_t b) {
  return vqshrn_high_n_u64(a, b, 19);
}

// C