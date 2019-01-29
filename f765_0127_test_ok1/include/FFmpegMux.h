

#include "libavformat/avformat.h"  
#include "libavutil/avstring.h"  


#include <stdio.h> 
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <objbase.h>

//以视频为基准合成音视频（如果音频比视频短，则音频循环，否则音频截断）
bool Muxer(const char* szVideoInput, const char* szAudioInput, const char* szVideoOutput);


