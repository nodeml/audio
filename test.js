
const portaudio = require('./lib')
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
    const hosts = portaudio.getHosts();
    const defaultHost = hosts[portaudio.getDefaultHostIndex()]
    console.log(hosts,defaultHost)
    const mainMic = portaudio.getHostDevices(defaultHost.id)[1]
    console.log("USING DEVICE JS",mainMic)
    const stream = portaudio.Stream.create({
        channelCount : 1,
        device: mainMic.id,
        sampleFormat: portaudio.formats.float32
    },undefined,16000,8000,(audioData) => {
        console.log("Got Audio",audioData)

        return 0;
    })
    stream.start()
    console.log(defaultHost)
    console.timeEnd("predict")

    while(true){
        await new Promise((r) => setTimeout(r,5000))
    }
}

main()

