/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cglm.h"
#include "call.h"

 
void
glmc_mat4_ucopy(mat4 mat, mat4 dest) {
  glm_mat4_copy(mat, dest);
}

 
void
glmc_mat4_copy(mat4 mat, mat4 dest) {
  glm_mat4_copy(mat, dest);
}

 
void
glmc_mat4_identity(mat4 mat) {
  glm_mat4_identity(mat);
}

 
void
glmc_mat4_identity_array(mat4 * __restrict mat, size_t count) {
  glm_mat4_identity_array(mat, count);
}

 
void
glmc_mat4_zero(mat4 mat) {
  glm_mat4_zero(mat);
}

 
void
glmc_mat4_pick3(mat4 mat, mat3 dest) {
  glm_mat4_pick3(mat, dest);
}

 
void
glmc_mat4_pick3t(mat4 mat, mat3 dest) {
  glm_mat4_pick3t(mat, dest);
}

 
void
glmc_mat4_ins3(mat3 mat, mat4 dest) {
  glm_mat4_ins3(mat, dest);
}

 
void
glmc_mat4_mul(mat4 m1, mat4 m2, mat4 dest) {
  glm_mat4_mul(m1, m2, dest);
}

 
void
glmc_mat4_mulN(mat4 * __restrict matrices[], uint32_t len, mat4 dest) {
  glm_mat4_mulN(matrices, len, dest);
}

 
void
glmc_mat4_mulv(mat4 m, vec4 v, vec4 dest) {
  glm_mat4_mulv(m, v, dest);
}

 
void
glmc_mat4_mulv3(mat4 m, vec3 v, float last, vec3 dest) {
  glm_mat4_mulv3(m, v, last, dest);
}

 
float
glmc_mat4_trace(mat4 m) {
  return glm_mat4_trace(m);
}

 
float
glmc_mat4_trace3(mat4 m) {
  return glm_mat4_trace3(m);
}

 
void
glmc_mat4_quat(mat4 m, versor dest) {
  glm_mat4_quat(m, dest);
}

 
void
glmc_mat4_transpose_to(mat4 m, mat4 dest) {
  glm_mat4_transpose_to(m, dest);
}

 
void
glmc_mat4_transpose(mat4 m) {
  glm_mat4_transpose(m);
}

 
void
glmc_mat4_scale_p(mat4 m, float s) {
  glm_mat4_scale_p(m, s);
}

 
void
glmc_mat4_scale(mat4 m, float s) {
  glm_mat4_scale(m, s);
}

 
float
glmc_mat4_det(mat4 mat) {
  return glm_mat4_det(mat);
}

 
void
glmc_mat4_inv(mat4 mat, mat4 dest) {
  glm_mat4_inv(mat, dest);
}

 
void
glmc_mat4_inv_precise(mat4 mat, mat4 dest) {
  glm_mat4_inv_precise(mat, dest);
}

 
void
glmc_mat4_inv_fast(mat4 mat, mat4 dest) {
  glm_mat4_inv_fast(mat, dest);
}

 
void
glmc_mat4_swap_col(mat4 mat, int col1, int col2) {
  glm_mat4_swap_col(mat, col1, col2);
}

 
void
glmc_mat4_swap_row(mat4 mat, int row1, int row2) {
  glm_mat4_swap_row(mat, row1, row2);
}

 
float
glmc_mat4_rmc(vec4 r, mat4 m, vec4 c) {
  return glm_mat4_rmc(r, m, c);
}
