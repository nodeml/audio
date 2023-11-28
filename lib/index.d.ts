
type ValueOf<T> = T[keyof T]

export interface IPortAudioHost {
    id: number;
    name: string;
    deviceCount: number;
    defaultInputDevice: number;
    defaultOutputDevice: number;
}

export interface IPortAudioDevice {
    id: number;
    name: string;
    hostId: number;
    maxInputChannels: number;
    maxOutputChannels: number;
    defaultSampleRate: number;
    defaultLowInputLatency: number;
    defaultLowOutputLatency: number;
    defaultHighInputLatency: number;
    defaultHighOutputLatency: number;
}


export namespace formats {
    declare const float32: 0x00000001;
    declare const int32: 0x00000002;
    declare const int24: 0x00000004;
    declare const int16: 0x00000008;
    declare const int8: 0x00000010;
    declare const uint8: 0x00000020;
}

export type StreamFormats = keyof formats | undefined

interface IFormatsToArrays {
    [formats.float32]: Float32Array;
    [formats.int32]: Int32Array;
    [formats.int24]: undefined;
    [formats.int16]: Int16Array;
    [formats.int8]: Int8Array;
    [formats.uint8]: Uint8Array;
}

export interface IStreamParams<T extends StreamFormats = StreamFormats> {
    channelCount: number;
    device: number;
    sampleFormat: T;
    suggestLatency?: number;
}

export type StreamCallback<InputFormat extends StreamFormats = StreamFormats> = (data: InputFormat extends undefined ? undefined : IFormatsToArrays[InputFormat], frameCount: number) => void;


export interface IStreamInputParams<T extends StreamFormats = StreamFormats> extends IStreamParams<T> {
    callback: StreamCallback<T>
}

export declare class Stream<OutputFormat extends StreamFormats> {
    start: () => void;

    stop: () => void;

    close: () => void;

    write: (data: OutputFormat extends undefined ? undefined : IFormatsToArrays[OutputFormat]) => boolean;
}

export declare function createStream<InputFormat extends StreamFormats = undefined, OutputFormat extends StreamFormats = undefined>(input: IStreamInputParams<InputFormat> | undefined, output: IStreamParams<OutputFormat> | undefined, sampleRate: number, framesPerBuffer: number): Stream<OutputFormat>;

export declare function getHosts(): IPortAudioHost[]

export declare function getHostDevices(hostId: number): IPortAudioDevice[]

export declare function getDefaultHostIndex(): number


export namespace wav {
    declare function write(fileName: string,data: Exclude<ValueOf<IFormatsToArrays>,undefined>, channels: number, sampleRate: number): void;
}
