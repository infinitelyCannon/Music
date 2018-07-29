export const ADD_SONGS = "ADD_SONGS";
export const NAVIGATE_UI = "NAVIGATE_UI";
export const SCAN_DIRECTORIES = "SCAN_DIRECTORIES";
export const ADDITION_FAILURE = "ADDITION_FAILURE";
export const ADDITION_READY = "ADDITION_READY"

export function addSongs(){
    return {
        type: ADD_SONGS
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