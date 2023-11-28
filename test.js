
const audio = require('./lib')
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
    const recordTime = 0.03;

    const framesPerBuffer = parseInt(sampleRate * recordTime)

    console.log(framesPerBuffer)
    const outputStream = audio.createStream(undefined,{
        channelCount : 1 ?? mainOut.maxOutputChannels,
        device: mainOut.id,
        sampleFormat: audio.formats.float32
    },sampleRate,framesPerBuffer)//,framesPerBuffer)

    const inputStream = audio.createStream({
        channelCount : 1 ?? mainIn.maxInputChannels,
        device: mainIn.id,
        sampleFormat: audio.formats.float32,
        callback: (data) => {

            outputStream.write(data);
            // // console.log("JS ",data.length)
            // const newArr = new Float32Array(collected.length + data.length);

            // newArr.set(collected)
            // newArr.set(data,collected.length);
            // collected = newArr
        }
    },undefined,sampleRate,framesPerBuffer);

    outputStream.start()
    inputStream.start();

    console.timeEnd("predict")

    while(true){
        await new Promise((r) => setTimeout(r,5000))
        // outputStream.write(collected)
        // collected = new Float32Array()
        // await new Promise((r) => setTimeout(r,1000))
    }
}

main()

