// OpenGL-ES2 utils

void gu_texture_from_file(const char *filename)
{
   struct m_image ubi = M_IMAGE_IDENTITY();
   if (m_image_load(&ubi, filename)) {
      if (ubi.comp == 1)
         glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ubi.width, ubi.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ubi.data);
      else if (ubi.comp == 3)
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ubi.width, ubi.height, 0, GL_RGB, GL_UNSIGNED_BYTE, ubi.data);
      else if (ubi.comp == 4)
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ubi.width, ubi.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ubi.data);
   }
   m_image_destroy(&ubi);
}

GLuint gu_shader_from_string(GLenum type, const char *src)
{
   GLuint shader;
   GLint compiled;

   shader = glCreateShader(type);
   if (shader == 0)
      return 0;

   glShaderSource(shader, 1, &src, NULL);
   glCompileShader(shader);

   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
   if (!compiled) {

      GLint ilen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &ilen);
      
      if (ilen > 0) {
         char *infolog = malloc(sizeof(char) * (ilen + 1));
         glGetShaderInfoLog(shader, ilen, NULL, infolog);
         printf("Error compiling shader:\n%s\n", infolog);
         free(infolog);
      }

      glDeleteShader(shader);
      return 0;
   }

   return shader;
}

void drawMesh() {
    if (index.count())
    {
        if (vaom == 0)
            vaom = vaomId();
        glBindVertexArray(vaom);
        glDrawElements(GL_TRIANGLES, index.count(), GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }
}

void drawShape() {
    if (lines.count())
    {
        if (vaos[0] == 0)
            vaos[0] = vaosId(lines);
        glBindVertexArray(vaos[0]);
        glDrawArrays(GL_LINES, 0, lines.count());
        glBindVertexArray(0);
    }
    if (points.count())
    {
        if (vaos[1] == 0)
            vaos[1] = vaosId(points);
        glBindVertexArray(vaos[1]);
        glDrawArrays(GL_POINTS, 0, points.count());
        glBindVertexArray(0);
    }
}

GLuint vaomId() {
    GLuint vao, buffer[4];

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(4, buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, verts.count() * sizeof(gml::dvec3), verts.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, norm.count() * sizeof(gml::dvec3), norm.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoord.count() * sizeof(gml::dvec2), texCoord.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.count() * sizeof(uint32_t), index.constData(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    qDebug() << vao << ") verts:" << verts.count();
    return vao;
}

GLuint vaosId(const std::vector<gml::dvec3> &buf) {
    GLuint vao, buffer;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &buffer);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, buf.count() * sizeof(gml::dvec3), buf.constData(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(gml::dvec3), NULL); // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2*sizeof(gml::dvec3), (GLvoid *)(1*sizeof(gml::dvec3))); // color
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    qDebug() << vao << ") buffer:" << buf.count();
    return vao;
}
