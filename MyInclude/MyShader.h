/**
 * Copyright(C),2023-,Ltd.
 * File name: MyShader.h
 * Author: Lisibo
 * Version: 1.0
 * Date: June-1-2023
 * Description: 顶点着色器、片段着色器程序加载，编译
 *              创建着色器程序对象
 * Others:
 * Function List:
 *  1.
 *  2.
 * History:
 *  1.Date:
 *    Author:
 *    Modification:
 *  2.Date:
 *    Author:
 *    Modification:
 *  3....
*/
#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
  /*程序ID*/
  unsigned int ID;
  /* 构造器读取并构建着色器
   * vertexPath:顶点着色器源代码路径
   * fragmentPath:片段着色器源代码路径
   */
  Shader (const char *vertexPath, const char * fragmentPath) {
    /* 从文件路径检索顶点/片段着色器源代码 */
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    /*确保ifstream对象能够抛出异常*/
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
      /* 打开文件 */
      vShaderFile.open (vertexPath);
      fShaderFile.open (fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      /* 读文件数据到流 */
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      /* 关闭文件 */
      vShaderFile.close();
      fShaderFile.close();
      /* 转换流到字符串 */
      vertexCode   = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure &e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    /* 编译着色器 */
    unsigned int vertex, fragment;
    /* 顶点着色器 */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    /* 片段着色器 */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    /* 着色器程序 */
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  /*使用，激活程序*/
  void use() {
    glUseProgram (ID);
  }
  /* uniform工具函数 */
  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
  }
  void setInt(const std::string &name, int value) const {
     glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
  }
  void setFloat(const std::string &name, float value) const {
     glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
  }

private:
  void checkCompileErrors (unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    } else {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
    }
  }
};

#endif
