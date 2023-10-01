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
    state->audio.music[state->audio.music_len][0].music = music;
    state->audio.music_len++;
    return state->audio.music_len - 1;
}

u32 LoadSound(const char *file_name)
{
    Mix_Chunk *sound = NULL;
    if (state->audio.sounds_len >= state->audio.sounds_max - 1)
    {
        state->audio.sounds_max *= 1.5f;
        state->audio.sounds = realloc(state->audio.sounds, sizeof(Mix_Music *) * state->audio.sounds_max);
    }
    for (int i = 0; i < state->audio.max_parallel_sounds; i++)
        state->audio.sounds[state->audio.sounds_len][i].chunk = Mix_LoadWAV(file_name);
    state->audio.sounds_len++;
    return state->audio.sounds_len - 1;
}

void SetGlobalVolume(u32 volume)
{
    state->audio.volume = (MIX_MAX_VOLUME * volume) / 100;
}

void SetVolume(u32 sound, u8 volume)
{
    for(int i = 0; i < state->audio.parallel_sounds; i++)
        state->audio.sounds[sound][i].chunk->volume = (MIX_MAX_VOLUME * volume) / 100;
}

void SetAudioStreamVolume(u32 stream, u8 volume)
{
    Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);
}

u32 PlayAudioStream(u32 music)
{
    if (!Mix_PlayingMusic())
    {
        Mix_Volume(-1, Mix_GetMusicVolume(state->audio.music[music][0].music) * state->audio.volume);
        Mix_PlayMusic(state->audio.music[music][0].music, -1);
    }
    return 0;
}

u32 PlaySound(u32 sound)
{
    Mix_Volume(1, state->audio.sounds[sound][state->audio.parallel_sounds].chunk->volume);
    Mix_PlayChannel(-1, state->audio.sounds[sound][state->audio.parallel_sounds].chunk, 0);
    if (state->audio.parallel_sounds < state->audio.max_parallel_sounds - 1)
        state->audio.parallel_sounds++;
    else
        state->audio.parallel_sounds = 0;
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
    state->audio.max_parallel_sounds = 10;
    state->audio.max_parallel_musics = 10;
    state->audio.sounds = malloc(sizeof(void *) * state->audio.sounds_max);
    for (int i = 0; i < state->audio.sounds_max; i++)
        state->audio.sounds[i] = malloc(state->audio.max_parallel_sounds * sizeof(Sound *));
    state->audio.music = malloc(sizeof(void *) * state->audio.music_max);
    for (int i = 0; i < state->audio.music_max; i++)
        state->audio.music[i] = malloc(state->audio.max_parallel_musics * sizeof(Music *));
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