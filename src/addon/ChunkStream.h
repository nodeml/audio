#pragma once
#include <queue>
#include <addon/utils.h>
#include <iostream>

namespace nodeml_audio
{
    struct Chunk
    {
        int format;
        void *data;
        size_t size;

        ~Chunk()
        {
            delete[] data;
        }
    };

    class ChunkStream
    {
        std::queue<Chunk *> chunks;

        int format;

        int channels;

    public:
        void SetFormat(int newFormat){
            format = newFormat;
        }

        void SetChannels(int newChannels){
            channels = newChannels;
        }

        void Put(void *data, size_t size);

        size_t GetData(void *dest, size_t needed);


        template <typename T>
        size_t GetDataTyped(T *dest, size_t needed);

        size_t GetPending(){
            return chunks.size();
        }

        ~ChunkStream(){
            while(!chunks.empty()){
                delete chunks.front();
                chunks.pop();
            }
        }
    };


    inline void ChunkStream::Put(void *data, size_t size)
    {
        auto newData = new Chunk();
        newData->data = data;
        newData->size = size;
        newData->format = format;
        chunks.push(newData);
    }

    inline size_t ChunkStream::GetData(void *dest, size_t needed)
    {
        switch (format)
        {
        case paFloat32:
            return GetDataTyped((float *)dest, needed);
            break;

        case paInt32:
            return GetDataTyped((int32_t *)dest, needed);
            break;

        case paInt24:
            return 0;
            break;

        case paInt16:
            return GetDataTyped((int16_t *)dest, needed);
            break;

        case paInt8:
            return GetDataTyped((int8_t *)dest, needed);
            break;

        case paUInt8:
            return GetDataTyped((uint8_t *)dest, needed);
            break;

        default:
            return 0;
            break;
        }
        
    }

    template <typename T>
    inline size_t ChunkStream::GetDataTyped(T *dest, size_t needed)
    {
        size_t elementsGiven = 0;

        size_t offset = 0;

        auto formatSize = sizeof(T);

        //std::cout << "Total chunks " << chunks.size() << std::endl;
        while (!chunks.empty())
        {
            auto chunk = chunks.front();
            auto chunkSize = chunk->size;
            auto elementsTakenFromChunk = 0;
            auto elementsCurrentlyNeeded = needed - elementsGiven;

            // std::cout << "Getting data from chunk" << std::endl;
            // std::cout << chunkSize << std::endl;
            // std::cout << elementsCurrentlyNeeded << std::endl;

            if (chunkSize <= elementsCurrentlyNeeded)
            {
                memcpy(dest + offset, chunk->data, chunkSize * formatSize);
                elementsTakenFromChunk = chunkSize;
            }
            else
            {
                
                memcpy(dest + offset, chunk->data, elementsCurrentlyNeeded * formatSize);
                elementsTakenFromChunk = elementsCurrentlyNeeded;
                // std::cout << "Getting data Smaller chunk" << std::endl;
                // std::cout << elementsTakenFromChunk << std::endl;
            }

            elementsGiven += elementsTakenFromChunk;

            offset = elementsGiven * formatSize;

            if (elementsTakenFromChunk == chunkSize)
            {
                //std::cout << "Memory Freed" << std::endl;
                delete chunk;
                chunks.pop();
            }
            else
            {
                auto chunksLeft = chunkSize - elementsTakenFromChunk;

                auto newPtr = utils::createFormatPtr(chunksLeft, format);

                memcpy(newPtr, ((T *)chunk->data) + elementsTakenFromChunk, chunksLeft * formatSize);

                delete[] chunk->data;

                chunk->data = newPtr;
                chunk->size = chunksLeft;
                //td::cout << "Resized Chunk" << std::endl;
            }

            if (elementsGiven == needed)
            {
                break;
            }
        }

        //std::cout << "Returning elements " << elementsGiven << std::endl;

        return elementsGiven;
    }
}
