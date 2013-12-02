#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

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
        std::fstream wav;
        int ChunkID;
        int size;
        int format;
        int subchunk1ID;
        int subchunk1size;
        int audioFormat;
        int numChannels;
        int sampleRate;
        int subchunk2ID;
        int subchunk2size;
        int bitsPerSample;
        unsigned dataStart;
        wavReader(const std::string& filename);
        std::vector <int> getSample(unsigned pos);
        bool good;
};

wavReader::wavReader(const std::string& filename){
    wav.open(filename.c_str(), std::fstream::in | std::fstream::binary);
    if (wav.is_open()){
        ChunkID = getBytes(wav, 4);
        size = getBytes(wav, 4);
        format = getBytes(wav, 4);
        subchunk1ID = getBytes(wav, 4);
        subchunk1size = getBytes(wav, 4);
        audioFormat = getBytes(wav, 2);
        numChannels = getBytes(wav, 2);
        sampleRate = getBytes(wav, 4);
        getBytes(wav, 6);
        bitsPerSample = getBytes(wav, 2);
        if ((subchunk1ID == stringBytes("fmt ")) && (format == stringBytes("WAVE")) && (ChunkID == stringBytes("RIFF"))){
            if (audioFormat == 1){
                subchunk2ID = 0;
                while (wav.good() && subchunk2ID != stringBytes("data")){
                    subchunk2ID = getBytes(wav, 4);
                }
                subchunk2size = getBytes(wav, 4);
                dataStart = wav.tellg();
                
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

std::vector <int> wavReader::getSample(unsigned pos){
    std::vector <int> channels;
    //wav.seekg(pos+dataStart, wav.beg);
    for (int i = 0; i < numChannels; i++){
        int val = getBytes(wav,bitsPerSample/8);
        channels.push_back(val);
        std::cout << "val: " << val << '\n';
    }
    return channels;
    
}

int main(int argc, char** argv){
    if (argc < 2){
        std::cout << argv[0] << " [options] <filename>\n";
        return 1;
    }else{
        wavReader rdr(argv[1]);
        if (rdr.good){
            for (unsigned j = 0; j < rdr.subchunk2size; j++){
                std::vector <int> channels = rdr.getSample(j);
                for (size_t i = 0; i < channels.size(); i++){
                    std::cout << channels[i] << '\n';
                }
            }
        }
    }
}
