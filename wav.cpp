#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

int getBytes(std::istream& binFile,const size_t num){
    int bytes = 0;
    for (size_t size = 0; size < num; ++size){
        bytes |= binFile.get() << (8 * size);
    }
    return bytes;
};

int stringBytes(const std::string& str){
    std::stringstream stringy;
    stringy << str;
    return getBytes(stringy, str.length());
};

class wavReader{
    public:
        wavReader(const std::string& filename){};
        bool good;
};

wavReader(const std::string& filename){
    std::fstream wav;
    wav.open(filename.c_str(), std::fstream::in | std::fstream::binary);
    if (wav.is_open()){
        int ChunkID = getBytes(wav, 4);
        int size = getBytes(wav, 4);
        int format = getBytes(wav, 4);
        int subchunk1ID = getBytes(wav, 4);
        int subchunk1size = getBytes(wav, 4);
        int audioFormat = getBytes(wav, 2);
        int numChannels = getBytes(wav, 2);
        int sampleRate = getBytes(wav, 4);
        getBytes(wav, 6);
        int bitsPerSample = getBytes(wav, 2);
        if ((subchunk1ID == stringBytes("fmt ")) && (format == stringBytes("WAVE")) && (ChunkID == stringBytes("RIFF"))){
            if (audioFormat == 1){
                int subchunk2ID = 0;
                while (wav.good() && subchunk2ID != stringBytes("data")){
                    subchunk2ID = getBytes(wav, 4);
                }
                int subchunk2size = getBytes(wav, 4);
                std::cout << subchunk2size << '\n';
            }else{
                std::cout << "Non-PCM file detected\n";
                good = false;
            }
        }else{
            std::cout << "Not valid WAV file\n";
            good = false;
        }
        wav.close();
    }else{
        good = false;
        std::cout << "Could not open " << filename << '\n';
        wav.close();
    }
}

int main(int argc, char** argv){
    if (argc < 2){
        std::cout << argv[0] << " [options] <filename>\n";
        return 1;
    }else{
        rdr = wavReader(argv[1]);
    }
