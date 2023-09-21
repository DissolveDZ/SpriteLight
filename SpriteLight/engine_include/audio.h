u32 LoadAudioStream(const char *file_name);
u32 LoadSound(const char *file_name);
u32 PlaySound(u32 sound);
u32 PlayAudioStream(u32 music);
void SetGlobalVolume(u32 volume);
u32 InitAudio();
u32 QuitAudio();
void ToggleAudio();