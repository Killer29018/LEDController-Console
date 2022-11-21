#ifndef LEDMATRIX_RENDERER_HPP
#define LEDMATRIX_RENDERER_HPP

#if FALSE

#include "LEDMatrix.hpp"

#include "glad/gl.h"
#include "KRE/KRE.hpp"

class LEDMatrixRenderer
{
public:
    uint32_t cellSpacing;
private:
    LEDMatrix* m_Matrix;
    uint32_t m_FrameBuffer;
    uint32_t m_ImageBuffer;

    float m_CellSize;

    KRE::Vertices m_Verticies;
    KRE::Indices m_Indicies;

    KRE::VertexArray m_VAO;
    KRE::VertexBuffer m_VBO;
    KRE::ElementArray m_EBO;

    KRE::Shader m_Shader;
public:
    LEDMatrixRenderer() = default;
    ~LEDMatrixRenderer() = default;

    void init(LEDMatrix* matrix, uint16_t spacing);

    void setupImage();
    void renderImGui() override;
private:
    void renderMatrix(int width = 0, int height = 0);

    void setCellSize(int width, int height);
};

#endif
#endif
