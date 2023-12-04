
const audio = require('./lib').stream
const path = require('node:path')

async function main() {
    

    console.log("Starting predict")
    console.time("predict")
    // const predictResult = await model.predict(`A chat between a user and an Ai Assistant
    // USER: how are you ?
    // ASSISTANT: `,(tokenId,token,total) => {
    //     console.log("Args",total)
    //     return !total.includes('USER:');
    // })
    const hosts = audio.getHosts();
    const defaultHost = hosts[0]
    const mainIn = audio.getHostDevices(defaultHost.id).find(c => c.id === defaultHost.defaultInputDevice);

    const mainOut = audio.getHostDevices(defaultHost.id).find(c => c.id === defaultHost.defaultOutputDevice);
    console.log("USING DEVICE JS",defaultHost,mainIn,mainOut)

    let collected = new Float32Array();

    const sampleRate = mainOut.defaultSampleRate;
    const recordTime = 1;

    const framesPerBuffer = 8000//parseInt(sampleRate * recordTime)

    console.log(framesPerBuffer)
    const outputStream = audio.open({
        output: {
            channelCount : 1 ?? mainOut.maxOutputChannels,
            device: mainOut.id,
            sampleFormat: audio.formats.float32
        }
    },sampleRate,framesPerBuffer)//,framesPerBuffer)

    const inputStream = audio.open({
        input: {
            channelCount : 1 ?? mainIn.maxInputChannels,
            device: mainIn.id,
            sampleFormat: audio.formats.float32,
            callback: (data) => {
    
                outputStream.write(data);
                // // console.log("JS ",data.length)
                const newArr = new Float32Array(collected.length + data.length);
    
                newArr.set(collected)
                newArr.set(data,collected.length);
                collected = newArr
            }
        }
    },sampleRate,framesPerBuffer);

    outputStream.start()
    inputStream.start();

    console.timeEnd("predict")

    while(true){
        await new Promise((r) => setTimeout(r,5000))
        // console.log("Saving",collected.length,"Frames")
        // console.time("Wav Write")
        // audio.wav.write("testData.wav",collected,2,mainIn.defaultSampleRate);
        // console.timeEnd("Wav Write")
        
        // outputStream.write(collected)
        // collected = new Float32Array()
        // await new Promise((r) => setTimeout(r,1000))
    }
}

main()

