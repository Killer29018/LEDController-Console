#include "LEDMatrixRenderer.hpp"

#include "glm/glm.hpp"

void LEDMatrixRenderer::init(LEDMatrix* matrix, uint16_t cellSize, uint16_t spacing)
{
    this->m_Matrix = matrix;
    this->cellSize=cellSize;
    this->cellSpacing = spacing;

    m_Indicies = KRE::Indices({
            0, 1, 2,
            1, 2, 3
            });

    m_Verticies = KRE::Vertices({
            -0.5f, -0.5f,
            -0.5f,  0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
    });

    m_VAO.init();
    m_VBO.init();
    m_EBO.init();

    m_VAO.bind();
    m_EBO.bind();
    m_EBO.setData(m_Indicies);
    m_VAO.unbind();

    setupImage();
}

void LEDMatrixRenderer::setupImage()
{
    m_TotalWidth  = cellSpacing + (m_Matrix->getCols() * (cellSize + cellSpacing));
    m_TotalHeight = cellSpacing + (m_Matrix->getRows() * (cellSize + cellSpacing));

    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    glGenTextures(1, &m_ImageBuffer);
    glBindTexture(GL_TEXTURE_2D, m_ImageBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TotalWidth, m_TotalHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_ImageBuffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    m_VAO.bind();
    m_VBO.bind();

    m_VBO.setData(m_Verticies);
    m_VBO.setVertexAttrib(0, 2, 2, 0);

    m_VBO.unbind();
    m_VAO.unbind();

    m_Shader.compilePath("res/shaders/MatrixVertex.vs.glsl", "res/shaders/MatrixFragment.fs.glsl");
}

void LEDMatrixRenderer::renderImgui()
{
    if (ImGui::Begin("###MATRIX_RENDER"))
    {
        ImGui::BeginChild("Render");
        ImVec2 wSize = ImGui::GetContentRegionAvail();

        cellSize = (int)((float)(wSize.x - cellSpacing - (cellSpacing * m_Matrix->getCols())) / (float)(m_Matrix->getCols()));

        renderMatrix(wSize.x, wSize.y);
        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - wSize.x) * 0.5);
        ImGui::SetCursorPosY((ImGui::GetWindowSize().y - wSize.y) * 0.5);

        ImGui::Image((ImTextureID)m_ImageBuffer, wSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::EndChild();
    }
    ImGui::End();
}

void LEDMatrixRenderer::renderMatrix(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.bind();

    glViewport(0, 0, m_TotalWidth, m_TotalHeight);
    glm::mat4 projection = glm::ortho(0.0f, (float)(width == 0 ? m_TotalWidth : width), (float)(height == 0 ? m_TotalHeight : height), 0.0f);
    m_Shader.setUniformMatrix4("u_Projection", projection);

    m_VAO.bind();

    glm::mat4 model(1.0f);
    for (int y = 0; y < m_Matrix->getRows(); y++)
    {
        for (int x = 0; x < m_Matrix->getCols(); x++)
        {
            int xPos = cellSpacing + (x * (cellSize + cellSpacing));
            int yPos = cellSpacing + (y * (cellSize + cellSpacing));

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
            model = glm::scale(model, glm::vec3(cellSize, cellSize, 1.0f));
            m_Shader.setUniformMatrix4("u_Model", model);
            cRGB& rgb = m_Matrix->getLED(x, y);
            m_Shader.setUniformVector3("u_Colour", glm::vec3(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f));
            // m_Shader.setUniformVector3("u_Colour", glm::vec3(0.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, m_Indicies.getCount(), GL_UNSIGNED_INT, NULL);
        }
    }

    m_VAO.unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
