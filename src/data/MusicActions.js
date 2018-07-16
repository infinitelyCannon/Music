import ActionTypes from './ActionTypes';
import MusicDispatcher from './MusicDispatcher';

const Actions = {
    addSong(dir){
        window.mm.parseFile(dir)
            .then((metadata) =>{
                MusicDispatcher.dispatch({
                    type: ActionTypes.ADD_SONG,
                    info: metadata,
                    dir
                });
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