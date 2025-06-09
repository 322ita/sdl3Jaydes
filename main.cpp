/* clear.c ... */

/*
 * This example code creates an SDL window and renderer, and then clears the
 * window to a different color every frame, so you'll effectively get a window
 * that's smoothly fading between colors.
 *
 * This code is public domain. Feel free to use it for any purpose!
 */

#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_image.h>
#include<SDL3/SDL_audio.h>

 /* We will use this renderer to draw into this window every frame. */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

char* wav_path = NULL;  /* path to the WAV file */
static SDL_AudioSpec audio_spec;  /* audio spec for the WAV file */
static SDL_AudioStream *audio_stream = NULL;  /* audio stream for the WAV file */
static Uint8* audio_buf = NULL;  /* audio buffer for the WAV file */
static Uint32 audio_len = 0;  /* length of the audio buffer */

bool started = false;  /* flag to check if audio has started playing */
bool playing = false;  /* flag to check if audio is playing */

char* UpH_path = NULL;  
SDL_Surface* UpH_surface = NULL;  
SDL_Texture* UpH_texture = NULL;  

char* DownH_path = NULL;  
SDL_Surface* DownH_surface = NULL;  
SDL_Texture* DownH_texture = NULL;  

char* LeftH_path = NULL;  
SDL_Surface* LeftH_surface = NULL;  
SDL_Texture* LeftH_texture = NULL;  

char* RightH_path = NULL;  
SDL_Surface* RightH_surface = NULL;  
SDL_Texture* RightH_texture = NULL;  

char* Cover_path = NULL;  
SDL_Surface* Cover_surface = NULL;  
SDL_Texture* Cover_texture = NULL;  


/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Convenience", 600, 600, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_asprintf(&wav_path, "%s\\audio\\convenience [gqt0BC2bkJY].wav", SDL_GetBasePath());
    if(!SDL_LoadWAV(wav_path, &audio_spec, &audio_buf, &audio_len)) {
        SDL_Log("Couldn't load WAV file: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }



    SDL_asprintf(&Cover_path, "%s\\images\\AlbumCover.jpeg", SDL_GetBasePath());
    Cover_surface = IMG_Load(Cover_path);
    SDL_SetWindowIcon(window, Cover_surface);

    SDL_asprintf(&UpH_path, "%s\\images\\UpH.jpg", SDL_GetBasePath());
    UpH_surface = IMG_Load(UpH_path);
    SDL_asprintf(&DownH_path, "%s\\images\\DownH.jpg", SDL_GetBasePath());
    DownH_surface = IMG_Load(DownH_path);
    SDL_asprintf(&LeftH_path, "%s\\images\\LeftH.jpg", SDL_GetBasePath());
    LeftH_surface = IMG_Load(LeftH_path);
    SDL_asprintf(&RightH_path, "%s\\images\\RightH.jpg", SDL_GetBasePath());
    RightH_surface = IMG_Load(RightH_path);
    if (!Cover_surface ||!UpH_surface || !DownH_surface || !LeftH_surface || !RightH_surface) {
        SDL_Log("Couldn't load image: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    Cover_texture = SDL_CreateTextureFromSurface(renderer, Cover_surface);
    UpH_texture = SDL_CreateTextureFromSurface(renderer, UpH_surface);
    DownH_texture = SDL_CreateTextureFromSurface(renderer, DownH_surface);
    LeftH_texture = SDL_CreateTextureFromSurface(renderer, LeftH_surface);
    RightH_texture = SDL_CreateTextureFromSurface(renderer, RightH_surface);
    if (!Cover_texture || !DownH_texture || !LeftH_texture || !RightH_texture) {

        SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }



    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    if(event->type == SDL_EVENT_KEY_DOWN) {
        SDL_Log("Premuto un tasto: %s", SDL_GetKeyName(event->key.key));
        switch (event->key.key){
            case SDLK_SPACE:
                SDL_Log("Inizio?");
                if(started) break;
                started = true;  /* set the flag to true */
                SDL_Log("Inizio audio");
                audio_stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, NULL, NULL);
                if(!audio_stream) {
                    SDL_Log("Couldn't open audio device: %s", SDL_GetError());
                    return SDL_APP_FAILURE;
                }
                if(!SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audio_spec, NULL, NULL)) {
                    SDL_Log("Couldn't open audio stream: %s", SDL_GetError());
                    return SDL_APP_FAILURE;
                }
                SDL_ResumeAudioStreamDevice(audio_stream);  /* start the audio stream */
                

                break;
            default:
                break;
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void* appstate)
{
    SDL_SetRenderDrawColorFloat(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE_FLOAT);  /* new color, full alpha. */
    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);
    if(started) {
        if(SDL_GetAudioStreamQueued(audio_stream) < int(audio_len) && !playing) {
            SDL_PutAudioStreamData(audio_stream, audio_buf, audio_len);
            SDL_Log("Audio data added to stream"); 
            SDL_Log("Audio queued: %d bytes", SDL_GetAudioStreamQueued(audio_stream));
            SDL_Log("Audio length: %d bytes", int(audio_len));
            playing = true;  /* set the flag to true */
        }

        SDL_RenderTexture(renderer, Cover_texture, NULL, NULL);  /* render the texture to the window. */

        if( SDL_GetAudioStreamQueued(audio_stream) == 0) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    }
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}



