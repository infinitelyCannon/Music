import Immutable from 'immutable';
import Song from './Song';
import {ReduceStore} from 'flux/utils';
import MusicDispatcher from './MusicDispatcher';
import ActionTypes from './ActionTypes';
import _ from 'underscore';
import {ipcRenderer} from 'electron';

function safeVal(value, fallback){
    if(typeof value == undefined || value == null){
        return fallback;
    }
    if(typeof value == 'object' && value.length != undefined){
        return _.uniq(value).toString().replace(',', ', ');
    }
    else{
        return value.trim();
    }
}

class SongStore extends ReduceStore{
    constructor(){
        super(MusicDispatcher);
    }

    getInitialState(){
        let music = window.eStore.get('songs');
        /* let albums = window.eStore.get('albums');
        let artists = window.eStore.get('artists'); */

        return ((music == undefined) ? Immutable.List() : Immutable.List(music));
    }

    reduce(state, action){
        let result, fileName = action.dir.replace('/', '\\');
        ipcRenderer.on('asynchronous-reply', (event, arg) => {
            //console.log(arg);
        });
        switch(action.type){
            case ActionTypes.ADD_SONG:
                if(state.find( (value) => {return value.id === action.dir.replace('/', '\\')}) === undefined){
                    result = state.push(Object.assign({}, Song, {
                        id: action.dir.replace('/', '\\'),
                        title: safeVal(action.info.common.title, fileName.slice(fileName.lastIndexOf('\\') + 1, fileName.lastIndexOf('.'))),
                        artist: safeVal(action.info.common.artists, "Unknown Artist"),
                        albumArtist: safeVal(action.info.common.albumartist, safeVal(action.info.common.artists, "Unknown Artist")),
                        album: safeVal(action.info.common.album, "Unknown Album"),
                        year: safeVal(action.info.common.year, NaN),
                        genre: safeVal(action.info.common.genre, ""),
                        duration: action.info.format.duration,
                        cover: safeVal(action.info.common.picture[0].data, []),
                        coverType: safeVal(action.info.common.picture[0].format, ""),
                        trackNum: safeVal(action.info.common.track.no, NaN),
                        diskNum: safeVal(action.info.common.disk.no, NaN)
                    }))
                    .sort((a, b) =>{
                        if(a.title < b.title){return -1;}
                        if(a.title > b.title){return 1;}
                        if(a.title === b.title){return 0;}
                    });
                    ipcRenderer.send('asynchronous-message', result.toArray());
                    return result;
                }
                else{
                    return state
                }
            default:
                return state;
        }
    }
}

export default new SongStore();