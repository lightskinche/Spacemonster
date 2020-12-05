/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cglm.h"
#include "call.h"

 
void
glmc_vec3(vec4 v4, vec3 dest) {
  glm_vec3(v4, dest);
}

 
void
glmc_vec3_copy(vec3 a, vec3 dest) {
  glm_vec3_copy(a, dest);
}

 
void
glmc_vec3_zero(vec3 v) {
  glm_vec3_zero(v);
}

 
void
glmc_vec3_one(vec3 v) {
  glm_vec3_one(v);
}

 
float
glmc_vec3_dot(vec3 a, vec3 b) {
  return glm_vec3_dot(a, b);
}

 
void
glmc_vec3_cross(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_cross(a, b, dest);
}

 
void
glmc_vec3_crossn(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_crossn(a, b, dest);
}

 
float
glmc_vec3_norm(vec3 v) {
  return glm_vec3_norm(v);
}

 
void
glmc_vec3_normalize_to(vec3 v, vec3 dest) {
  glm_vec3_normalize_to(v, dest);
}

 
void
glmc_vec3_normalize(vec3 v) {
  glm_vec3_normalize(v);
}

 
float
glmc_vec3_norm2(vec3 v) {
  return glm_vec3_norm2(v);
}

 
float
glmc_vec3_norm_one(vec3 v) {
  return glm_vec3_norm_one(v);
}

 
float
glmc_vec3_norm_inf(vec3 v) {
  return glm_vec3_norm_inf(v);
}

 
void
glmc_vec3_add(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_add(a, b, dest);
}

 
void
glmc_vec3_adds(vec3 v, float s, vec3 dest) {
  glm_vec3_adds(v, s, dest);
}

 
void
glmc_vec3_sub(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_sub(a, b, dest);
}

 
void
glmc_vec3_subs(vec3 v, float s, vec3 dest) {
  glm_vec3_subs(v, s, dest);
}

 
void
glmc_vec3_mul(vec3 a, vec3 b, vec3 d) {
  glm_vec3_mul(a, b, d);
}

 
void
glmc_vec3_scale(vec3 v, float s, vec3 dest) {
  glm_vec3_scale(v, s, dest);
}

 
void
glmc_vec3_scale_as(vec3 v, float s, vec3 dest) {
  glm_vec3_scale_as(v, s, dest);
}

 
void
glmc_vec3_div(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_div(a, b, dest);
}

 
void
glmc_vec3_divs(vec3 a, float s, vec3 dest) {
  glm_vec3_divs(a, s, dest);
}

 
void
glmc_vec3_addadd(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_addadd(a, b, dest);
}

 
void
glmc_vec3_subadd(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_subadd(a, b, dest);
}

 
void
glmc_vec3_muladd(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_muladd(a, b, dest);
}

 
void
glmc_vec3_muladds(vec3 a, float s, vec3 dest) {
  glm_vec3_muladds(a, s, dest);
}

 
void
glmc_vec3_maxadd(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_maxadd(a, b, dest);
}

 
void
glmc_vec3_minadd(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_minadd(a, b, dest);
}

 
void
glmc_vec3_negate(vec3 v) {
  glm_vec3_negate(v);
}

 
void
glmc_vec3_negate_to(vec3 v, vec3 dest) {
  glm_vec3_negate_to(v, dest);
}

 
float
glmc_vec3_angle(vec3 a, vec3 b) {
  return glm_vec3_angle(a, b);
}

 
void
glmc_vec3_rotate(vec3 v, float angle, vec3 axis) {
  glm_vec3_rotate(v, angle, axis);
}

 
void
glmc_vec3_rotate_m4(mat4 m, vec3 v, vec3 dest) {
  glm_vec3_rotate_m4(m, v, dest);
}

 
void
glmc_vec3_rotate_m3(mat3 m, vec3 v, vec3 dest) {
  glm_vec3_rotate_m3(m, v, dest);
}

 
void
glmc_vec3_proj(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_proj(a, b, dest);
}

 
void
glmc_vec3_center(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_center(a, b, dest);
}

 
float
glmc_vec3_distance(vec3 a, vec3 b) {
  return glm_vec3_distance(a, b);
}

 
float
glmc_vec3_distance2(vec3 a, vec3 b) {
  return glm_vec3_distance2(a, b);
}

 
void
glmc_vec3_maxv(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_maxv(a, b, dest);
}

 
void
glmc_vec3_minv(vec3 a, vec3 b, vec3 dest) {
  glm_vec3_minv(a, b, dest);
}

 
void
glmc_vec3_clamp(vec3 v, float minVal, float maxVal) {
  glm_vec3_clamp(v, minVal, maxVal);
}

 
void
glmc_vec3_ortho(vec3 v, vec3 dest) {
  glm_vec3_ortho(v, dest);
}

 
void
glmc_vec3_lerp(vec3 from, vec3 to, float t, vec3 dest) {
  glm_vec3_lerp(from, to, t, dest);
}

 
void
glmc_vec3_lerpc(vec3 from, vec3 to, float t, vec3 dest) {
  glm_vec3_lerpc(from, to, t, dest);
}

 
void
glmc_vec3_step_uni(float edge, vec3 x, vec3 dest) {
  glm_vec3_step_uni(edge, x, dest);
}

 
void
glmc_vec3_step(vec3 edge, vec3 x, vec3 dest) {
  glm_vec3_step(edge, x, dest);
}

 
void
glmc_vec3_smoothstep_uni(float edge0, float edge1, vec3 x, vec3 dest) {
  glm_vec3_smoothstep_uni(edge0, edge1, x, dest);
}

 
void
glmc_vec3_smoothstep(vec3 edge0, vec3 edge1, vec3 x, vec3 dest) {
  glm_vec3_smoothstep(edge0, edge1, x, dest);
}

 
void
glmc_vec3_smoothinterp(vec3 from, vec3 to, float t, vec3 dest) {
  glm_vec3_smoothinterp(from, to, t, dest);
}

 
void
glmc_vec3_smoothinterpc(vec3 from, vec3 to, float t, vec3 dest) {
  glm_vec3_smoothinterpc(from, to, t, dest);
}

