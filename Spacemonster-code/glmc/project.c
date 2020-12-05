/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "cglm.h"
#include "call.h"

 
void
glmc_unprojecti(vec3 pos, mat4 invMat, vec4 vp, vec3 dest) {
  glm_unprojecti(pos, invMat, vp, dest);
}

 
void
glmc_unproject(vec3 pos, mat4 m, vec4 vp, vec3 dest) {
  glm_unproject(pos, m, vp, dest);
}

 
void
glmc_project(vec3 pos, mat4 m, vec4 vp, vec3 dest) {
  glm_project(pos, m, vp, dest);
}
