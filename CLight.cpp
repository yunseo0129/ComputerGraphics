#include "CLight.h"

CLight* CLight::instance = nullptr;

CLight::CLight()
    : lightOn(true), ambientStrength(0.3f), lightColor(1.0f, 1.0f, 1.0f) {
}

CLight* CLight::GetInstance() {
    if (!instance)
        instance = new CLight();
    return instance;
}

void CLight::Init() {
    lightOn = true;
    ambientStrength = 0.3f;
    lightColor = glm::vec3(1.0f);
}

void CLight::SetAmbientStrength(float strength) {
    ambientStrength = strength;
}

void CLight::ToggleLight() {
    lightOn = !lightOn;
}

void CLight::ApplyAmbient(GLuint shaderProgramID) {
    GLint loc;

    loc = glGetUniformLocation(shaderProgramID, "lightOn");
    if (loc >= 0) glUniform1i(loc, lightOn ? 1 : 0);

    loc = glGetUniformLocation(shaderProgramID, "ambientStrength");
    if (loc >= 0) glUniform1f(loc, ambientStrength);

    loc = glGetUniformLocation(shaderProgramID, "lightColor");
    if (loc >= 0) glUniform3f(loc, lightColor.r, lightColor.g, lightColor.b);
}