/* ext */

 
void
glmc_vec3_mulv(vec3 a, vec3 b, vec3 d) {
  glm_vec3_mulv(a, b, d);
}

 
void
glmc_vec3_broadcast(float val, vec3 d) {
  glm_vec3_broadcast(val, d);
}

 
void
glmc_vec3_fill(vec3 v, float val) {
  glm_vec3_fill(v, val);
}

 
bool
glmc_vec3_eq(vec3 v, float val) {
  return glm_vec3_eq(v, val);
}

 
bool
glmc_vec3_eq_eps(vec3 v, float val) {
  return glm_vec3_eq_eps(v, val);
}

 
bool
glmc_vec3_eq_all(vec3 v) {
  return glm_vec3_eq_all(v);
}

 
bool
glmc_vec3_eqv(vec3 a, vec3 b) {
  return glm_vec3_eqv(a, b);
}

 
bool
glmc_vec3_eqv_eps(vec3 a, vec3 b) {
  return glm_vec3_eqv_eps(a, b);
}

 
float
glmc_vec3_max(vec3 v) {
  return glm_vec3_max(v);
}

 
float
glmc_vec3_min(vec3 v) {
  return glm_vec3_min(v);
}

 
bool
glmc_vec3_isnan(vec3 v) {
  return glm_vec3_isnan(v);
}

 
bool
glmc_vec3_isinf(vec3 v) {
  return glm_vec3_isinf(v);
}

 
bool
glmc_vec3_isvalid(vec3 v) {
  return glm_vec3_isvalid(v);
}

 
void
glmc_vec3_sign(vec3 v, vec3 dest) {
  glm_vec3_sign(v, dest);
}

 
void
glmc_vec3_abs(vec3 v, vec3 dest) {
  glm_vec3_abs(v, dest);
}

 
void
glmc_vec3_fract(vec3 v, vec3 dest) {
  glm_vec3_fract(v, dest);
}

 
float
glmc_vec3_hadd(vec3 v) {
  return glm_vec3_hadd(v);
}

 
void
glmc_vec3_sqrt(vec3 v, vec3 dest) {
  glm_vec3_sqrt(v, dest);
}
