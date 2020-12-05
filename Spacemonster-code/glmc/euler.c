/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cglm.h"
#include "call.h"

 
void
glmc_euler_angles(mat4 m, vec3 dest) {
  glm_euler_angles(m, dest);
}

 
void
glmc_euler(vec3 angles, mat4 dest) {
  glm_euler(angles, dest);
}

 
void
glmc_euler_xyz(vec3 angles,  mat4 dest) {
  glm_euler_xyz(angles, dest);
}

 
void
glmc_euler_zyx(vec3 angles,  mat4 dest) {
  glm_euler_zyx(angles, dest);
}

 
void
glmc_euler_zxy(vec3 angles, mat4 dest) {
  glm_euler_zxy(angles, dest);
}

 
void
glmc_euler_xzy(vec3 angles, mat4 dest) {
  glm_euler_xzy(angles, dest);
}

 
void
glmc_euler_yzx(vec3 angles, mat4 dest) {
  glm_euler_yzx(angles, dest);
}

 
void
glmc_euler_yxz(vec3 angles, mat4 dest) {
  glm_euler_yxz(angles, dest);
}

 
void
glmc_euler_by_order(vec3 angles, glm_euler_seq axis, mat4 dest) {
  glm_euler_by_order(angles, axis, dest);
}
