#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs();
mat4 getViewMatrix();
mat4 getProjectionMatrix();

void computeMatOrbit();
mat4 getOrbitProjMat();
mat4 getOrbitViewMat();

#endif