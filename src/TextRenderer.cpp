#include "TextRenderer.h"

TextRenderer::TextRenderer(const char* fontPath)
{
  // Initiate Freetype
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    exit(1);
  }

  FT_Face face;
  if (FT_New_Face(ft, fontPath, 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
    exit(1);
  }
  FT_Set_Pixel_Sizes(face, 0, 48);
  
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
  for (unsigned char c = 0; c < 128; c++)
  {
    // load character glyph 
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    // generate texture
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    Character character = {
      texture, 
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };
    Characters.insert(std::pair<char, Character>(c, character));
  }
  
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
  
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
}

void TextRenderer::type(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
  shader.Activate();
  glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);
  
  glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
  
  // Make the glyphs visible
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  // iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;
    // update VBO for each character
    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },            
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }           
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}