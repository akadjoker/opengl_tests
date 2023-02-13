#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <vector>

#include "../glad/glad.h"
#include "../utils.hpp"
#include "../render.hpp"
#include "../math.hpp"
#include "../camera.hpp"
#include "../core.hpp"
  
const int screenWidth = 1280;
const int screenHeight = 720;

const char *vertexShaderSource = R"(
#version 320 es
precision mediump float;
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  

   gl_Position = projection * view * model * vec4(FragPos, 1.0);
})";

const char *fragmentShaderSource = R"(
#version 320 es
precision mediump float;

uniform vec3 color;
uniform vec3 viewPos; 

in vec3 Normal;
in vec3 FragPos; 

out vec4 FragColor;

void main()
{
    vec3 lightPos   = vec3 (1.2,1.0,2.0);
    vec3 lightColor = vec3 (1.0,1.0,1.0);

  // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * color;

    FragColor = vec4(result, 1.0);
})";




int run_sample() 
{
App app;
app.CreateWindow(screenWidth, screenHeight, "Ambient Light 2");

Shader shader;
shader.create(vertexShaderSource,fragmentShaderSource);
shader.LoadDefaults();


Camera camera;
camera.SetPosition(0, 1, 10);

 

Surface *cube = Surface::CreateCube();
Surface *plane = Surface::CreatePlane(50,50);


 
   Mat4 projection = Mat4::ProjectionMatrix(45.0f,  static_cast<float>(screenWidth) / screenHeight, 0.1f, 1000.0f);
    

  glClearColor(0.1,0.1,0.3f,1.0);
  glEnable(GL_DEPTH_TEST);  
  

  while (!app.ShouldClose()) 
  {
        
       float deltaTime = app.GetFrameTime();
        


        const Uint8 *keys = SDL_GetKeyboardState(NULL);

         if(keys[SDL_SCANCODE_W])
          camera.ProcessKeyboard(FORWARD, deltaTime);
        else if(keys[SDL_SCANCODE_S])
           camera.ProcessKeyboard(BACKWARD, deltaTime);
  
        if(keys[SDL_SCANCODE_A])
             camera.ProcessKeyboard(LEFT, deltaTime);
        else if(keys[SDL_SCANCODE_D])
             camera.ProcessKeyboard(RIGHT, deltaTime);
           
        
        int MouseX,MouseY;
        
        Uint32 button = SDL_GetRelativeMouseState(&MouseX, &MouseY);
        if (button & SDL_BUTTON(SDL_BUTTON_LEFT)) 
        {
          camera.ProcessMouseMovement(MouseX, -MouseY);
        } 

 
             
       
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       

        Mat4 view =camera.GetViewMatrix();
  

        Mat4 model= Mat4::Identity();
   
        shader.Bind();
        shader.setMatrix4("view", view);
        shader.setMatrix4("projection" ,projection);
        shader.setVector3("viewPos",camera.Position);

        
        shader.setMatrix4("model", model);
        shader.setFloat3("color",0.2f,0.2f,0.2f);
        plane->Render();

        
      //  model = Mat4::Identity();
        model = Mat4::Translate(0,1,0) *  Mat4::Rotate(Vec3(0,1,0),SDL_GetTicks() / 1000.0f) ;
        shader.setFloat3("color",1.0f, 0.5f, 0.2f);
        shader.setMatrix4("model", model);
        cube->Render();

        
      //  model = Mat4::Identity();
        model = Mat4::Translate(-4,1,0)  * Mat4::Rotate(Vec3(0,1,0),SDL_GetTicks() / 1000.0f) ;
        shader.setMatrix4("model", model);
        shader.setFloat3("color",1.0f, 0.5f, 1.0f);
        cube->Render();
     
        model = Mat4::Identity();
        model = Mat4::Translate(4,1,0) * Mat4::Rotate(Vec3(1,0,0),SDL_GetTicks() / 1000.0f) ;     
        shader.setMatrix4("model", model);
        shader.setFloat3("color",0.0f, 0.5f, 1.0f);
        cube->Render();
     

        app.Swap();



  }
  delete cube;
  delete plane;
  
  return 0;
}