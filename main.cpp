#define SDL_MAIN_USE_CALLBACKS 1 
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <iomanip>
#include <sstream>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

char* wav_path = NULL;  
static SDL_AudioSpec audio_spec;
static SDL_AudioStream *audio_stream = NULL;  
static Uint8* audio_buf = NULL;  
static Uint32 audio_len = 0;  

bool started = false;  
bool playing = false; 

char* UpH_path = NULL;  
SDL_Surface* UpH_surface = NULL;  
SDL_Texture* UpH_texture = NULL;  
SDL_FRect UpH_rect = {86, 20, 224, 176};

char* DownH_path = NULL;  
SDL_Surface* DownH_surface = NULL;  
SDL_Texture* DownH_texture = NULL;
SDL_FRect DownH_rect = {138, 394, 314, 202};  

char* LeftH_path = NULL;  
SDL_Surface* LeftH_surface = NULL;  
SDL_Texture* LeftH_texture = NULL;  
SDL_FRect LeftH_rect = {0, 206, 226, 170};

char* RightH_path = NULL;  
SDL_Surface* RightH_surface = NULL;  
SDL_Texture* RightH_texture = NULL;  
SDL_FRect RightH_rect = {324, 182, 272, 202};

char* Cover_path = NULL;  
SDL_Surface* Cover_surface = NULL;  
SDL_Texture* Cover_texture = NULL;  
SDL_FRect Cover_rect = {0, 0, 300, 300};

char* font_path = NULL;  /* path to the font file */
TTF_Font *font = NULL;  /* font for rendering text */
SDL_Surface *text_surface = NULL;  /* surface for rendering text */
SDL_Texture *text_texture = NULL;  /* texture for rendering text */
std::ostringstream oss;



