#include "LEDMatrixRenderer.hpp"

#include "glm/glm.hpp"

void LEDMatrixRenderer::init(LEDMatrix* matrix, uint16_t spacing)
{
    this->m_Matrix = matrix;
    this->cellSpacing = spacing;

    m_CellSize = 0;

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
    glGenFramebuffers(1, &m_FrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

    glGenTextures(1, &m_ImageBuffer);

    glBindTexture(GL_TEXTURE_2D, m_ImageBuffer);
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

void LEDMatrixRenderer::renderImGui()
{
    if (ImGui::Begin("Preview"))
    {
        ImGui::BeginChild("Render");
        ImVec2 wSize = ImGui::GetContentRegionAvail();

        setCellSize(wSize.x, wSize.y);

        glBindTexture(GL_TEXTURE_2D, m_ImageBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wSize.x, wSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.bind();

    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
    m_Shader.setUniformMatrix4("u_Projection", projection);

    m_VAO.bind();

    glm::mat4 model(1.0f);

    float sizeX = (cellSpacing + (m_CellSize + cellSpacing) * m_Matrix->getColumns()) / 2.0;
    float sizeY = (cellSpacing + (m_CellSize + cellSpacing) * m_Matrix->getRows()) / 2.0;
    float xStart = (width / 2) - sizeX;
    float yStart = (height / 2) - sizeY;
    for (int y = 0; y < m_Matrix->getRows(); y++)
    {
        for (int x = 0; x < m_Matrix->getColumns(); x++)
        {
            float xPos = xStart + (m_CellSize / 2) + cellSpacing + (x * (m_CellSize + cellSpacing));
            float yPos = yStart + (m_CellSize / 2) + cellSpacing + (y * (m_CellSize + cellSpacing));

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(xPos, yPos, 0.0f));
            model = glm::scale(model, glm::vec3(m_CellSize, m_CellSize, 1.0f));
            m_Shader.setUniformMatrix4("u_Model", model);
            cRGB rgb = m_Matrix->getLEDWBrightness(x, y);
            m_Shader.setUniformVector3("u_Colour", glm::vec3(rgb.r / 255.0f, rgb.g / 255.0f, rgb.b / 255.0f));
            glDrawElements(GL_TRIANGLES, m_Indicies.getCount(), GL_UNSIGNED_INT, NULL);
        }
    }

    m_VAO.unbind();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LEDMatrixRenderer::setCellSize(int width, int height)
{
    float aspectRatio = (float)m_Matrix->getColumns() / (float)m_Matrix->getRows();

    int correctWidth = height * aspectRatio;
    int correctHeight = width / aspectRatio;

    int widthDiff = correctWidth - width;
    int heightDiff = correctHeight - height;

    float size = (heightDiff < widthDiff) ? width : height;
    float side = (heightDiff < widthDiff) ? m_Matrix->getColumns() : m_Matrix->getRows();

    m_CellSize = (size  - cellSpacing - (cellSpacing * side)) / side;
}
