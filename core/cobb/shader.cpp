//
// Created by andrew.genualdo on 9/10/2024.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "shader.hpp"
#include "../ew/external/glad.h"



cobb::Shader::Shader(std::string basePath)
{
    load(basePath+".vert", basePath+".frag");
}

cobb::Shader::Shader(string vertexPath, string fragmentPath) {
    load(vertexPath, fragmentPath);
}

void cobb::Shader::load(string vertexPath, string fragmentPath) {
    string vertexCode, fragmentCode;

    ifstream vShaderFile, fShaderFile;

    //This enables the ifstreams to throw errors in case they don't work? I don't understand why they wouldn't already do this
    vShaderFile.exceptions( ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions( ifstream::failbit | ifstream::badbit);

    try {
        //open the files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        //read file contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //close file streams
        vShaderFile.close();
        fShaderFile.close();
        //convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch(ifstream::failure e) {
        //lol successfully is spelled wrong in the guide
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY:READ" << endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();


    //compile the shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    //vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    //print any errors if there are some
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    //linking them
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    if(!success) { //where does success get changed from? inside the vertex/fragment shaders?
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    //clean up the memory since they are already compiled/linked to the program
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void cobb::Shader::use() {
    glUseProgram(ID);
}

void cobb::Shader::setBool(const string &name, bool value) const {
    //this works this way because the gpu doesn't understand booleans, only 1 and 0s
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void cobb::Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void cobb::Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void cobb::Shader::setVec2(const string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

//void cobb::Shader::setTexture2D(const string &name, )