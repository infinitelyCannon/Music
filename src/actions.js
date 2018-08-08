export const ADD_SONGS = "ADD_SONGS";
export const NAVIGATE_UI = "NAVIGATE_UI";
export const SCAN_DIRECTORIES = "SCAN_DIRECTORIES";
export const ADDITION_FAILURE = "ADDITION_FAILURE";
export const ADDITION_READY = "ADDITION_READY";
export const NAVIGATE_HISTORY = "NAVIGATE_HISTORY";
export const ADD_TO_QUEUE = "ADD_TO_QUEUE";

export function addSongs(){
    return {
        type: ADD_SONGS
    };
}

export function addToQueue(songs, index){
    return {
        type: ADD_TO_QUEUE,
        list: songs,
        start: index
    };
}

export function navigateHistory(forward){
    return {
        type: NAVIGATE_HISTORY,
        goForward: forward
    };
}

export function navigateUI(target, value){
    return {
        type: NAVIGATE_UI,
        target,
        value
    };
}

export function additionReady(songs){
    return {
        type: ADDITION_READY,
        data: songs
    }
}

export function scanDirectories(dirs){
    return function(dispatch){
        dispatch(addSongs());

        return window.imagemin.convert([window.dataPath + '/img/*.{jpg,png,jpeg}'], window.dataPath + '/img', {
            use: [
                window.imagemin.jpeg(),
                window.imagemin.png()
            ]
        })
        .then(() => dispatch(additionReady(dirs)));
    }
}