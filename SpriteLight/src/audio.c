u32 LoadAudioStream(const char *file_name)
{
    Mix_Music *music = NULL;
    music = Mix_LoadMUS(file_name);
    if (!music)
    {
        printf("SDL_Mixer: Failed to load music %s\n", Mix_GetError());
        return -1;
    }
    if (state->audio.music_len >= state->audio.music_max - 1)
    {
        state->audio.music_max *= 1.5f;
        state->audio.music = realloc(state->audio.music, sizeof(Mix_Music *) * state->audio.music_max);
    }
    state->audio.music[state->audio.music_len] = music;
    state->audio.music_len++;
    return state->audio.music_len - 1;
}

u32 LoadSound(const char *file_name)
{
    Mix_Chunk *sound = NULL;
    sound = Mix_LoadWAV(file_name);
    if (!sound)
    {
        printf("SDL_Mixer: Failed to load sound %s\n", Mix_GetError());
        return -1;
    }
    if (state->audio.sounds_len >= state->audio.sounds_max - 1)
    {
        state->audio.sounds_max *= 1.5f;
        state->audio.sounds = realloc(state->audio.sounds, sizeof(Mix_Music *) * state->audio.sounds_max);
    }
    state->audio.sounds[state->audio.sounds_len] = sound;
    state->audio.sounds_len++;
    return state->audio.sounds_len - 1;
}

void SetGlobalVolume(u32 volume)
{
    state->audio.volume = (MIX_MAX_VOLUME * volume) / 100;
}

u32 PlayAudioStream(u32 music)
{
    if (!Mix_PlayingMusic())
    {
        Mix_Volume(1, state->audio.volume);
        Mix_PlayMusic(state->audio.music[music], -1);
    }
    return 0;
}

u32 PlaySound(u32 sound)
{
    Mix_Volume(-1, state->audio.volume);
    Mix_PlayChannel(-1, state->audio.sounds[sound], 0);
    return 0;
}

u32 InitAudio()
{
    Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_FLAC | MIX_INIT_OGG);
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_Mixer: couldn't initiate %s\n", Mix_GetError);
        return -1;
    }
    state->audio.music_max = 2;
    state->audio.sounds_max = 10;
    state->audio.sounds = malloc(sizeof(Mix_Chunk *) * state->audio.sounds_max);
    state->audio.music = malloc(sizeof(Mix_Music *) * state->audio.music_max);
    SetGlobalVolume(80);
    return 0;
}

u32 QuitAudio()
{
    for (int sound = 0; sound < state->audio.sounds_len; sound++)
    {
        Mix_FreeChunk(state->audio.sounds[sound]);
        state->audio.sounds[sound] = NULL;
    }
    for (int music = 0; music < state->audio.music_len; music++)
    {
        Mix_FreeChunk(state->audio.music[music]);
        state->audio.music[music] = NULL;
    }
    Mix_Quit();
}

void ToggleAudio()
{
    if (Mix_PausedMusic() == 1)
        Mix_ResumeMusic();
    else
    {
        Mix_PauseMusic();
    }
}