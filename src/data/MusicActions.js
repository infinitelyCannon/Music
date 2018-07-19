import ActionTypes from './ActionTypes';
import MusicDispatcher from './MusicDispatcher';
import hash from 'hash-sum';
import {URL} from 'url';
import fs from 'fs';

const Actions = {
    addSong(dir){
        window.mm.parseFile(dir)
            .then((metadata) =>{
                if(metadata.common.picture != undefined){
                    let pathURL = new URL('file://' + dir);
                    let path = pathURL.toString();
                    let temp = window.dataPath + '/img/' + hash(path) + '.' + metadata.common.picture[0].format.slice(metadata.common.picture[0].format.indexOf('/') + 1);
                    fs.writeFileSync(temp, metadata.common.picture[0].data);

                    window.imagemin.convert([window.dataPath + '/img/*' + '.{jpg,png,jpeg}'], window.dataPath + '/img', {
                        use: [window.imagemin.plugin({quality: 80})]
                    }).then(() => {
                        fs.unlink(temp, (err) => {if(err) throw err});
                        MusicDispatcher.dispatch({
                            type: ActionTypes.ADD_SONG,
                            info: metadata,
                            dir
                        });
                    });
                }
                else{
                    MusicDispatcher.dispatch({
                        type: ActionTypes.ADD_SONG,
                        info: metadata,
                        dir
                    });
                }
            })
            .catch((err) =>{
                console.error("Music Metadata Error: " + err.message);
            });
    },

    syncAlbums(){
        MusicDispatcher.dispatch({
            type: ActionTypes.SYNC_ALBUM
        });
    },

    syncArtists(){
        MusicDispatcher.dispatch({
            type: ActionTypes.SYNC_ARTIST
        });
    },

    navigateUI(first, second, third){
        MusicDispatcher.dispatch({
            type: ActionTypes.NAVIGATE_UI,
            primary: first,
            secondary: second,
            tertiary: third
        });
    }
};

export default Actions;