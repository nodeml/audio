
const gpt4all = require('./lib')
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
    console.timeEnd("predict")

}

main()

