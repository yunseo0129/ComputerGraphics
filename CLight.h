#pragma once
#include "Default.h"

class CLight {
public:
    static CLight* GetInstance();

    void Init();
    void ApplyAmbient(GLuint shaderProgramID);

    void SetAmbientStrength(float strength);
    void ToggleLight();

    bool IsLightOn() const { return lightOn; }

private:
    CLight();
    static CLight* instance;

    bool lightOn;
    float ambientStrength;
    glm::vec3 lightColor;
};

