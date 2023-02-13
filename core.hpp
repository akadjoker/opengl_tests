/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/12 13:51:30 by lrosa-do          #+#    #+#             */
/*   Updated: 2023/02/12 17:00:41 by lrosa-do         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <SDL2/SDL.h>
#include <iostream>
#include <array>
#include <vector>
#include "glad/glad.h"
#include "utils.hpp"
#include "math.hpp"
  

class Input
{
    
    
};
  
class App
{
    public:
        App()
        {
             if (SDL_Init(SDL_INIT_VIDEO) < 0) 
            {
                 Log(2,"SDL could not initialize! Error: %s", SDL_GetError());
                return ;
            }
                m_shouldclose=false;


                SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
                SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        }
        bool CreateWindow(int width, int height, const std::string &tile, bool vsync = true)
        {
        window = SDL_CreateWindow(tile.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (!window) 
        {
            std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << '\n';
            
            return false;
        }

        context = SDL_GL_CreateContext(window);
        if (!context) 
        {
            Log(2, "OpenGL context could not be created! %s ", SDL_GetError());
            SDL_DestroyWindow(window);
            return false; 
        }

        gladLoadGLES2Loader(SDL_GL_GetProcAddress);
        Log(0,"Vendor  :  %s",glGetString(GL_VENDOR));
        Log(0,"Renderer:  %s",glGetString(GL_RENDERER));
        Log(0,"Version :  %s",glGetString(GL_VERSION));

        srand((unsigned int)SDL_GetTicks());              // Initialize random seed
        m_previous = GetTime();       // Get time as double

        (vsync==true)?SDL_GL_SetSwapInterval(1):SDL_GL_SetSwapInterval(0);
            return true;
        }


        
        bool ShouldClose()
        {
              m_current = GetTime();            // Number of elapsed seconds since InitTimer()
              m_update = m_current - m_previous;
              m_previous = m_current;


            SDL_Event event;
            while (SDL_PollEvent(&event)) 
            {
                
                switch (event.type)
                {
                    case SDL_QUIT:
                    {
                        m_shouldclose = true;
                        break;
                    }
                    case SDL_KEYDOWN:
                    {
                        if (event.key.keysym.sym==SDLK_ESCAPE)
                        {
                            m_shouldclose = true;
                            break;
                        }
                
                        break;
                    }
                    
                    case SDL_KEYUP:
                    {
                    
                    }
                    break;
                    case SDL_MOUSEBUTTONDOWN:
                    {
              

                    }break;
                    case SDL_MOUSEBUTTONUP:
                    {
                     
                     break;
                    }
                    case SDL_MOUSEMOTION:
                    {

                    break;   
                    }
                    
                    case SDL_MOUSEWHEEL:
                    {
                   
                    break;
                    }
                }
            } 
            
            return m_shouldclose;
        }
  
        void Close()
        {
            SDL_GL_DeleteContext(context);
            SDL_DestroyWindow(window);
        }

        void Swap()
        {
            SDL_GL_SwapWindow(window);
              // Frame time control system
            m_current = GetTime();
            m_draw = m_current - m_previous;
            m_previous = m_current;

            m_frame = m_update + m_draw;

            // Wait for some milliseconds...
            if (m_frame < m_target)
            {
                Wait((float)(m_target -m_frame)*1000.0f);

                m_current = GetTime();
                double waitTime = m_current - m_previous;
                m_previous = m_current;

                m_frame += waitTime;      // Total frame time: update + draw + wait
            }
        }
        void Wait(float ms)
        {
        SDL_Delay((int)ms);
        }

        ~App()
        {
         Close();
         SDL_Quit();
         Log(0,"Unload and free");
        }

        void SetTargetFPS(int fps)
        {
            if (fps < 1) m_target = 0.0;
            else m_target = 1.0/(double)fps;

            Log(0,"TIMER: Target time per frame: %02.03f milliseconds", (float)m_target*1000);
        }

        int GetFPS(void)
        {
            #define FPS_CAPTURE_FRAMES_COUNT    30      // 30 captures
            #define FPS_AVERAGE_TIME_SECONDS   0.5f     // 500 millisecondes
            #define FPS_STEP (FPS_AVERAGE_TIME_SECONDS/FPS_CAPTURE_FRAMES_COUNT)

            static int index = 0;
            static float history[FPS_CAPTURE_FRAMES_COUNT] = { 0 };
            static float average = 0, last = 0;
            float fpsFrame = GetFrameTime();

            if (fpsFrame == 0) return 0;

            if ((GetTime() - last) > FPS_STEP)
            {
                last = (float)GetTime();
                index = (index + 1)%FPS_CAPTURE_FRAMES_COUNT;
                average -= history[index];
                history[index] = fpsFrame/FPS_CAPTURE_FRAMES_COUNT;
                average += history[index];
            }

            return (int)roundf(1.0f/average);
        }

        // Returns time in seconds for last frame drawn
        float GetFrameTime(void)
        {
            return (float)m_frame;
        }
        double GetTime(void)
        {
        return (double) SDL_GetTicks()/1000.0;
        }

        
    private:
    SDL_Window *window;
    SDL_GLContext context;
    bool m_shouldclose;
        //time
        double m_current;                     // Current time measure
        double m_previous;                    // Previous time measure
        double m_update;                      // Time measure for frame update
        double m_draw;                        // Time measure for frame draw
        double m_frame;                       // Time measure for one frame
        double m_target;                      // Desired time for one frame, if 0 not applied
};