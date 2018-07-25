/* import imagemin from 'imagemin';
import imageminWebp from 'imagemin-webp'; */
const fs = require('fs');

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
                window.imagemin.plugin(/*{quality: 85}*/)
            ]
        })
        .then(() => {
            let items = fs.readdirSync(window.dataPath + '/img');
            let stat;

            for(let i = 0; i < items.length; i++){
                var pic = window.dataPath + '/img/' + items[i];

                stat = fs.statSync(pic);
                if(stat.isFile()){
                    switch(pic.slice(pic.lastIndexOf('.') + 1, pic.length)){
                        case 'png':
                        case 'jpg':
                        case 'jpeg':
                            fs.unlink(pic, (err) => {if(err)console.warn(err.message)});
                            break;
                        default:
                            break;
                    }
                }
            }
        })
        .then(() => dispatch(additionReady(dirs)));
    }
}