void TextShow();
double AudioPos();
void Lyrics(std::string &text, double pos);

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

    if(!TTF_Init()) {
        SDL_Log("Couldn't initialize TTF: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_asprintf(&font_path, "%s\\font\\BodoniModa.ttf", SDL_GetBasePath());
    font = TTF_OpenFont(font_path, 24);
    if (!font) {
        SDL_Log("Couldn't open font: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    text_surface = TTF_RenderText_Blended_Wrapped(font, "Convenience\nPRESS SPACE\nTO START", 0, {0,0,0, 255},0);
    if (!text_surface) {
        SDL_Log("Couldn't render text: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture) {
        SDL_Log("Couldn't create texture from surface: %s", SDL_GetError());
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

    SDL_asprintf(&UpH_path, "%s\\images\\UpH.png", SDL_GetBasePath());
    UpH_surface = IMG_Load(UpH_path);
    SDL_asprintf(&DownH_path, "%s\\images\\DownH.png", SDL_GetBasePath());
    DownH_surface = IMG_Load(DownH_path);
    SDL_asprintf(&LeftH_path, "%s\\images\\LeftH.png", SDL_GetBasePath());
    LeftH_surface = IMG_Load(LeftH_path);
    SDL_asprintf(&RightH_path, "%s\\images\\RightH.png", SDL_GetBasePath());
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
    SDL_SetTextureScaleMode(Cover_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(UpH_texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureScaleMode(DownH_texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureScaleMode(LeftH_texture, SDL_SCALEMODE_NEAREST);
        SDL_SetTextureScaleMode(RightH_texture, SDL_SCALEMODE_NEAREST);
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
    double now = double(SDL_GetTicks()) / 1000.0;  /* get the current time in seconds. */
    double sin1 = SDL_sin(now);  /* just to make sure we don't get a warning about unused variables. */

    SDL_RenderClear(renderer);


    if(started) {
        if(SDL_GetAudioStreamQueued(audio_stream) < int(audio_len) && !playing) {
            SDL_PutAudioStreamData(audio_stream, audio_buf, audio_len);
            SDL_Log("Audio data added to stream"); 
            SDL_Log("Audio queued: %d bytes", SDL_GetAudioStreamQueued(audio_stream));
            SDL_Log("Audio length: %d bytes", int(audio_len));
            playing = true;  /* set the flag to true */
        }
        UpH_rect.x = 86 + 10 * sin1;  /* move the texture around a bit. */
        LeftH_rect.y = 206 + 10 * sin1;  /* move the texture around a bit. */
        DownH_rect.x = 138 + 10 * -sin1;  /* move the texture around a bit. */
        RightH_rect.y = 182 + 10 * -sin1;  /* move the texture around a bit. */
        SDL_RenderTexture(renderer, UpH_texture, NULL, &UpH_rect);  /* render the texture to the window. */
        SDL_RenderTexture(renderer, DownH_texture, NULL, &DownH_rect);  /* render the texture to the window. */
        SDL_RenderTexture(renderer, LeftH_texture, NULL, &LeftH_rect);  /* render the texture to the window. */
        SDL_RenderTexture(renderer, RightH_texture, NULL, &RightH_rect);  /* render the texture to the window. */
        
        
        

        TextShow();
        //SDL_Log("Audio position: %f seconds", AudioPos());

        if( SDL_GetAudioStreamQueued(audio_stream) == 0) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    }
    SDL_RenderTexture(renderer, text_texture, NULL, NULL);  /* render the texture to the window. */
    SDL_SetTextureScaleMode(UpH_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(DownH_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(LeftH_texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(RightH_texture, SDL_SCALEMODE_NEAREST);
    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result)
{

}



void TextShow(){
    double pos = AudioPos();  /* get the audio position */
    std::string Testo = "Convenience";  /* text to render */
    oss.clear();  /* clear the string stream */
    oss.str("");  /* clear the string stream */
    //  if(pos>= 0){
    //     oss << std::fixed << std::setprecision(2) << pos;  /* set the precision to 2 decimal places */
    //     Testo = "first\nsecond\nthird\nforth "+oss.str();  /* convert the double to a string */
    //     //Testo = "Convenience - " + std::to_string(int(pos)) + " seconds";  /* convert the double to a string */
    //  }
          /* convert the integer to a string */
    Lyrics(Testo, pos);  /* get the lyrics */
    //SDL_Log("Audio position: %f seconds", pos);
    SDL_DestroySurface(text_surface);  /* free the old surface */                   //fix memory leak
    text_surface = TTF_RenderText_Blended_Wrapped(font, Testo.c_str(), 0, {0,0,0, 255},0);
    SDL_DestroyTexture(text_texture);  /* destroy the old texture */
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    
    

}

double AudioPos(){
    double queued = SDL_GetAudioStreamQueued(audio_stream);
    double bytes_per_second = audio_spec.freq * audio_spec.channels * SDL_AUDIO_BYTESIZE(audio_spec.format);  /* calculate the bytes per second */
    double seconds_remaining = queued / bytes_per_second;  /* calculate the seconds remaining */
    double full_seconds = audio_len / bytes_per_second;  /* calculate the full seconds of audio */
    //SDL_Log("Audio data read from stream: %f bytes, len: %f, now: %f", seconds_remaining, full_seconds, full_seconds - seconds_remaining);
    return full_seconds-seconds_remaining;  /* return the seconds remaining */
}

void Lyrics(std::string &text, double pos){
    if(pos >= 0.3)
        text = "Yeah";
    if(pos >= 1.181)
        text = "I still love you";
    if(pos >= 2.284)
        text = "you can't know that yet";
    if(pos >= 4.519)
        text = "Your voice deteriorates\nin my";
    if(pos >= 6.743)
        text = "Your voice deteriorates\nin my head";
    if(pos >= 8.031)
        text = "I got no time to waste,\nthat's just lately";
    if(pos >= 11.283)
        text = "I got hoes I don't want\nin my bed";
    if(pos >= 14.371)
        text = "You know exactly";
    if(pos >= 15.836)
        text = "You know exactly\nwhat you're doing";
    if(pos >= 18.113)
        text = "uh";
    if(pos >= 21.089)
        text = "You know exactly";
    if(pos >= 22.670)
        text = "You know exactly\nwhat you're doing";
    if(pos >= 24.904)
        text = "yeah";
    if(pos >= 26.582)
        text = "She don't give a fuck";
    if(pos >= 28.648)
        text = "She broke my heart\nbut it got my money up";
    if(pos >= 31.697)
        text = "And i think I smoked\ntoo much";
    if(pos >= 33.186)
        text = "'cause I can't run\nfrom you anymore";
    if(pos >= 35.461)
        text = "Behind closed doors\nI say,";
    if(pos >= 37.140)
        text = "\"Damn\"";
    if(pos >= 37.962)
        text = "Can't believe you'd\ndo this to me";

    if(pos >= 40.093)
        text = "She don't give a fuck";
    if(pos >= 42.146)
        text = "She broke my heart\nbut it got my money up";
    if(pos >= 45.209)
        text = "And i think I smoked\ntoo much";
    if(pos >= 46.649)
        text = "'cause I can't run\nfrom you anymore";
    if(pos >= 48.984)
        text = "Behind closed doors\nI say,";
    if(pos >= 50.647)
        text = "\"Damn\"";
    if(pos >= 52.392)
        text = "Behind closed doors\nI'm like,";
    if(pos >= 54.056)
        text = "\"Damn\"";
    if(pos >= 55.372)
        text = "I swear that you're only\nhere";
    if(pos >= 56.945)
        text = "when you need me";
    if(pos >= 58.625)
        text = "I swear that you're only\nhere";
    if(pos >= 60.155)
        text = "when you need me";
    if(pos >= 62.092)
        text = "I swear that you're only\nhere";
    if(pos >= 63.806)
        text = "when you need me";
    if(pos >= 65.254)
        text = "When it's convenient";
    if(pos >= 66.388)
        text = "";
    
}


/*

[Intro]
(You have no heart!)
Yeah, I still love you, you can't know that yet (Can't know that yet)
Your voice deteriorates in my head
I got no time to waste, that's just lately (That's just lately)
I got hoes I don't want in my bed
You know exactly what you're doing, uh
You know exactly what you're doing, yeah

[Verse]
She don't give a fuck (She don't give a fuck)
She broke my heart but it got my money up (Got my money up)
And I think I smoked too much 'cause I can't run from you anymore
Behind closed doors I say, "Damn
Can't believe you'd do this to me"
She don't give a fuck (She don't give a fuck)
She broke my heart but it got my money up (Got my money up)
And I think I smoked too much 'cause I can't run from you anymore
Behind closed doors I say, "Damn"

[Outro]
Behind closed doors I'm like, "Damn"
I swear that you're only here when you need me
I swear that you're only here when you need me
I swear that you're only here when you need me
When it's convenient

*/